# libleon
This repo contains the C++ implementation of LEON, capable of encoding and decoding any JavaScript data structure into and from a compact string.

## Install
`$ make`

`$ make install`

## Usage
The heart of the library is the `LEON::Value` class. You can construct a `LEON::Value` with any primitive, string, a `vector<LEON::Value>`, or `map<std::string, LEON::Value>`. An easier way to construct an Array or Object is by using the [] operator.  As an example:

```
#include <leon.h>
int main(int argc, char** argv) {
  LEON::Value val;
  val["firstkey"] = true;
  val["secondkey"] = 5;
  Value LEON::parse(LEON::stringify(val));
  
}
// {"firstkey":true,"secondkey":5}
```
`LEON::parse` returns a Value object. A value object has a `type` property that can be used to discern it's type. The following example shows how to do this:

```
// extern string somestring
LEON::Value val = LEON::parse(somestring);
switch (val.type) {
  case LEON_UNSIGNED_CHAR:
    unsigned char c = val.toUnsignedChar();
    break;
  case LEON_CHAR:
    char c = val.toChar();
    break;
  case LEON_UNSIGNED_SHORT:
    unsigned short s = val.toUnsignedShort();
    break;
  case LEON_SHORT:
    short s = val.toShort();
    break;
  case LEON_UNSIGNED_INT:
  case LEON_DATE:
    unsigned int i = val.toUnsignedInt();
    break;
  case LEON_INT:
    int i = val.toInt();
    break;
  case LEON_FLOAT:
    float f = val.toFloat();
    break;
  case LEON_DOUBLE:
    double d = val.toDouble();
    break;
  case LEON_ARRAY:
    std::vector<LEON::Value> v = val.toVector();
    break;
  case LEON_OBJECT:
    std::map<std::string, LEON::Value> m = val.toMap();
    break;
  case LEON_STRING:
  case LEON_REGEXP:
  case LEON_BUFFER:
    std::string s = val.toString();
    break;
  case LEON_BOOLEAN:
    bool b = val.toBoolean();
    break;
  case LEON_NULL:
  case LEON_UNDEFINED:
  case LEON_NAN:
  case LEON_POSITIVE_INFINITY:
  case LEON_MINUS_INFINITY:
    // no conversion
    break;
```
If you want to serialize `null` or `NaN`, you can get a `LEON::Value` of that type by calling `LEON::Value::Null()` or `LEON::Value::NaN()`. If you want `undefined` you can simply construct `LEON::Value` with no arguments. Other special values are a `Date` which can be constructed by passing a timestamp to `LEON::Value::Date(unsigned int)`, a `RegExp` which can be constructed by passing its string representation to `LEON::Value::RegExp(std::string)`, and a `Buffer` object which can be constructed by passing an `unsigned char` array to `LEON::Value::Buffer(const unsigned char*)`.

Just like in the JavaScript implementation, you can construct a `LEON::Channel` by passing a `LEON::Value` to its constructor that is a template of the data to be sent. The same rules apply as in the JavaScript version, i.e. an array must consist of the same type, and if the values are objects they must have the same keys and associated values. An example:

```
LEON::Value dataTemplate;
dataTemplate[0]["firstkey"] = LEON_CHAR;
dataTemplate[0]["secondkey"] = LEON_BOOLEAN;
LEON::Channel channel (dataTemplate);
LEON::Value list;
list[0]["firstkey"] = -100;
list[0]["secondkey"] = true;
list[1]["firstkey"] = 120;
list[1]["secondkey"] = false;
LEON::Value sameList = channel.parse(channel.stringify(list));
```
