#ifndef PARSER_H
#define PARSER_H

#include <string>
#include <vector>
#include "buffer-iterator.h"
#include "string-buffer.h"
#include "value.h"

class Parser {
public:
  Parser(const std::string);
  Parser(const std::string, const LEON::Value&);
  BufferIterator buffer;
  unsigned char state;
  std::vector<std::string> stringIndex;
  std::vector<std::vector<unsigned int>> objectLayoutIndex;
  unsigned char stringIndexType;
  unsigned char OLItype;
  LEON::Value spec;
  bool hasSpec;
  std::string readString();
  Parser* parseSI();
  Parser* parseOLI();
  LEON::Value parseValueWithSpec();
  LEON::Value parseValueWithSpec(LEON::Value&);
  LEON::Value parseValueWithSpec(const unsigned char);
  LEON::Value parseValue();
  LEON::Value parseValue(const unsigned char);
};

#endif
