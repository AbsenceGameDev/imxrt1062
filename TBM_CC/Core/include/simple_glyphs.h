#ifndef SIMPLE_GLYPHS_H
#define SIMPLE_GLYPHS_H

#include <stdint.h>

struct fourfour {
  uint8_t r0 = 0b00000000;
  uint8_t r1 = 0b00000000;
};

struct eighteight {
  uint8_t r0 = 0b00000000;
  uint8_t r1 = 0b00000000;
  uint8_t r2 = 0b00000000;
  uint8_t r3 = 0b00000000;
  uint8_t r4 = 0b00000000;
  uint8_t r5 = 0b00000000;
  uint8_t r6 = 0b00000000;
  uint8_t r7 = 0b00000000;
};

typedef struct fourfour   _4x4_;
typedef struct eighteight _8x8_;

#endif // SIMPLE_GLYPHS_H