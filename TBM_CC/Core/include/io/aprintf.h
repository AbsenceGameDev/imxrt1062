/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2026, MIT-License included in project toplevel dir
 */

#ifndef APRINTF_H
#define APRINTF_H

#include "simple_glyphs.h"

/** 
 * @brief Gets a 4x4 pixel block for a glyph 
 * @param instream Character we want a glyph for
 */ 
void*
__get_glyph_block_4x4__(char * instream);

/** 
 * @brief Gets a 7x6 pixel block for a glyph 
 * @param instream Character we want a glyph for
 */ 
void*
__get_glyph_block_7x6__(char * instream);

/** 
 * @brief Gets a 8x8 pixel block for a glyph 
 * @param instream Character we want a glyph for
 */ 
void*
__get_glyph_block_8x8__(char * instream);

int
aprintf(char * instring);

#endif // APRINTF_H