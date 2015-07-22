#include <string>
#include <vector>
#include <exception>
#include <iostream>
#include "buffer-iterator.h"
#include "string-buffer.h"
#include "value.h"
#include "parser.h"
#include "exceptions.h"

using namespace std;
using namespace LEON;

Parser::Parser(const std::string s) : hasSpec(false), buffer(BufferIterator(s)), state(0) {}
Parser::Parser(const std::string s, const Value& v) : hasSpec(true), buffer(BufferIterator(s)), spec(v), state(0) {}
string Parser::readString() {
  string ret;
  unsigned char c;
  Value lenVal = buffer.readValue(buffer.readUInt8());
  unsigned int len = lenVal.toUnsignedInt();
  for (unsigned i = 0; i < len; ++i) {
    c = buffer.readUInt8();
    ret += c;
  }
  return ret;
}
Parser* Parser::parseSI() {
  if (state & 0x01 != 0) throw RepeatException();
  Value stringCountVal;
  unsigned int stringCount;
  stringIndexType = buffer.readUInt8();
  switch (stringIndexType) {
    case LEON_UNSIGNED_CHAR:
    case LEON_UNSIGNED_SHORT:
    case LEON_UNSIGNED_INT:
      stringCountVal = buffer.readValue(stringIndexType);
      break;
    case LEON_EMPTY:
      stringCount = 0;
      break;
    default:
      throw ShouldBeUnsignedException();
  }
  stringCount = stringCountVal.toUnsignedChar();
  for (unsigned i = 0; i < stringCount; ++i) {
    stringIndex.push_back(readString());
  }
  state |= 0x01;
  return this;
}

Parser* Parser::parseOLI() {
  if (state & 0x01 == 0) parseSI();
  if (stringIndex.size() == 0) return this;
  OLItype = buffer.readUInt8();
  Value countVal;
  switch (OLItype) {
    case LEON_UNSIGNED_CHAR:
    case LEON_UNSIGNED_SHORT:
    case LEON_UNSIGNED_INT:
      countVal = buffer.readValue(OLItype);
      break;
    case LEON_EMPTY:
      return this;
    default:
      throw ShouldBeUnsignedException();
  }
  unsigned int count = countVal.toUnsignedInt();
  for (unsigned i = 0; i < count; ++i) {
    objectLayoutIndex.push_back(vector<unsigned int>());
    Value numFieldsVal = buffer.readValue(buffer.readUInt8());
    unsigned int numFields = numFieldsVal.toUnsignedInt();
    for (unsigned j = 0; j < numFields; ++j) {
      Value indexVal = buffer.readValue(stringIndexType);
      unsigned int index = indexVal.toUnsignedInt();
      objectLayoutIndex[i].push_back(index);
    }
  }
  return this;
}
Value Parser::parseValueWithSpec() {
  return parseValueWithSpec(spec);
}
Value Parser::parseValueWithSpec(Value& v) {
  if (v.type == LEON_UNSIGNED_CHAR || v.type == LEON_CHAR) return parseValueWithSpec(v.toUnsignedChar());
  else if (v.type == LEON_OBJECT) {
    Value ret;
    map<string, Value> vMap = v.toMap();
    for (auto it = vMap.begin(); it != vMap.end(); ++it) {
      ret[it->first] = parseValueWithSpec(it->second);
    }
    return ret;
  } else if (v.type == LEON_ARRAY) {
    vector<Value> vVec = v.toVector();
    Value spec = vVec[0];
    Value ret;
    Value lenVal = parseValue();
    unsigned int len = lenVal.toUnsignedInt();
    for (unsigned i = 0; i < len; ++i) {
      ret.push(parseValueWithSpec(spec));
    }
    return ret;
  }
}
Value Parser::parseValueWithSpec(const unsigned char t) {
  if (t == LEON_STRING) return readString();
  else if (t == LEON_REGEXP) return Value::RegExp(readString());
  else if (t == LEON_BOOLEAN) return parseValue();
  else return parseValue(t);
}
Value Parser::parseValue() {
  return parseValue(buffer.readUInt8());
}
Value Parser::parseValue(const unsigned char t) {
  if (t < LEON_OBJECT) {
    return buffer.readValue(t);
  } else if (t == LEON_ARRAY) {
    Value lenVal = buffer.readValue(buffer.readUInt8());
    unsigned int len = lenVal.toUnsignedInt();
    Value ret;
    for (unsigned i = 0; i < len; ++i) {
      Value el = parseValue();
      ret.push(el);
    }
    return ret;
  } else if (t == LEON_OBJECT) {
    Value indexVal = buffer.readValue(OLItype);
    unsigned int index = indexVal.toUnsignedInt();
    vector<unsigned int> layout = objectLayoutIndex[index];
    Value ret;
    for (unsigned i = 0; i < layout.size(); ++i) {
      Value el = parseValue();
      ret.set(stringIndex[layout[i]], el);
    }
    return ret;
  } else if (t == LEON_STRING) {
    Value indexVal = buffer.readValue(stringIndexType);
    unsigned int index = indexVal.toUnsignedInt();
    return Value(stringIndex[index]);
  } else if (t == LEON_REGEXP) {
    return Value(readString(), LEON_REGEXP);
  } else if (t == LEON_UNDEFINED) {
    return Value();
  } else if (t == LEON_TRUE) {
    return Value(true);
  } else if (t == LEON_FALSE) {
    return Value(false);
  } else if (t == LEON_NULL) {
    return Value::Null();
  } else if (t == LEON_NAN) {
    return Value::NaN();
  } else if (t == LEON_POSITIVE_INFINITY) {
    return Value::Infinity();
  } else if (t == LEON_MINUS_INFINITY) {
    return Value::MinusInfinity();
  } else if (t == LEON_DATE) {
    Value ret = buffer.readValue(LEON_UNSIGNED_INT);
    ret.type = LEON_DATE;
    return ret;
  } else if (t == LEON_REGEXP) {
    return Value::RegExp(readString(), readString());
  } else if (t == LEON_BUFFER) {
    Value lenVal = buffer.readValue(buffer.readUInt8());
    unsigned int len = lenVal.toUnsignedInt();
    string ret;
    for (unsigned i = 0; i < len; ++i) {
      ret += buffer.readUInt8();
    }
    return Value::Buffer(ret.c_str());
  } else {
    throw TypeException();
  }
}
