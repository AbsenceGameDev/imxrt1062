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

#define _0_GLYPH _0_4x4
#define _1_GLYPH _1_4x4
#define _2_GLYPH _2_4x4
#define _3_GLYPH _3_4x4
#define _4_GLYPH _4_4x4
#define _5_GLYPH _5_4x4
#define _6_GLYPH _6_4x4
#define _7_GLYPH _7_4x4
#define _8_GLYPH _8_4x4
#define _9_GLYPH _9_4x4

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

#define a_GLYPH _a_4x4
#define A_GLYPH _a_4x4
#define b_GLYPH _b_4x4
#define B_GLYPH _b_4x4
#define c_GLYPH _c_4x4
#define C_GLYPH _c_4x4
#define d_GLYPH _d_4x4
#define D_GLYPH _d_4x4
#define e_GLYPH _e_4x4
#define E_GLYPH _e_4x4
#define f_GLYPH _f_4x4
#define F_GLYPH _f_4x4
#define g_GLYPH _g_4x4
#define G_GLYPH _g_4x4
#define h_GLYPH _h_4x4
#define H_GLYPH _h_4x4
#define i_GLYPH _i_4x4
#define I_GLYPH _i_4x4
#define j_GLYPH _j_4x4
#define J_GLYPH _j_4x4
#define k_GLYPH _k_4x4
#define K_GLYPH _k_4x4
#define l_GLYPH _l_4x4
#define L_GLYPH _l_4x4
#define m_GLYPH _m_4x4
#define M_GLYPH _m_4x4
#define n_GLYPH _n_4x4
#define N_GLYPH _n_4x4
#define o_GLYPH _o_4x4
#define O_GLYPH _o_4x4
#define p_GLYPH _p_4x4
#define P_GLYPH _p_4x4
#define q_GLYPH _q_4x4
#define Q_GLYPH _q_4x4
#define r_GLYPH _r_4x4
#define R_GLYPH _r_4x4
#define s_GLYPH _s_4x4
#define S_GLYPH _s_4x4
#define t_GLYPH _t_4x4
#define T_GLYPH _t_4x4
#define u_GLYPH _u_4x4
#define U_GLYPH _u_4x4
#define v_GLYPH _v_4x4
#define V_GLYPH _v_4x4
#define w_GLYPH _w_4x4
#define W_GLYPH _w_4x4
#define x_GLYPH _x_4x4
#define X_GLYPH _x_4x4
#define y_GLYPH _y_4x4
#define Y_GLYPH _y_4x4
#define z_GLYPH _z_4x4
#define Z_GLYPH _z_4x4
#define ao_GLYPH _ao_4x4
#define AO_GLYPH _ao_4x4
#define ae_GLYPH _ae_4x4
#define AE_GLYPH _ae_4x4
#define oe_GLYPH _oe_4x4
#define OE_GLYPH _oe_4x4

#endif // SIMPLE_GLYPHS_H