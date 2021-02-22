#ifndef SIMPLE_GLYPHS_H
#define SIMPLE_GLYPHS_H

#include <stdint.h>

struct fourfour {
  uint8_t r0;
  uint8_t r1;
};

struct sevensix {
  uint8_t r0;
  uint8_t r1;
  uint8_t r2;
  uint8_t r3;
  uint8_t r4;
  uint8_t r5;
  uint8_t r6;
};

struct eighteight {
  uint8_t r0;
  uint8_t r1;
  uint8_t r2;
  uint8_t r3;
  uint8_t r4;
  uint8_t r5;
  uint8_t r6;
  uint8_t r7;
};

typedef struct fourfour   _4x4_;
typedef struct sevensix   _7x6_;
typedef struct eighteight _8x8_;

// specials
extern _4x4_ _eq_4x4;
extern _4x4_ _add_4x4;
extern _4x4_ _sub_4x4;
extern _4x4_ _mul_4x4;
extern _4x4_ _div_4x4;
extern _4x4_ _rem_4x4;
extern _4x4_ _or_4x4;
extern _4x4_ _not_4x4;
extern _4x4_ _and_4x4;
extern _4x4_ _at_4x4;
extern _4x4_ _undscr_4x4;
extern _4x4_ _semicln_4x4;
extern _4x4_ _cln_4x4;
extern _4x4_ _dot_4x4;
extern _4x4_ _comma_4x4;
extern _4x4_ _lsqbrack_4x4;
extern _4x4_ _rsqbrack_4x4;
extern _4x4_ _lbrack_4x4;
extern _4x4_ _rbrack_4x4;
extern _4x4_ _lprthss_4x4;
extern _4x4_ _rprthss_4x4;
extern _4x4_ _xor_4x4;
extern _4x4_ _leftsh_4x4;
extern _4x4_ _rightsh_4x4;
extern _4x4_ _exclm_4x4;
extern _4x4_ _question_4x4;
extern _4x4_ _quot_4x4;
extern _4x4_ _backtick_4x4;
extern _4x4_ _fronttick_4x4;
extern _4x4_ _singlequot_4x4;
extern _4x4_ _hash_4x4;
extern _4x4_ _backsl_4x4;
extern _4x4_ _sp_4x4;

// digits
extern _4x4_ _0_4x4;
extern _4x4_ _1_4x4;
extern _4x4_ _2_4x4;
extern _4x4_ _3_4x4;
extern _4x4_ _4_4x4;
extern _4x4_ _5_4x4;
extern _4x4_ _6_4x4;
extern _4x4_ _7_4x4;
extern _4x4_ _8_4x4;
extern _4x4_ _9_4x4;

// letters
extern _4x4_ _a_4x4;
extern _4x4_ _b_4x4;
extern _4x4_ _c_4x4;
extern _4x4_ _d_4x4;
extern _4x4_ _e_4x4;
extern _4x4_ _f_4x4;
extern _4x4_ _g_4x4;
extern _4x4_ _h_4x4;
extern _4x4_ _i_4x4;
extern _4x4_ _j_4x4;
extern _4x4_ _k_4x4;
extern _4x4_ _l_4x4;
extern _4x4_ _m_4x4;
extern _4x4_ _n_4x4;
extern _4x4_ _o_4x4;
extern _4x4_ _p_4x4;
extern _4x4_ _q_4x4;
extern _4x4_ _r_4x4;
extern _4x4_ _s_4x4;
extern _4x4_ _t_4x4;
extern _4x4_ _u_4x4;
extern _4x4_ _v_4x4;
extern _4x4_ _w_4x4;
extern _4x4_ _x_4x4;
extern _4x4_ _y_4x4;
extern _4x4_ _z_4x4;
extern _4x4_ _ao_4x4;
extern _4x4_ _ae_4x4;
extern _4x4_ _oe_4x4;

// 7x6
// specials
extern _7x6_ _eq_7x6;
extern _7x6_ _add_7x6;
extern _7x6_ _sub_7x6;
extern _7x6_ _mul_7x6;
extern _7x6_ _div_7x6;
extern _7x6_ _rem_7x6;
extern _7x6_ _or_7x6;
extern _7x6_ _not_7x6;
extern _7x6_ _and_7x6;
extern _7x6_ _at_7x6;
extern _7x6_ _undscr_7x6;
extern _7x6_ _semicln_7x6;
extern _7x6_ _cln_7x6;
extern _7x6_ _dot_7x6;
extern _7x6_ _comma_7x6;
extern _7x6_ _lsqbrack_7x6;
extern _7x6_ _rsqbrack_7x6;
extern _7x6_ _lbrack_7x6;
extern _7x6_ _rbrack_7x6;
extern _7x6_ _lprthss_7x6;
extern _7x6_ _rprthss_7x6;
extern _7x6_ _xor_7x6;
extern _7x6_ _leftsh_7x6;
extern _7x6_ _rightsh_7x6;
extern _7x6_ _exclm_7x6;
extern _7x6_ _question_7x6;
extern _7x6_ _quot_7x6;
extern _7x6_ _backtick_7x6;
extern _7x6_ _fronttick_7x6;
extern _7x6_ _singlequot_7x6;
extern _7x6_ _hash_7x6;
extern _7x6_ _backsl_7x6;
extern _7x6_ _sp_7x6;

// digits
extern _7x6_ _0_7x6;
extern _7x6_ _1_7x6;
extern _7x6_ _2_7x6;
extern _7x6_ _3_7x6;
extern _7x6_ _4_7x6;
extern _7x6_ _5_7x6;
extern _7x6_ _6_7x6;
extern _7x6_ _7_7x6;
extern _7x6_ _8_7x6;
extern _7x6_ _9_7x6;

// letters
extern _7x6_ _a_7x6;
extern _7x6_ _b_7x6;
extern _7x6_ _c_7x6;
extern _7x6_ _d_7x6;
extern _7x6_ _e_7x6;
extern _7x6_ _f_7x6;
extern _7x6_ _g_7x6;
extern _7x6_ _h_7x6;
extern _7x6_ _i_7x6;
extern _7x6_ _j_7x6;
extern _7x6_ _k_7x6;
extern _7x6_ _l_7x6;
extern _7x6_ _m_7x6;
extern _7x6_ _n_7x6;
extern _7x6_ _o_7x6;
extern _7x6_ _p_7x6;
extern _7x6_ _q_7x6;
extern _7x6_ _r_7x6;
extern _7x6_ _s_7x6;
extern _7x6_ _t_7x6;
extern _7x6_ _u_7x6;
extern _7x6_ _v_7x6;
extern _7x6_ _w_7x6;
extern _7x6_ _x_7x6;
extern _7x6_ _y_7x6;
extern _7x6_ _z_7x6;
extern _7x6_ _ao_7x6;
extern _7x6_ _ae_7x6;
extern _7x6_ _oe_7x6;

/*
// 8x8
// specials
extern _8x8_ _eq_8x8;
extern _8x8_ _add_8x8;
extern _8x8_ _sub_8x8;
extern _8x8_ _mul_8x8;
extern _8x8_ _div_8x8;
extern _8x8_ _rem_8x8;
extern _8x8_ _or_8x8;
extern _8x8_ _not_8x8;
extern _8x8_ _and_8x8;
extern _8x8_ _at_8x8;
extern _8x8_ _undscr_8x8;
extern _8x8_ _semicln_8x8;
extern _8x8_ _cln_8x8;
extern _8x8_ _dot_8x8;
extern _8x8_ _comma_8x8;
extern _8x8_ _lsqbrack_8x8;
extern _8x8_ _rsqbrack_8x8;
extern _8x8_ _lbrack_8x8;
extern _8x8_ _rbrack_8x8;
extern _8x8_ _lprthss_8x8;
extern _8x8_ _rprthss_8x8;
extern _8x8_ _xor_8x8;
extern _8x8_ _leftsh_8x8;
extern _8x8_ _rightsh_8x8;
extern _8x8_ _exclm_8x8;
extern _8x8_ _question_8x8;
extern _8x8_ _quot_8x8;
extern _8x8_ _backtick_8x8;
extern _8x8_ _fronttick_8x8;
extern _8x8_ _singlequot_8x8;
extern _8x8_ _hash_8x8;
extern _8x8_ _backsl_8x8;
extern _8x8_ _sp_8x8;

// digits
extern _8x8_ _0_8x8;
extern _8x8_ _1_8x8;
extern _8x8_ _2_8x8;
extern _8x8_ _3_8x8;
extern _8x8_ _4_8x8;
extern _8x8_ _5_8x8;
extern _8x8_ _6_8x8;
extern _8x8_ _7_8x8;
extern _8x8_ _8_8x8;
extern _8x8_ _9_8x8;

// letters
extern _8x8_ _a_8x8;
extern _8x8_ _b_8x8;
extern _8x8_ _c_8x8;
extern _8x8_ _d_8x8;
extern _8x8_ _e_8x8;
extern _8x8_ _f_8x8;
extern _8x8_ _g_8x8;
extern _8x8_ _h_8x8;
extern _8x8_ _i_8x8;
extern _8x8_ _j_8x8;
extern _8x8_ _k_8x8;
extern _8x8_ _l_8x8;
extern _8x8_ _m_8x8;
extern _8x8_ _n_8x8;
extern _8x8_ _o_8x8;
extern _8x8_ _p_8x8;
extern _8x8_ _q_8x8;
extern _8x8_ _r_8x8;
extern _8x8_ _s_8x8;
extern _8x8_ _t_8x8;
extern _8x8_ _u_8x8;
extern _8x8_ _v_8x8;
extern _8x8_ _w_8x8;
extern _8x8_ _x_8x8;
extern _8x8_ _y_8x8;
extern _8x8_ _z_8x8;
extern _8x8_ _ao_8x8;
extern _8x8_ _ae_8x8;
extern _8x8_ _oe_8x8;
*/

// Move these to more appropriate places later
#define GLYPHBYTE_UPPERNYBBLE(byte) (((byte) >> 0x4) & 0x4)
#define GLYPHBYTE_LOWERNYBBLE(byte) ((byte)&0x4)
#define MSG_MAX_LENGTH 0xff
#define LOGIC_XOR(a, b) (((a) || (b)) && ~((a) && (b)))

extern char const h2a_const[];
#define hex2ascii(hexd) (h2a_const[hexd])

_4x4_ *
get_glyph_4x4(char * inchar);

_7x6_ *
get_glyph_7x6(char * inchar);

_8x8_ *
get_glyph_8x8(char * inchar);

#endif // SIMPLE_GLYPHS_H