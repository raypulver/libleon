#ifndef LEON_H
#define LEON_H

#include <string>
#include "value.h"
#include "types.h"

namespace LEON {
  std::string stringify (Value);
  Value parse (std::string);
  class Channel {
    Value spec;
  public:
    Channel(Value);
    std::string stringify (Value);
    Value parse (std::string);
  };
}

#endif
