#include <string>
#include <exception>
#include "exceptions.h"

const char* BufferRangeException::what() const throw() {
  return "Tried to read past end of buffer.";
}

const char* RangeException::what() const throw() {
  return "Tried to read past end of buffer.";
}

const char* RepeatException::what() const throw() {
  return "Tried to parse string index twice.";
}
const char* ShouldBeUnsignedException::what() const throw() {
  return "Expected an unsigned numeric value while reading array length.";
}
const char* TypeException::what() const throw() {
  return "Tried to read a value with unknown type.";
}
const char* StringException::what() const throw() {
  return "Value is not a string.";
}
const char* UnsignedCharException::what() const throw() {
  return "Value is not an unsigned char.";
}
const char* CharException::what() const throw() {
  return "Value is not a char.";
}
const char* UnsignedShortException::what() const throw() {
  return "Value is not an unsigned short.";
}
const char* ShortException::what() const throw() {
  return "Value is not a short.";
}
const char* UnsignedIntException::what() const throw() {
  return "Value is not an unsigned int.";
}
const char* IntException::what() const throw() {
  return "Value is not an int.";
}
const char* FloatException::what() const throw() {
  return "Value is not a float.";
}
const char* DoubleException::what() const throw() {
  return "Value is not a double.";
}
const char* BoolException::what() const throw() {
  return "Value is not a bool.";
}
const char* ObjectException::what() const throw() {
  return "Value is not an object.";
}
const char* ArrayException::what() const throw() {
  return "Value is not an array.";
}
const char* ObjectArrayException::what() const throw() {
  return "Value is not an array or object.";
}
const char* NumericException::what() const throw() {
  return "Value is not numeric.";
}
const char* RegExpException::what() const throw() {
  return "Value is not a RegExp.";
}
const char* DateException::what() const throw() {
  return "Value is not a Date.";
}
const char* BufferException::what() const throw() {
  return "Value is not a Buffer.";
}
const char* InfinityException::what() const throw() {
  return "Value is not Infinity.";
}
const char* NullException::what() const throw() {
  return "Value is not null.";
}
const char* UndefinedException::what() const throw() {
  return "Value is not undefined.";
}
const char* NaNException::what() const throw() {
  return "Value is not NaN.";
}
