#include <string>
#include <exception>
#include <algorithm>
#include "string-buffer.h"
#include "value.h"
#include "types.h"
#include "encoder.h"
#include "type-check.h"
#include "exceptions.h"

using namespace std;
using namespace LEON;

unsigned char bool_check(Value& v) {
  if (v.type != LEON_TRUE && v.type != LEON_FALSE) throw BoolException();
  if (v.toBoolean()) return LEON_TRUE;
  return LEON_FALSE;
}
template <typename T> void set_push(vector<T>& arr, T val) {
  auto it = find(arr.begin(), arr.end(), val);
  if (it == arr.end()) arr.push_back(val);
}
vector<string> gather_strings(Value* v, vector<string>& ret, Value* branch) {
  if (branch->type == LEON_OBJECT) {
    map<string, Value> vMap = branch->toMap();
    for (auto it = vMap.begin(); it != vMap.end(); ++it) {
      set_push<string>(ret, it->first);
    }
    for (auto it = vMap.begin(); it != vMap.end(); ++it) {
      gather_strings(v, ret, &(it->second));
    }
  } else if (branch->type == LEON_ARRAY) {
    vector<Value> vArr = branch->toVector();
    for (unsigned i = 0; i < vArr.size(); ++i) {
      gather_strings(v, ret, &(vArr[i]));
    }
  } else if (branch->type == LEON_STRING) {
    set_push<string>(ret, branch->toString());
  }
  return ret;
}
vector<string> gather_strings(Value* v) {
  vector<string> ret;
  return gather_strings(v, ret, v);
}
vector<vector<unsigned int>> gather_layouts(Value* v, vector<string>& stringIndex, vector<vector<unsigned int>>& ret, Value* branch) {
  if (branch->type == LEON_OBJECT) {
    map<string, Value> vMap = branch->toMap();
    ret.push_back(vector<unsigned int>());
    for (auto it = vMap.begin(); it != vMap.end(); ++it) {
      auto ptr = find(stringIndex.begin(), stringIndex.end(), it->first);
      ret[ret.size() - 1].push_back(distance(stringIndex.begin(), ptr));
    }
    for (auto it = vMap.begin(); it != vMap.end(); ++it) {
      gather_layouts(v, stringIndex, ret, &(it->second));
    }
  } else if (branch->type == LEON_ARRAY) {
    for (unsigned i = 0; i < branch->size(); ++i) {
      gather_layouts(v, stringIndex, ret, &(branch->operator[](i)));
    }
  }
  return ret;
}
unsigned int match_layout (Value& v, const vector<string>& stringIndex, const vector<vector<unsigned int>>& OLI) {
  vector<unsigned int> layout;
  map<string, Value> vMap = v.toMap();
  for (auto it = vMap.begin(); it != vMap.end(); ++it) {
    auto ptr = find(stringIndex.begin(), stringIndex.end(), it->first);
    layout.push_back(distance(stringIndex.begin(), ptr));
  }
  for (unsigned i = 0; i < OLI.size(); ++i) {
    if (is_permutation(OLI[i].begin(), OLI[i].end(), layout.begin())) return i;
  }
  return 0;
}
vector<vector<unsigned int>> gather_layouts(Value* v, vector<string>& stringIndex) {
  vector<vector<unsigned int>> ret;
  return gather_layouts(v, stringIndex, ret, v);
}

Encoder::Encoder(Value& obj, Value& s) : buffer(StringBuffer()), payload(obj), spec(s), hasSpec(true) {}
Encoder::Encoder(Value& obj) : buffer(StringBuffer()), payload(obj), hasSpec(false) {}
void Encoder::append(const StringBuffer& sb) { buffer.concat(sb); }
Encoder* Encoder::writeData() {
  if (hasSpec) writeValueWithSpec(payload);
  else writeValue(payload);
  return this;
}
string Encoder::getBuffer() {
  return buffer.buffer;
}
void Encoder::writeValueWithSpec(Value& v) {
  writeValueWithSpec(v, spec);
}
void Encoder::writeValueWithSpec(Value& v, unsigned char t) {
  if (v.type != t) {
    switch (t) {
      case LEON_UNSIGNED_CHAR:
        throw UnsignedCharException();
      case LEON_CHAR:
        throw CharException();
      case LEON_UNSIGNED_SHORT:
        throw UnsignedShortException();
      case LEON_SHORT:
        throw ShortException();
      case LEON_UNSIGNED_INT:
        throw UnsignedIntException();
      case LEON_INT:
        throw IntException();
      case LEON_FLOAT:
        throw FloatException();
      case LEON_DOUBLE:
        throw DoubleException();
      case LEON_TRUE:
      case LEON_FALSE:
        throw BoolException();
      case LEON_STRING:
        throw StringException();
      case LEON_REGEXP:
        throw RegExpException();
      case LEON_DATE:
        throw DateException();
      case LEON_BUFFER:
        throw BufferException();
    }
  }
  writeValue(v, t, true);
}
void Encoder::writeValueWithSpec(Value& v, Value& s) {
  if (s.type == LEON_UNSIGNED_CHAR || s.type == LEON_CHAR) {
    return writeValueWithSpec(v, s.toUnsignedChar());
  }
  if (s.type == LEON_OBJECT) {
    if (v.type != LEON_OBJECT) throw ObjectException();
    map<string, Value> sMap = s.toMap();
    for (auto it = sMap.begin(); it != sMap.end(); ++it) {
      writeValueWithSpec(v[it->first], it->second);
    }
    return;
  }
  if (s.type == LEON_ARRAY) {
    if (v.type != LEON_ARRAY) throw ArrayException();
    Value len (v.size());
    writeValue(len, numeric_type_check(v.size()));
    for (unsigned i = 0; i < v.size(); ++i) {
      writeValueWithSpec(v[i], s[0]);
    }
    return;
  }
  if (s.type == LEON_BOOLEAN) {
    if (bool_check(v)) v.type = LEON_TRUE;
    else v.type = LEON_FALSE;
    writeValue(v, true);
  }
  writeValue(v, true);
}
unsigned int Encoder::writeValue(Value& v) {
  return writeValue(v, false);
}
unsigned int Encoder::writeValue(Value& v, unsigned char type) {
  return writeValue(v, type, false);
}
unsigned int Encoder::writeValue(Value& v, bool implicit) {
  return writeValue(v, v.type, implicit);
}
unsigned int Encoder::writeValue(Value& v, unsigned char type, bool implicit) {
  StringBuffer typeByte;
  if (type == LEON_BOOLEAN) {
    if (bool_check(v)) type = LEON_TRUE;
    else type = LEON_FALSE;
  }
  typeByte.writeUInt8(type, 0);
  if (type == LEON_UNDEFINED || type == LEON_TRUE || type == LEON_FALSE || type == LEON_NULL || type == LEON_NAN) {
    append(typeByte);
    return 1;
  }
  unsigned int count = 0;
  if (!implicit) {
    append(typeByte);
    ++count;
  }
  if (type == LEON_STRING) {
    string str = v.toString();
    if (stringIndex.size() == 0) {
      writeString(str);
      return count + 1 + str.size();
    }
    auto it = find(stringIndex.begin(), stringIndex.end(), str);
    Value idx (distance(stringIndex.begin(), it));
    writeValue(idx, stringIndexType, true);
    return count + 1;
  }
  if (type == LEON_CHAR) {
    StringBuffer bytes;
    bytes.writeInt8(v.toChar(), 0);
    append(bytes);
    return count + 1;
  }
  if (type == LEON_UNSIGNED_CHAR) {
    StringBuffer bytes;
    bytes.writeUInt8(v.toUnsignedChar(), 0);
    append(bytes);
    return count + 1;
  }
  if (type == LEON_SHORT) {
    StringBuffer bytes;
    bytes.writeInt16LE(v.toShort(), 0);
    append(bytes);
    return count + 2;
  }
  if (type == LEON_UNSIGNED_SHORT) {
    StringBuffer bytes;
    bytes.writeUInt16LE(v.toUnsignedShort(), 0);
    append(bytes);
    return count + 2;
  }
  if (type == LEON_INT) {
    StringBuffer bytes;
    bytes.writeInt32LE(v.toInt(), 0);
    append(bytes);
    return count + 4;
  }
  if (type == LEON_UNSIGNED_INT) {
    StringBuffer bytes;
    bytes.writeUInt32LE(v.toUnsignedInt(), 0);
    append(bytes);
    return count + 4;
  }
  if (type == LEON_FLOAT) {
    StringBuffer bytes;
    bytes.writeFloatLE(v.toFloat(), 0);
    append(bytes);
    return count + 4;
  }
  if (type == LEON_DOUBLE) {
    StringBuffer bytes;
    bytes.writeDoubleLE(v.toDouble(), 0);
    append(bytes);
    return count + 8;
  }
  if (type == LEON_ARRAY) {
    Value size (v.size());
    writeValue(size, numeric_type_check(v.size()));
    for (unsigned i = 0; i < v.size(); ++i) {
      writeValue(v[i]);
    }
    return count + v.size();
  }
  if (type == LEON_OBJECT) {
    unsigned int index = match_layout(v, stringIndex, OLI);
    Value indexVal(index);
    if (!implicit) writeValue(indexVal, OLItype, true);
    for (unsigned i = 0; i < OLI[index].size(); ++i) {
      Value strIndex (v[stringIndex[OLI[index][i]]]);
      writeValue(strIndex);
    }
    return count;
  }
  if (type == LEON_REGEXP) {
    string reg = v.toString();
    writeString(reg);
    return count + reg.size();
  }
  if (type == LEON_DATE) {
    v.type = LEON_UNSIGNED_INT;
    writeValue(v, true);
    v.type = LEON_DATE;
    return count + 4;
  }
  if (type == LEON_BUFFER) {
    string buf = v.toString();
    Value len (buf.size());
    unsigned int lenSize = writeValue(len);
    writeString(buf, false, false);
    return count + lenSize + buf.size();
  }
}
void Encoder::writeString(string str, bool inIndex, bool term) {
  StringBuffer add;
  for (char c : str) {
    add.writeUInt8(c, -1);
  }
  if (term) add.writeUInt8(0, -1);
  append(add);
}
void Encoder::writeString(string str) {
  writeString(str, false, true);
}
void Encoder::writeString(string str, bool inIndex) {
  if (!inIndex) writeString(str);
  else {
    Value idx = Value(distance(stringIndex.begin(), find(stringIndex.begin(), stringIndex.end(), str)));
    writeValue(idx, stringIndexType, true);
  }
}
Encoder* Encoder::writeOLI () {
  if (stringIndex.size() == 0) return this;
  OLI = gather_layouts(&payload, stringIndex);
  if (OLI.size() == 0) {
    Value empty (LEON_EMPTY);
    writeValue(empty, LEON_CHAR, true);
    return this;
  }
  OLItype = numeric_type_check(OLI.size());
  Value sizeVal (OLI.size());
  writeValue(sizeVal, OLItype);
  for (unsigned i = 0; i < OLI.size(); ++i) {
    unsigned char type = numeric_type_check(OLI[i].size());
    Value sizeVal (OLI[i].size());
    writeValue(sizeVal, type);
    for (unsigned j = 0; j < OLI[i].size(); ++j) {
      Value strIndexVal (OLI[i][j]);
      writeValue(strIndexVal, stringIndexType, true);
    }
  }
  return this;
}
Encoder* Encoder::writeSI () {
  stringIndex = gather_strings(&payload);
  if (stringIndex.size() == 0) {
    Value empty (LEON_EMPTY);
    writeValue(empty, LEON_CHAR, true);
    return this;
  }
  stringIndexType = numeric_type_check(stringIndex.size());
  Value strIndexSize (stringIndex.size());
  writeValue(strIndexSize, stringIndexType);
  for (unsigned i = 0; i < stringIndex.size(); ++i) {
    writeString(stringIndex[i]);
  }
  return this;
}
