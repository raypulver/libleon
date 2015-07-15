#include <string>
#include "value.h"
#include "string-buffer.h"
#include "parser.h"
#include "encoder.h"
#include "leon-s.h"

using namespace std;

string LEON::stringify(LEON::Value v) {
  Encoder enc(v);
  return enc.writeSI()->writeOLI()->writeData()->getBuffer();
}

LEON::Value LEON::parse(std::string v) {
  Parser parser(v);
  return parser.parseSI()->parseOLI()->parseValue();
}

LEON::Channel::Channel(LEON::Value s) : spec(s) {}

string LEON::Channel::stringify(LEON::Value v) {
  Encoder enc(v, spec);
  return enc.writeData()->getBuffer();
}

LEON::Value LEON::Channel::parse(string v) {
  Parser parser(v, spec);
  return parser.parseValueWithSpec();
}
