/**
 * @authors   Ario Amin @ Permadev, 
 * @copyright Copyright (c) 2021-2024, MIT-License included in project toplevel dir
 */

#include "io/aprintf.h"

/**
 * @brief Generate a 4x4 glyph block based on read data.
 **/

void *
__get_glyph_block_4x4__(char * instream)
{
  char * base_ptr = &(*instream);
  while ((*instream != '\0') || ((instream - base_ptr) < MSG_MAX_LENGTH)) {

    const _4x4_ * current_c = get_glyph_4x4(instream);

    ++instream;
  }

  return ((void *)0); // @todo stream data to file, remove this dummy return
}

/**
 * @brief Generate a 4x4 glyph block based on read data.
 **/

void *
__get_glyph_block_7x6__(char * instream)
{
  char * base_ptr = &(*instream);
  while ((*instream != '\0') || ((instream - base_ptr) < MSG_MAX_LENGTH)) {

    const _7x6_ * current_c = get_glyph_7x6(instream);

    ++instream;
  }

  return ((void *)0); // @todo stream data to file, remove this dummy return
}

/**
 * @brief Generate a 8x8 glyph block based on read data.
 **/

void *
__get_glyph_block_8x8__(char * instream)
{
  char * base_ptr = &(*instream);
  while ((*instream != '\0') || ((instream - base_ptr) < MSG_MAX_LENGTH)) {

    const _8x8_ * current_c = get_glyph_8x8(instream);

    ++instream;
  }

  return ((void *)0); // @todo stream data to file, remove this dummy return
}

/**
 * @brief Print function to print text to screen.
 * NOTE: It will first be written only to a minioled i've ordered, will extend
 *       it to be used for more screen when I've learned more on how to output
 *       to screens, new territory for me.
 **/
int
aprintf(char * instring);