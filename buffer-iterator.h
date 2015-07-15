#ifndef BUFFER_ITERATOR_H
#define BUFFER_ITERATOR_H

#include "string-buffer.h"
#include "value.h"
#include "types.h"

class BufferIterator {
public:
  unsigned long i;
  StringBuffer buffer;
  BufferIterator(const StringBuffer&);
  unsigned char readUInt8();
  char readInt8();
  unsigned short readUInt16();
  short readInt16();
  unsigned int readUInt32();
  int readInt32();
  float readFloat();
  double readDouble();
  LEON::Value readValue(const unsigned char);
};

#endif
