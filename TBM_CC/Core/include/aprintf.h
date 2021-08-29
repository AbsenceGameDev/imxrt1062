/**
 * @file      aprintf.h
 * @author    Ario@Permadev
 * @brief
 * @version   0.1
 * @date      2021-08-29
 *
 * @copyright Copyright (c) 2021, MIT-License included in project toplevel dir
 *
 */

#ifndef APRINTF_H
#define APRINTF_H

#include "simple_glyphs.h"

void *
__get_glyph_block_4x4__(char * instream);

void *
__get_glyph_block_8x8__(char * instream);

int
aprintf(char * instring);

#endif // APRINTF_H