
#include "buffer-iterator.h"
#include "value.h"

BufferIterator::BufferIterator(const StringBuffer& v) : buffer(v), i(0) {}
unsigned char BufferIterator::readUInt8() {
  ++i;
  return buffer.readUInt8(i - 1);
}
char BufferIterator::readInt8() {
  ++i;
  return buffer.readUInt8(i - 1);
}
unsigned short BufferIterator::readUInt16() {
  i += 2;
  return buffer.readUInt16LE(i - 2);
}
short BufferIterator::readInt16() {
  i += 2;
  return buffer.readInt16LE(i - 2);
}
unsigned int BufferIterator::readUInt32() {
  i += 4;
  return buffer.readUInt32LE(i - 4);
}
int BufferIterator::readInt32() {
  i += 4;
  return buffer.readInt32LE(i - 4);
}
float BufferIterator::readFloat() {
  i += 4;
  return buffer.readFloatLE(i - 4);
}
double BufferIterator::readDouble() {
  i += 8;
  return buffer.readDoubleLE(i - 8);
}
LEON::Value BufferIterator::readValue(const unsigned char type) {
  switch (type) {
    case LEON_UNSIGNED_CHAR:
      return LEON::Value(readUInt8());
    case LEON_CHAR:
      return LEON::Value(readInt8());
    case LEON_UNSIGNED_SHORT:
      return LEON::Value(readUInt16());
    case LEON_SHORT:
      return LEON::Value(readInt16());
    case LEON_UNSIGNED_INT:
      return LEON::Value(readUInt32());
    case LEON_INT:
      return LEON::Value(readInt32());
    case LEON_FLOAT:
      return LEON::Value(readFloat());
    case LEON_DOUBLE:
      return LEON::Value(readDouble());
  }
}
