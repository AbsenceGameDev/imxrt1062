#include "simple_glyphs.h"

/**
 * @brief Reference graphs below for 4x4 characters.
 * They are meant to to be used on small 64x16 (or 128x32) px screens.
 * I opted to create these 4x4 glyphs to go from an initial max of 16 (or 32)
 * characters on the screen to max 64 (or 128) at any given time compared to
 * standard 8x8 glyphs.
 * At minumum I need:
 * 1. All numerical character
 * 2. Some alphabetical characters, which alphabetical characters will be needed
 *    is of no consequence, as long as I can have enough to represent atleast 5
 *    different variables with single characters.
 * 3. ;
 * 4. []
 * 5. -> <- .
 * 6. + - / % =
 * 7. ^ | &
 **/

// Numerical 4x4 glyphs
/** 0 4x4
 *
 * .00.
 * 00.0
 * 0.00
 * .00.
 */

_4x4_ _0_4x4 = {.r0 = 0b01101101, .r1 = 0b10110110};

/** 1 4x4
 *
 * 000.
 * .00.
 * .00.
 * 0000
 */
_4x4_ _1_4x4 = {.r0 = 0b11100110, .r1 = 0b01101111};

/** 2 4x4
 *
 * .00.
 * 0..0
 * ..0.
 * 0000
 */
_4x4_ _2_4x4 = {.r0 = 0b01101001, .r1 = 0b00101111};

/** 3 4x4
 *
 * 0000
 * .000
 * ...0
 * 0000
 */
_4x4_ _3_4x4 = {.r0 = 0b11110111, .r1 = 0b00011111};

/** 4 4x4
 *
 * 0..0
 * 0..0
 * 0000
 * ...0
 */
_4x4_ _4_4x4 = {.r0 = 0b10011001, .r1 = 0b11110001};

/** 5 4x4
 *
 * 0000
 * 00..
 * -.00
 * 0000
 */
_4x4_ _5_4x4 = {.r0 = 0b11111100, .r1 = 0b00111111};

/** 6 4x4
 *
 * 0000
 * 00..
 * 0.00
 * 0000
 */
_4x4_ _6_4x4 = {.r0 = 0b11111100, .r1 = 0b10111111};

/** 7 4x4
 *
 * 0000
 * ...0
 * ...0
 * ...0
 */
_4x4_ _7_4x4 = {.r0 = 0b11110001, .r1 = 0b00010001};

/** 8 4x4
 *
 * 000.
 * 0.0.
 * .0.0
 * .000
 */
_4x4_ _8_4x4 = {.r0 = 0b11101010, .r1 = 0b01010111};

/** 9 4x4
 *
 * 0000
 * 0..0
 * 0000
 * ...0
 */
_4x4_ _9_4x4 = {.r0 = 0b11111001, .r1 = 0b11110001};

// Alphabetical 4x4 glyphs
/** A 4x4
 * ..0.
 * 0..0
 * 0000
 * 0..0
 */
_4x4_ a_4x4 = {.r0 = 0b00000000, .r1 = 0b00000000};

/** B 4x4
 * 0...
 * 0000
 * 0..0
 * 0000
 */

/** c 4x4
 * 0...
 * 0000
 * 0..0
 * 0000
 */

// 8x8 glyphs