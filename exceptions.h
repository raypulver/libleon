#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>

class BufferRangeException : public std::exception {
  virtual const char* what() const throw(); 
};

class RangeException : public std::exception {
  virtual const char* what() const throw();
};

class RepeatException : public std::exception {
  virtual const char* what() const throw();
};

class TypeException : public std::exception {
  virtual const char* what() const throw();
};

class ShouldBeUnsignedException : public TypeException {
  virtual const char* what() const throw();
};

class StringException : public TypeException {
  virtual const char* what() const throw();
};

class UnsignedCharException : public TypeException {
  virtual const char* what() const throw();
};

class CharException : public TypeException {
  virtual const char* what() const throw();
};

class UnsignedShortException : public TypeException {
  virtual const char* what() const throw();
};

class ShortException : public TypeException {
  virtual const char* what() const throw();
};

class UnsignedIntException : public TypeException {
  virtual const char* what() const throw();
};

class IntException : public TypeException {
  virtual const char* what() const throw();
};

class FloatException : public TypeException {
  virtual const char* what() const throw();
};

class DoubleException : public TypeException {
  virtual const char* what() const throw();
};

class BoolException : public TypeException {
  virtual const char* what() const throw();
};

class ObjectException : public TypeException {
  virtual const char* what() const throw();
};

class ArrayException : public TypeException {
  virtual const char* what() const throw();
};

class ObjectArrayException : public TypeException {
  virtual const char* what() const throw();
};

class NumericException : public TypeException {
  virtual const char* what() const throw();
};

class RegExpException : public TypeException {
  virtual const char* what() const throw();
};

class DateException : public TypeException {
  virtual const char* what() const throw();
};

class BufferException : public TypeException {
  virtual const char* what() const throw();
};

#endif
