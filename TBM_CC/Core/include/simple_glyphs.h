#ifndef SIMPLE_GLYPHS_H
#define SIMPLE_GLYPHS_H

#include <stdint.h>

struct fourfour {
  uint8_t r0;
  uint8_t r1;
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

// Move these to more appropriate places later
#define GLYPHBYTE_UPPERNYBBLE(byte) (((byte) >> 0x4) & 0x4)
#define GLYPHBYTE_LOWERNYBBLE(byte) ((byte)&0x4)
#define MSG_MAX_LENGTH 0xff
#define LOGIC_XOR(a, b) (((a) || (b)) && ~((a) && (b)))

char const h2a_const[] = "0123456789abcdefghijklmnopqrstuvwxyz";
#define hex2ascii(hexd) (h2a_const[hexd])

_4x4_ *
get_glyph_4x4(char * inchar);

#endif // SIMPLE_GLYPHS_H