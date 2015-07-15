#ifndef VALUE_H
#define VALUE_H

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

};

#endif
