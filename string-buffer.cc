#include <string>
#include <cstring>
#include <iostream>
#include "string-buffer.h"
#include "exceptions.h"

using namespace std;

static inline void Swizzle(unsigned char* start, unsigned int len) {
  unsigned char* end = start + len - 1;
  while (start < end) {
    char tmp = *start;
    *start++ = *end;
    *end-- = tmp;
  }
}

static inline bool isLE() {
  const union {
    uint8_t u8[2];
    uint16_t u16;
  } u = {
    { 1, 0 }
  };
  return u.u16 == 1;
}

StringBuffer::StringBuffer (string buf) : buffer(buf) {}
StringBuffer::StringBuffer () : StringBuffer("") {}
StringBuffer::StringBuffer (const StringBuffer& sb) : buffer(sb.buffer) {}
StringBuffer::~StringBuffer () {}

void StringBuffer::concat(const StringBuffer& sb) {
  buffer += sb.buffer;
}

StringBuffer StringBuffer::fromString(const std::string str) {
  StringBuffer ret;
  for (char c : str) {
    ret.writeUInt8(c, -1);
  }
  ret.writeUInt8(0, -1);
  return ret;
}

unsigned int StringBuffer::writeUInt8(const unsigned char v, const long i) {
  if (i >= buffer.size() || i == -1) {
    buffer += v;
  } else {
    buffer[i] = v;
  }
  return i + 1;
}
unsigned int StringBuffer::writeInt8(const char v, const long i) {
  const unsigned char val = static_cast<unsigned char>(v);
  if (i >= buffer.size() || i == -1) {
    buffer += val;
  } else {
    buffer[i] = val;
  }
  return i + 1;
}
unsigned int StringBuffer::writeUInt16LE(const unsigned short v, const long i) {
  union NoAlias {
    unsigned char val[2];
    unsigned short num;
  };
  union NoAlias na;
  na.num = v;
  unsigned char val[2];
  if (!isLE()) Swizzle(na.val, 2);
  memcpy(val, na.val, 2);
  if (i >= buffer.size() || i == -1) {
    buffer += val[0];
    buffer += val[1];
  } else {
    buffer[i] = val[0];
    buffer[i + 1] = val[1];
  }
  return i + 2;
}
unsigned int StringBuffer::writeInt16LE(const short v, const long i) {
  const unsigned short val = static_cast<unsigned short>(v);
  return writeUInt16LE(val, i);
}
unsigned int StringBuffer::writeUInt32LE(const unsigned int v, const long i) {
  union NoAlias {
    unsigned char bytes[4];
    unsigned int val;
  };
  union NoAlias na;
  na.val = v;
  unsigned char val[4];
  if (!isLE()) Swizzle(na.bytes, 4);
  memcpy(val, na.bytes, 4);
  if (i >= buffer.size() || i == -1) {
    for (unsigned j = 0; j < 4; ++j) {
      buffer += val[j];
    }
  } else {
    for (unsigned j = 0; j < 4; ++j) {
      buffer[i + j] = val[j];
    }
  }
  return i + 4;
}
unsigned int StringBuffer::writeInt32LE(const int v, const long i) {
  const unsigned int val = static_cast<unsigned int>(v);
  return writeUInt32LE(val, i);
}
unsigned int StringBuffer::writeFloatLE(const float v, const long i) {
  union NoAlias {
    float f;
    unsigned int bytes;
    unsigned char bytez[4];
  };
  union NoAlias na = { v };
  unsigned int val;
  if (!isLE()) Swizzle(na.bytez, 4);
  memcpy(&val, &na.bytes, 4); 
  return writeUInt32LE(val, i);
}
unsigned int StringBuffer::writeDoubleLE(const double v, const long i) {
  union NoAlias {
    unsigned long val;
    unsigned char bytes[8];
    double source;
  };
  union NoAlias na;
  na.source = v;
  unsigned long num;
  if (!isLE()) Swizzle(na.bytes, 8);
  memcpy(&num, &na.val, 8);
  writeUInt32LE(num & 0xFFFFFFFF, i);
  writeUInt32LE(num >> 32, i + 4);
  return i + 8;
}
unsigned char StringBuffer::readUInt8(const long i) {
  if (i >= buffer.size()) throw RangeException();
  return buffer[i];
}
char StringBuffer::readInt8(const long i) {
  return static_cast<char>(buffer[i]);
}
unsigned short StringBuffer::readUInt16LE(const long i) {
  if (i >= buffer.size() - 1) throw RangeException();
  union NoAlias {
    unsigned char bytes[2];
    unsigned short val;
  } na;
  na.bytes[0] = buffer[i];
  na.bytes[1] = buffer[i + 1];
  if (!isLE()) Swizzle(na.bytes, 2);
  return na.val;
}
short StringBuffer::readInt16LE(const long i) {
  return static_cast<short>(readUInt16LE(i));
}
unsigned int StringBuffer::readUInt32LE(const long i) {
  if (i >= buffer.size() - 3) throw RangeException();
  union NoAlias {
    unsigned char bytes[4];
    unsigned int val;
  } na;
  for (unsigned j = 0; j < 4; ++j) {
    na.bytes[j] = buffer[i + j];
  }
  if (!isLE()) Swizzle(na.bytes, 4);
  return na.val;
}
int StringBuffer::readInt32LE(const long i) {
  return static_cast<int>(readUInt32LE(i));
}
float StringBuffer::readFloatLE(const long i) {
  if (i >= buffer.size() - 3) throw RangeException();
  union NoAlias {
    float val;
    unsigned char bytes[4];
  } na;
  memcpy(na.bytes, buffer.substr(i, 4).c_str(), 4);
  if (!isLE()) Swizzle(na.bytes, 4);
  return na.val;
}
double StringBuffer::readDoubleLE(const long i) {
  if (i >= buffer.size() - 7) throw RangeException();
  unsigned long num = static_cast<unsigned long>(readUInt32LE(i)) | (static_cast<unsigned long>(readUInt32LE(i + 4)) << 32);
  union NoAlias {
    unsigned long val;
    unsigned char bytes[8];
    double ret;
  };
  union NoAlias na;
  na.val = num;
  double ret;
  if (!isLE()) Swizzle(na.bytes, 8);
  memcpy(&ret, &na.ret, 8);
  return ret;
}
