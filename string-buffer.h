#ifndef STRINGBUFFER_H
#define STRINGBUFFER_H

#include <string>
#include "value.h"

class StringBuffer {
public:
  std::string buffer;
  StringBuffer(std::string);
  StringBuffer();
  StringBuffer(const StringBuffer&);
  ~StringBuffer();
  static StringBuffer fromString(const std::string);
  void concat(const StringBuffer&);
  unsigned int writeUInt8(const unsigned char, const long);
  unsigned int writeInt8(const char, const long);
  unsigned int writeUInt16LE(const unsigned short, const long);
  unsigned int writeInt16LE(const short, const long);
  unsigned int writeUInt32LE(const unsigned int, const long);
  unsigned int writeInt32LE(const int, const long);
  unsigned int writeFloatLE(const float, const long);
  unsigned int writeDoubleLE(const double, const long);
  unsigned char readUInt8(const long);
  char readInt8(const long);
  unsigned short readUInt16LE(const long);
  short readInt16LE(const long);
  unsigned int readUInt32LE(const long);
  int readInt32LE(const long);
  float readFloatLE(const long);
  double readDoubleLE(const long);
};

#endif
