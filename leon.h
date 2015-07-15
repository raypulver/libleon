#ifndef LEON_H
#define LEON_H

#include <string>
#include <vector>
#include <map>

namespace LEON {
  typedef union {
    long i;
    double fp;
  } NUMERIC;
  class Value {
  public:
    NUMERIC num;
    char type;
    std::map<std::string, Value> obj;
    std::vector<Value> arr;
    std::string str;
    static Value NaN();
    static Value Null();
    static Value RegExp(const char*);
    static Value Buffer(const char*);
    static Value RegExp(std::string);
    static Value Date(unsigned int);
    Value();
    Value(unsigned char);
    Value(char);
    Value(unsigned short);
    Value(short);
    Value(unsigned int);
    Value(unsigned int, unsigned char);
    Value(int);
    Value(unsigned long);
    Value(long);
    Value(bool);
    Value(float);
    Value(double);
    Value(std::string);
    Value(std::string, unsigned char);
    Value(const char*);
    Value(const char*, unsigned char);
    Value(std::map<std::string, Value>&);
    Value(std::vector<Value>&);
    std::string toString();
    unsigned char toUnsignedChar();
    char toChar();
    unsigned short toUnsignedShort();
    short toShort();
    unsigned int toUnsignedInt();
    int toInt();
    bool toBoolean();
    float toFloat();
    double toDouble();
    std::map<std::string, Value>& toMap();
    std::vector<Value>& toVector();
    void set(unsigned char);
    void set(char);
    void set(unsigned short);
    void set(short);
    void set(unsigned int);
    void set(unsigned int, unsigned char);
    void set(int);
    void set(bool);
    void set(float);
    void set(double);
    void set(std::string);
    void set(std::string, unsigned char);
    void set(const char*);
    void set(const char*, unsigned char);
    void set(std::map<std::string, Value>&);
    void set(std::string, Value);
    void set(std::vector<Value>&);
    unsigned int size();
    void push(Value);
    Value pop();
    Value& operator[](std::string);
    Value& operator[](unsigned long);
    void reset();
  };
  std::string stringify (Value);
  Value parse (std::string);
  class Channel {
    Value spec;
  public:
    Channel(Value);
    std::string stringify (Value);
    Value parse (std::string);
  };
};

#define LEON_UNSIGNED_CHAR '\0'
#define LEON_CHAR '\1'
#define LEON_UNSIGNED_SHORT '\2'
#define LEON_SHORT '\3'
#define LEON_UNSIGNED_INT '\4'
#define LEON_INT '\5'
#define LEON_FLOAT '\6'
#define LEON_DOUBLE '\7'
#define LEON_ARRAY '\200'
#define LEON_OBJECT '\11'
#define LEON_STRING '\20'
#define LEON_TRUE '\40'
#define LEON_FALSE '\41'
#define LEON_BOOLEAN '\40'
#define LEON_NULL '@'
#define LEON_UNDEFINED '\24'
#define LEON_DATE '\25'
#define LEON_BUFFER '\26'
#define LEON_REGEXP '\27'
#define LEON_NAN '\30'
#define LEON_EMPTY '\377'

#endif
