#ifndef ENCODER_H
#define ENCODER_H

#include <vector>
#include "value.h"
#include "string-buffer.h"

class Encoder {
public:
  LEON::Value payload;
  StringBuffer buffer;
  LEON::Value spec;
  bool hasSpec;
  std::vector<std::string> stringIndex;
  unsigned char stringIndexType;
  std::vector<std::vector<unsigned int>> OLI;
  unsigned char OLItype;
  Encoder(LEON::Value&);
  Encoder(LEON::Value&, LEON::Value&);
  void append(const StringBuffer&);
  Encoder* writeData();
  void writeValueWithSpec(LEON::Value&, LEON::Value&);
  void writeValueWithSpec(LEON::Value&, unsigned char);
  void writeValueWithSpec(LEON::Value&);
  unsigned int writeValue(LEON::Value&, const unsigned char, const bool);
  unsigned int writeValue(LEON::Value&, const unsigned char);
  unsigned int writeValue(LEON::Value&);
  unsigned int writeValue(LEON::Value&, const bool);
  void writeString(std::string);
  void writeString(std::string, bool);
  void writeString(std::string, bool, bool);
  Encoder* writeOLI();
  Encoder* writeSI();
  std::string getBuffer();
};

#endif
