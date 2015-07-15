#include <vector>
#include <map>
#include <string>
#include <iostream>
#include <exception>
#include "exceptions.h"
#include "types.h"
#include "value.h"

using namespace std;
using namespace LEON;

static bool is_numeric(const Value*);

Value::Value() : type(LEON_UNDEFINED) {}
Value::Value(unsigned char v) : type(LEON_UNSIGNED_CHAR) {
  num.i = v;
}
Value::Value(char v) : type(LEON_CHAR) {
  num.i = v;
}
Value::Value(unsigned short v) : type(LEON_UNSIGNED_SHORT) {
  num.i = v;
}
Value::Value(short v) : type(LEON_SHORT) {
  num.i = v;
}
Value::Value(unsigned int v) : type(LEON_UNSIGNED_INT) {
  num.i = v;
}
Value::Value(unsigned int v, unsigned char t) {
  num.i = v;
  if (t == LEON_DATE) {
    type = LEON_DATE;
  } else type = LEON_UNSIGNED_INT;
}
Value::Value(int v) : type(LEON_INT) {
  num.i = v;
}
Value::Value(unsigned long v) : type(LEON_UNSIGNED_INT) {
  num.i = static_cast<long>(v);
}
Value::Value(long v) : type(LEON_INT) {
  num.i = v;
}
Value::Value(bool v) {
  if (v) type = LEON_TRUE;
  else type = LEON_FALSE;
  num.i = v;
}
Value::Value(float v) : type(LEON_FLOAT) {
  num.fp = v;
}
Value::Value(double v) : type(LEON_DOUBLE) {
  num.fp = v;
}
Value::Value(string v) : type(LEON_STRING), str(v) {}
Value::Value(string v, unsigned char t) : str(v), type(t) {
  str = v;
  if (t == LEON_REGEXP) {
    type = LEON_REGEXP;
  } else type = LEON_STRING;
}
Value::Value(const char* v) : Value(string(v)) {}
Value::Value(const char* v, unsigned char t) : Value(string(v), t) {}
Value::Value(map<string, Value>& v) : type(LEON_OBJECT), obj(v) {}
Value::Value(vector<Value>& v) : type(LEON_ARRAY), arr(v) {}
Value Value::RegExp(const char* v) {
  return Value(v, LEON_REGEXP);
}
Value Value::RegExp(string v) {
  return Value(v, LEON_REGEXP);
}
Value Value::Date(unsigned int v) {
  return Value(v, LEON_DATE);
}
Value Value::Null() {
  Value ret;
  ret.type = LEON_NULL;
  return ret;
}
Value Value::NaN() {
  Value ret;
  ret.type = LEON_NAN;
  return ret;
}
Value Value::Buffer(const char* v) {
  Value ret(v);
  ret.type = LEON_BUFFER;
  return ret;
}
string Value::toString() {
  if (type != LEON_STRING && type != LEON_REGEXP && type != LEON_BUFFER) throw StringException();
  return str;
}
unsigned char Value::toUnsignedChar() {
  if (!is_numeric(this)) throw NumericException();
  unsigned char ret = (unsigned char) num.i;
  if (num.i != ret) throw UnsignedCharException();
  return ret;
}
char Value::toChar() {
  if (!is_numeric(this)) throw NumericException();
  char ret = (char) num.i;
  if (num.i != ret) throw CharException();
  return ret;
}
unsigned short Value::toUnsignedShort() {
  if (!is_numeric(this)) throw NumericException();
  unsigned short ret = (unsigned short) num.i;
  if (num.i != ret) throw UnsignedShortException();
  return ret;
}
short Value::toShort() {
  if (!is_numeric(this)) throw NumericException();
  short ret = (short) num.i;
  if (num.i != ret) throw ShortException();
  return ret;
}
unsigned int Value::toUnsignedInt() {
  if (!is_numeric(this)) throw NumericException();
  unsigned int ret = (unsigned int) num.i;
  if (num.i != ret) throw UnsignedIntException();
  return ret;
}
int Value::toInt() {
  if (!is_numeric(this)) throw NumericException();
  int ret = (int) num.i;
  if (num.i != ret) throw IntException();
  return ret;
}
bool Value::toBoolean() {
  if (type != LEON_FALSE && type != LEON_TRUE) throw BoolException();
  return (bool) num.i;
}
float Value::toFloat() {
  if (type != LEON_FLOAT) throw FloatException();
  return (float) num.fp;
}
double Value::toDouble() {
  if (type != LEON_DOUBLE) throw DoubleException();
  return (double) num.fp;
}
map<string, Value>& Value::toMap() {
  if (type != LEON_OBJECT) throw ObjectException();
  return obj;
}
vector<Value>& Value::toVector() {
  if (type != LEON_ARRAY) throw ArrayException();
  return arr;
}
void Value::reset() {
  type = LEON_UNDEFINED;
  arr.clear();
  obj.clear();
  str = "";
  num.i = 0;
}
void Value::set(unsigned char v) {
  reset();
  type = LEON_UNSIGNED_CHAR;
  num.i = v;
}
void Value::set(char v) {
  reset();
  type = LEON_CHAR;
  num.i = v;
}
void Value::set(unsigned short v) {
  reset();
  type = LEON_UNSIGNED_SHORT;
  num.i = v;
}
void Value::set(short v) {
  reset();
  type = LEON_SHORT;
  num.i = v;
}
void Value::set(unsigned int v) {
  reset();
  type = LEON_UNSIGNED_INT;
  num.i = v;
}
void Value::set(unsigned int v, unsigned char t) {
  reset();
  if (t == LEON_DATE) {
    type = LEON_DATE;
    num.i = v;
  } else set(v);
}
void Value::set(int v) {
  reset();
  type = LEON_INT;
  num.i = v;
}
void Value::set(bool v) {
  reset();
  type = LEON_BOOLEAN;
  num.i = (long) v;
}
void Value::set(float v) {
  reset();
  type = LEON_FLOAT;
  num.fp = v;
}
void Value::set(double v) {
  reset();
  type = LEON_DOUBLE;
  num.fp = v;
}
void Value::set(string v) {
  reset();
  type = LEON_STRING;
  str = v;
}
void Value::set(string v, unsigned char t) {
  reset();
  if (t == LEON_REGEXP) {
    type = LEON_REGEXP;
    str = v;
  } else set(v);
}
void Value::set(const char* v) {
  set(string(v));
}
void Value::set(map<string, Value>& v) {
  reset();
  type = LEON_OBJECT;
  obj = v;
}
void Value::set(string k, Value v) {
  if (type != LEON_OBJECT) {
    reset();
    type = LEON_OBJECT;
  }
  obj[k] = v;
}
void Value::set(vector<Value>& v) {
  reset();
  type = LEON_ARRAY;
  arr = v;
}
unsigned int Value::size() {
  if (type == LEON_ARRAY) {
    return arr.size();
  } else if (type == LEON_OBJECT) {
    return obj.size();
  } else throw ObjectArrayException();
}
void Value::push(Value v) {
  if (type != LEON_ARRAY) {
    reset();
    type = LEON_ARRAY;
  }
  arr.push_back(v);
}
Value Value::pop() {
  if (type != LEON_ARRAY) {
    reset();
    type = LEON_ARRAY;
  }
  Value ret = arr[arr.size() - 1];
  arr.pop_back();
  return ret;
}
Value& Value::operator[] (string k) {
  if (type != LEON_OBJECT) {
    reset();
    type = LEON_OBJECT;
  }
  return obj[k];
}
Value& Value::operator[] (unsigned long i) {
  if (type != LEON_ARRAY) {
    reset();
    type = LEON_ARRAY;
  }
  if (i >= arr.size()) {
    for (unsigned j = arr.size(); j < i + 1; ++j) {
      arr.push_back(Value());
    }
  }
  return arr[i];
}

static bool is_numeric(const Value* v) {
  switch (v->type) {
    case LEON_UNSIGNED_CHAR:
    case LEON_CHAR:
    case LEON_UNSIGNED_SHORT:
    case LEON_SHORT:
    case LEON_UNSIGNED_INT:
    case LEON_INT:
    case LEON_FLOAT:
    case LEON_DOUBLE:
    case LEON_DATE:
      return true;
    default:
      return false;
  }
}
