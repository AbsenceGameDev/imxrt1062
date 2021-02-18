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
 * 4. [] :
 * 5. -> <- _ .
 * 6. + - / % =
 * 7. ^ | & @
 **/

// Special 4x4 glyphs

/**
 * @brief = 4x4
 *
 * ....
 * 0000
 * ....
 * 0000
 */
_4x4_ _eq_4x4 = {.r0 = 0b00001111, .r1 = 0b00001111};

/**
 * @brief + 4x4
 *
 * ....
 * .0..
 * 000.
 * .0..
 */
_4x4_ _add_4x4 = {.r0 = 0b00000100, .r1 = 0b11100100};

/**
 * @brief - 4x4
 *
 * ....
 * ....
 * 000.
 * ....
 */
_4x4_ _sub_4x4 = {.r0 = 0b00000000, .r1 = 0b11100000};

/**
 * @brief * 4x4
 *
 * ....
 * .00.
 * .00.
 * ....
 */
_4x4_ _mul_4x4 = {.r0 = 0b00000110, .r1 = 0b01100000};

/**
 * @brief * 4x4
 *
 * ...0
 * ..0.
 * .0..
 * 0...
 */
_4x4_ _div_4x4 = {.r0 = 0b00010010, .r1 = 0b01001000};

/**
 * @brief % 4x4
 *
 * 0..0
 * ..0.
 * .0..
 * 0..0
 */
_4x4_ _rem_4x4 = {.r0 = 0b10010010, .r1 = 0b01001001};

/**
 * @brief or | 4x4
 *
 * .0..
 * .0..
 * .0..
 * .0..
 */
_4x4_ _or_4x4 = {.r0 = 0b01000100, .r1 = 0b01000100};

/**
 * @brief not ~ 4x4
 *
 * ....
 * 0.0.
 * .0.0
 * ....
 */
_4x4_ _not_4x4 = {.r0 = 0b00001010, .r1 = 0b01010000};

/**
 * @brief and & 4x4
 *
 * ..0.
 * 0000
 * 0.0.
 * 000.
 */
_4x4_ _and_4x4 = {.r0 = 0b00101111, .r1 = 0b10101110};

/**
 * @brief at @ 4x4
 *
 * 0000
 * 0.00
 * 0...
 * 0000
 */
_4x4_ _at_4x4 = {.r0 = 0b11111011, .r1 = 0b10001111};

/**
 * @brief underscore _ 4x4
 *
 * ....
 * ....
 * ....
 * 0000
 */
_4x4_ _undscr_4x4 = {.r0 = 0b00000000, .r1 = 0b00001111};

/**
 * @brief semicolon ; 4x4
 *
 * ..0.
 * ....
 * ..0.
 * .0..
 */
_4x4_ _semicln_4x4 = {.r0 = 0b00100000, .r1 = 0b00100100};

/**
 * @brief colon : 4x4
 *
 * ..0.
 * ....
 * ..0.
 * ....
 */
_4x4_ _cln_4x4 = {.r0 = 0b00100000, .r1 = 0b00100000};

/**
 * @brief dot . 4x4
 *
 * ....
 * ....
 * ....
 * .0..
 */
_4x4_ _dot_4x4 = {.r0 = 0b00000000, .r1 = 0b00000100};

/**
 * @brief comma , 4x4
 *
 * ....
 * ....
 * ..0.
 * .00.
 */
_4x4_ _comma_4x4 = {.r0 = 0b00000000, .r1 = 0b00100110};

/**
 * @brief left squarebracket [ 4x4
 *
 * .00.
 * .0..
 * .0..
 * .00.
 */
_4x4_ _lsqbrack_4x4 = {.r0 = 0b01100100, .r1 = 0b01000110};

/**
 * @brief right square bracket ] 4x4
 *
 * .00.
 * ..0.
 * ..0.
 * .00.
 */
_4x4_ _rsqbrack_4x4 = {.r0 = 0b01100010, .r1 = 0b00100110};

/**
 * @brief left bracket { 4x4
 *
 * .00.
 * .0..
 * 00..
 * .00.
 */
_4x4_ _lbrack_4x4 = {.r0 = 0b01100100, .r1 = 0b11000110};

/**
 * @brief right bracket } 4x4
 *
 * .00.
 * ..0.
 * ..00
 * .00.
 */
_4x4_ _rbrack_4x4 = {.r0 = 0b01100010, .r1 = 0b00110110};

/**
 * @brief left parenthesis ( 4x4
 *
 * ..0.
 * .0..
 * .0..
 * ..0.
 */
_4x4_ _lprthss_4x4 = {.r0 = 0b00100100, .r1 = 0b01000010};

/**
 * @brief right parenthesis ) 4x4
 *
 * .0..
 * ..0.
 * ..0.
 * .0..
 */
_4x4_ _rprthss_4x4 = {.r0 = 0b01000010, .r1 = 0b00100100};

/**
 * @brief xor ^ 4x4
 *
 * ..0.
 * .0.0
 * ....
 * ....
 */
_4x4_ _xor_4x4 = {.r0 = 0b00100101, .r1 = 0b00000000};

/**
 * @brief leftsh < 4x4
 *
 * ....
 * ..0.
 * .0..
 * ..0.
 */
_4x4_ _leftsh_4x4 = {.r0 = 0b00000010, .r1 = 0b01000010};

/**
 * @brief rightsh > 4x4
 *
 * ....
 * .0..
 * ..0.
 * .0..
 */
_4x4_ _rightsh_4x4 = {.r0 = 0b00000100, .r1 = 0b00100100};

/**
 * @brief exclamationmark ? 4x4
 *
 * .00.
 * .00.
 * ....
 * .00.
 */
_4x4_ _exclm_4x4 = {.r0 = 0b01100110, .r1 = 0b00000110};

/**
 * @brief questionmark ? 4x4
 *
 * .000
 * ..00
 * ....
 * ..0.
 */
_4x4_ _question_4x4 = {.r0 = 0b01110011, .r1 = 0b00000010};

/**
 * @brief double quotationmark " 4x4
 *
 * .0.0
 * 0.0.
 * ....
 * ....
 */
_4x4_ _quot_4x4 = {.r0 = 0b01011010, .r1 = 0b00000000};

/**
 * @brief backtick  4x4
 *
 * ..0.
 * .0..
 * ....
 * ....
 */
_4x4_ _backtick_4x4 = {.r0 = 0b00100100, .r1 = 0b00000000};

/**
 * @brief fronttick  4x4
 *
 * .0..
 * ..0.
 * ....
 * ....
 */
_4x4_ _fronttick_4x4 = {.r0 = 0b01000010, .r1 = 0b00000000};

/**
 * @brief single quotationmark ' 4x4
 *
 * .0..
 * .0..
 * ....
 * ....
 */
_4x4_ _singlequot_4x4 = {.r0 = 0b01000100, .r1 = 0b00000000};

/**
 * @brief hash # 4x4
 *
 * .0.0
 * 0.0.
 * .0.0
 * 0.0.
 */
_4x4_ _hash_4x4 = {.r0 = 0b01011010, .r1 = 0b01011010};

/**
 * @brief backslash \ 4x4
 *
 * 0...
 * .0..
 * ..0.
 * ...0
 */
_4x4_ _backsl_4x4 = {.r0 = 0b10000100, .r1 = 0b00100001};

/**
 * @brief space  4x4
 *
 * ....
 * ....
 * ....
 * ....
 */
_4x4_ _sp_4x4 = {.r0 = 0b00000000, .r1 = 0b00000000};

// Numerical 4x4 glyphs
/**
 * @brief 0 4x4
 *
 * .00.
 * 00.0
 * 0.00
 * .00.
 */

_4x4_ _0_4x4 = {.r0 = 0b01101101, .r1 = 0b10110110};

/**
 * @brief 1 4x4
 *
 * 00..
 * .0..
 * .0..
 * 000.
 */
_4x4_ _1_4x4 = {.r0 = 0b11100110, .r1 = 0b01101111};

/**
 * @brief 2 4x4
 *
 * .00.
 * 0..0
 * ..0.
 * 0000
 */
_4x4_ _2_4x4 = {.r0 = 0b01101001, .r1 = 0b00101111};

/**
 * @brief 3 4x4
 *
 * 0000
 * .000
 * ...0
 * 0000
 */
_4x4_ _3_4x4 = {.r0 = 0b11110111, .r1 = 0b00011111};

/**
 * @brief 4 4x4
 *
 * 0..0
 * 0..0
 * 0000
 * ...0
 */
_4x4_ _4_4x4 = {.r0 = 0b10011001, .r1 = 0b11110001};

/**
 * @brief 5 4x4
 *
 * 0000
 * 00..
 * -.00
 * 0000
 */
_4x4_ _5_4x4 = {.r0 = 0b11111100, .r1 = 0b00111111};

/**
 * @brief 6 4x4
 *
 * 0000
 * 00..
 * 0.00
 * 0000
 */
_4x4_ _6_4x4 = {.r0 = 0b11111100, .r1 = 0b10111111};

/**
 * @brief 7 4x4
 *
 * 0000
 * ...0
 * ...0
 * ...0
 */
_4x4_ _7_4x4 = {.r0 = 0b11110001, .r1 = 0b00010001};

/**
 * @brief 8 4x4
 *
 * 000.
 * 0.0.
 * .0.0
 * .000
 */
_4x4_ _8_4x4 = {.r0 = 0b11101010, .r1 = 0b01010111};

/**
 * @brief 9 4x4
 *
 * 0000
 * 0..0
 * 0000
 * ...0
 */
_4x4_ _9_4x4 = {.r0 = 0b11111001, .r1 = 0b11110001};

// Alphabetical 4x4 glyphs
/**
 * @brief a 4x4
 *
 * ..0.
 * 0..0
 * 0000
 * 0..0
 */
_4x4_ _a_4x4 = {.r0 = 0b00000000, .r1 = 0b00000000};

/**
 * @brief b 4x4
 *
 * 0...
 * 000.
 * 0.0.
 * 000.
 */
_4x4_ _b_4x4 = {.r0 = 0b10001111, .r1 = 0b10011111};

/**
 * @brief c 4x4
 *
 * ....
 * 000.
 * 0...
 * 000.
 */
_4x4_ _c_4x4 = {.r0 = 0b00001111, .r1 = 0b10001111};

/**
 * @brief d 4x4
 *
 * ....
 * ..0.
 * 000.
 * 000.
 */
_4x4_ _d_4x4 = {.r0 = 0b00011111, .r1 = 0b10011111};

/**
 * @brief e 4x4
 *
 * 000.
 * 0...
 * 00..
 * 000.
 */
_4x4_ _e_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief f 4x4
 *
 * 000.
 * 0...
 * 00..
 * 0...
 */
_4x4_ _f_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief g 4x4
 *
 * 000.
 * 0...
 * 0.0.
 * 000.
 */
_4x4_ _g_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief h 4x4
 *
 * ....
 * 0.0.
 * 000.
 * 0.0.
 */
_4x4_ _h_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief i 4x4
 *
 * ....
 * 000.
 * .0..
 * 000.
 */
_4x4_ _i_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief j 4x4
 *
 * ....
 * 000.
 * ..0.
 * .00.
 */
_4x4_ _j_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief k 4x4
 *
 * ....
 * 0.0.
 * 00..
 * 0.0.
 */
_4x4_ _k_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief l 4x4
 *
 * ....
 * 0...
 * 0...
 * 000.
 */
_4x4_ _l_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief m 4x4
 *
 * 0..0
 * 0000
 * 00.0
 * 0..0
 */
_4x4_ _m_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief n 4x4
 *
 * 0..0
 * 00.0
 * 0.00
 * 0..0
 */
_4x4_ _n_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief o 4x4
 *
 * ....
 * 000.
 * 0.0.
 * 000.
 */
_4x4_ _o_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};
/**
 * @brief p 4x4
 *
 * 0000
 * 0..0
 * 000.
 * 0...
 */
_4x4_ _p_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief q 4x4
 *
 * 000.
 * 0.0.
 * 000.
 * ...0
 */
_4x4_ _q_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief r 4x4
 *
 * 000.
 * 0.0.
 * 00..
 * 0.0.
 */
_4x4_ _r_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief s 4x4
 *
 * 000.
 * 0...
 * 000.
 * .00.
 */
_4x4_ _s_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief t 4x4
 *
 * ....
 * 000.
 * .0..
 * .0..
 */
_4x4_ _t_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief u 4x4
 *
 * ....
 * ....
 * 0.0.
 * 000.
 */
_4x4_ _u_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief v 4x4
 *
 * ....
 * 0.0.
 * 0.0.
 * .0..
 */
_4x4_ _v_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief w 4x4
 *
 * 0..0
 * 00.0
 * 0000
 * 0..0
 */
_4x4_ _w_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief x 4x4
 *
 * ....
 * 0.0.
 * .0..
 * 0.0.
 */
_4x4_ _x_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief y 4x4
 *
 * 0.0.
 * 000.
 * .0..
 * .0..
 */
_4x4_ _y_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief z 4x4
 *
 * 000.
 * ..0.
 * .0..
 * 000.
 */
_4x4_ _z_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief å 4x4
 * .0..
 * ....
 * 000.
 * 0.0.
 */
_4x4_ _ao_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief ä 4x4
 *
 * 0.0.
 * ....
 * 000.
 * 0.0.
 */
_4x4_ _ae_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

/**
 * @brief ö 4x4
 *
 * 0.0.
 * ....
 * 000.
 * 000.
 */
_4x4_ _oe_4x4 = {.r0 = 0b11111000, .r1 = 0b11101111};

_4x4_ *
get_glyph_4x4(char * inchar)
{
  // Special char switch:
  switch (*inchar) {
    case ' ': return &_sp_4x4;
    case '=': return &_eq_4x4;
    case '+': return &_add_4x4;
    case '-': return &_sub_4x4;
    case '/': return &_div_4x4;
    case '%': return &_rem_4x4;
    case '|': return &_or_4x4;
    case '~': return &_not_4x4;
    case '&': return &_and_4x4;
    case '@': return &_at_4x4;
    case '_': return &_undscr_4x4;
    case ';': return &_semicln_4x4;
    case ':': return &_cln_4x4;
    case '.': return &_dot_4x4;
    case ',': return &_comma_4x4;
    case '[': return &_lsqbrack_4x4;
    case ']': return &_rsqbrack_4x4;
    case '{': return &_lbrack_4x4;
    case '}': return &_rbrack_4x4;
    case '(': return &_lprthss_4x4;
    case ')': return &_rprthss_4x4;
    case '^': return &_xor_4x4;
    case '<': return &_leftsh_4x4;
    case '>': return &_rightsh_4x4;
    case '!': return &_exclm_4x4;
    case '?': return &_question_4x4;
    case '"': return &_quot_4x4;
    case '\'': return &_fronttick_4x4;
    case '`': return &_backtick_4x4;
    case '#': return &_hash_4x4;
    case '\\': return &_backsl_4x4;
    default: break;
  }

  // Numerical switch:
  switch (*inchar) {
    case '0': return &_0_4x4;
    case '1': return &_1_4x4;
    case '2': return &_2_4x4;
    case '3': return &_3_4x4;
    case '4': return &_4_4x4;
    case '5': return &_5_4x4;
    case '6': return &_6_4x4;
    case '7': return &_7_4x4;
    case '8': return &_8_4x4;
    case '9': return &_9_4x4;

    default: break;
  }

  // Alphabetical switch:
  switch (*inchar) {
    case 'a':
    case 'A': return &_a_4x4;
    case 'b':
    case 'B': return &_b_4x4;
    case 'c':
    case 'C': return &_c_4x4;
    case 'd':
    case 'D': return &_d_4x4;
    case 'e':
    case 'E': return &_e_4x4;
    case 'f':
    case 'F': return &_f_4x4;
    case 'g':
    case 'G': return &_g_4x4;
    case 'h':
    case 'H': return &_h_4x4;
    case 'i':
    case 'I': return &_i_4x4;
    case 'j':
    case 'J': return &_j_4x4;
    case 'k':
    case 'K': return &_k_4x4;
    case 'l':
    case 'L': return &_l_4x4;
    case 'm':
    case 'M': return &_m_4x4;
    case 'n':
    case 'N': return &_n_4x4;
    case 'o':
    case 'O': return &_o_4x4;
    case 'p':
    case 'P': return &_p_4x4;
    case 'q':
    case 'Q': return &_q_4x4;
    case 'r':
    case 'R': return &_r_4x4;
    case 's':
    case 'S': return &_s_4x4;
    case 't':
    case 'T': return &_t_4x4;
    case 'u':
    case 'U': return &_u_4x4;
    case 'v':
    case 'V': return &_v_4x4;
    case 'w':
    case 'W': return &_w_4x4;
    case 'x':
    case 'X': return &_x_4x4;
    case 'y':
    case 'Y': return &_y_4x4;
    case 'z':
    case 'Z': return &_z_4x4;

    default: break;
  }
  return ((_4x4_ *)0); // void/NOTFOUND
}

// 8x8 glyphs