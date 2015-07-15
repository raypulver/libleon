#include "types.h"

unsigned char numeric_type_check(const long v) {
  if (v < 0) {
    if (static_cast<long>(static_cast<char>(v)) == v) return LEON_CHAR;
    if (static_cast<long>(static_cast<short>(v)) == v) return LEON_SHORT;
    if (static_cast<long>(static_cast<int>(v)) == v) return LEON_INT;
    return LEON_DOUBLE;
  }
  if (static_cast<long>(static_cast<unsigned char>(v)) == v) return LEON_UNSIGNED_CHAR;
  if (static_cast<long>(static_cast<unsigned short>(v)) == v) return LEON_UNSIGNED_SHORT;
  if (static_cast<long>(static_cast<unsigned int>(v)) == v) return LEON_UNSIGNED_INT;
  return LEON_DOUBLE;
}
