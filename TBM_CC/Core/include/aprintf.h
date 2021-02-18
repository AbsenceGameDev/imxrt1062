#ifndef APRINTF_H
#define APRINTF_H

#include "simple_glyphs.h"

void *
__get_glyph_block_4x4__(char * instream);

int
aprintf(char * instring);

#endif // APRINTF_H