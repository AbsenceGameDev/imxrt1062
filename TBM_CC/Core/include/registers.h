#ifndef REGISTERS_H
#define REGISTERS_H
/* Following assumes long is 32-bits */
typedef volatile unsigned long  vuint32_t;
typedef volatile unsigned short vuint16_t;
typedef volatile unsigned char  vuint8_t;
// void *                          DMAMUX_BASE_ADDR = 0x40094000;

//
//
//
// 32BIT REGISTER  OFFSETS
//
typedef struct {
  vuint32_t _0x0000;
  vuint32_t _0x0004;
  vuint32_t _0x0008;
  vuint32_t _0x000c;
  vuint32_t _0x0010;
  vuint32_t _0x0014;
  vuint32_t _0x0018;
  vuint32_t _0x001c;
  vuint32_t _0x0020;
  vuint32_t _0x0024;
  vuint32_t _0x0028;
  vuint32_t _0x002c;
  vuint32_t _0x0030;
  vuint32_t _0x0034;
  vuint32_t _0x0038;
  vuint32_t _0x003c;

  vuint32_t _0x0040;
  vuint32_t _0x0044;
  vuint32_t _0x0048;
  vuint32_t _0x004c;
  vuint32_t _0x0050;
  vuint32_t _0x0054;
  vuint32_t _0x0058;
  vuint32_t _0x005c;
  vuint32_t _0x0060;
  vuint32_t _0x0064;
  vuint32_t _0x0068;
  vuint32_t _0x006c;
  vuint32_t _0x0070;
  vuint32_t _0x0074;
  vuint32_t _0x0078;
  vuint32_t _0x007c;

  vuint32_t _0x0080;
  vuint32_t _0x0084;
  vuint32_t _0x0088;
  vuint32_t _0x008c;
  vuint32_t _0x0090;
  vuint32_t _0x0094;
  vuint32_t _0x0098;
  vuint32_t _0x009c;
  vuint32_t _0x00a0;
  vuint32_t _0x00a4;
  vuint32_t _0x00a8;
  vuint32_t _0x00ac;
  vuint32_t _0x00b0;
  vuint32_t _0x00b4;
  vuint32_t _0x00b8;
  vuint32_t _0x00bc;

  vuint32_t _0x00c0;
  vuint32_t _0x00c4;
  vuint32_t _0x00c8;
  vuint32_t _0x00cc;
  vuint32_t _0x00d0;
  vuint32_t _0x00d4;
  vuint32_t _0x00d8;
  vuint32_t _0x00dc;
  vuint32_t _0x00e0;
  vuint32_t _0x00e4;
  vuint32_t _0x00e8;
  vuint32_t _0x00ec;
  vuint32_t _0x00f0;
  vuint32_t _0x00f4;
  vuint32_t _0x00f8;
  vuint32_t _0x00fc;

  vuint32_t _0x0100;
  vuint32_t _0x0104;
  vuint32_t _0x0108;
  vuint32_t _0x010c;
  vuint32_t _0x0110;
  vuint32_t _0x0114;
  vuint32_t _0x0118;
  vuint32_t _0x011c;
  vuint32_t _0x0120;
  vuint32_t _0x0124;
  vuint32_t _0x0128;
  vuint32_t _0x012c;
  vuint32_t _0x0130;
  vuint32_t _0x0134;
  vuint32_t _0x0138;
  vuint32_t _0x013c;

  vuint32_t _0x0140;
  vuint32_t _0x0144;
  vuint32_t _0x0148;
  vuint32_t _0x014c;
  vuint32_t _0x0150;
  vuint32_t _0x0154;
  vuint32_t _0x0158;
  vuint32_t _0x015c;
  vuint32_t _0x0160;
  vuint32_t _0x0164;
  vuint32_t _0x0168;
  vuint32_t _0x016c;
  vuint32_t _0x0170;
  vuint32_t _0x0174;
  vuint32_t _0x0178;
  vuint32_t _0x017c;

  vuint32_t _0x0180;
  vuint32_t _0x0184;
  vuint32_t _0x0188;
  vuint32_t _0x018c;
  vuint32_t _0x0190;
  vuint32_t _0x0194;
  vuint32_t _0x0198;
  vuint32_t _0x019c;
  vuint32_t _0x01a0;
  vuint32_t _0x01a4;
  vuint32_t _0x01a8;
  vuint32_t _0x01ac;
  vuint32_t _0x01b0;
  vuint32_t _0x01b4;
  vuint32_t _0x01b8;
  vuint32_t _0x01bc;

  vuint32_t _0x01c0;
  vuint32_t _0x01c4;
  vuint32_t _0x01c8;
  vuint32_t _0x01cc;
  vuint32_t _0x01d0;
  vuint32_t _0x01d4;
  vuint32_t _0x01d8;
  vuint32_t _0x01dc;
  vuint32_t _0x01e0;
  vuint32_t _0x01e4;
  vuint32_t _0x01e8;
  vuint32_t _0x01ec;
  vuint32_t _0x01f0;
  vuint32_t _0x01f4;
  vuint32_t _0x01f8;
  vuint32_t _0x01fc;

  vuint32_t _0x0200;
  vuint32_t _0x0204;
  vuint32_t _0x0208;
  vuint32_t _0x020c;
  vuint32_t _0x0210;
  vuint32_t _0x0214;
  vuint32_t _0x0218;
  vuint32_t _0x021c;
  vuint32_t _0x0220;
  vuint32_t _0x0224;
  vuint32_t _0x0228;
  vuint32_t _0x022c;
  vuint32_t _0x0230;
  vuint32_t _0x0234;
  vuint32_t _0x0238;
  vuint32_t _0x023c;
  vuint32_t _0x0240;

  vuint32_t _0x0244;
  vuint32_t _0x0248;
  vuint32_t _0x024c;
  vuint32_t _0x0250;
  vuint32_t _0x0254;
  vuint32_t _0x0258;
  vuint32_t _0x025c;
  vuint32_t _0x0260;
  vuint32_t _0x0264;
  vuint32_t _0x0268;
  vuint32_t _0x026c;
  vuint32_t _0x0270;
  vuint32_t _0x0274;
  vuint32_t _0x0278;
  vuint32_t _0x027c;

  vuint32_t _0x0280;
  vuint32_t _0x0284;
  vuint32_t _0x0288;
  vuint32_t _0x028c;
  vuint32_t _0x0290;
  vuint32_t _0x0294;
  vuint32_t _0x0298;
  vuint32_t _0x029c;
  vuint32_t _0x02a0;
  vuint32_t _0x02a4;
  vuint32_t _0x02a8;
  vuint32_t _0x02ac;
  vuint32_t _0x02b0;
  vuint32_t _0x02b4;
  vuint32_t _0x02b8;
  vuint32_t _0x02bc;

  vuint32_t _0x02c0;
  vuint32_t _0x02c4;
  vuint32_t _0x02c8;
  vuint32_t _0x02cc;
  vuint32_t _0x02d0;
  vuint32_t _0x02d4;
  vuint32_t _0x02d8;
  vuint32_t _0x02dc;
  vuint32_t _0x02e0;
  vuint32_t _0x02e4;
  vuint32_t _0x02e8;
  vuint32_t _0x02ec;
  vuint32_t _0x02f0;
  vuint32_t _0x02f4;
  vuint32_t _0x02f8;
  vuint32_t _0x02fc;

  vuint32_t _0x0300;
  vuint32_t _0x0304;
  vuint32_t _0x0308;
  vuint32_t _0x030c;
  vuint32_t _0x0310;
  vuint32_t _0x0314;
  vuint32_t _0x0318;
  vuint32_t _0x031c;
  vuint32_t _0x0320;
  vuint32_t _0x0324;
  vuint32_t _0x0328;
  vuint32_t _0x032c;
  vuint32_t _0x0330;
  vuint32_t _0x0334;
  vuint32_t _0x0338;
  vuint32_t _0x033c;

  vuint32_t _0x0340;
  vuint32_t _0x0344;
  vuint32_t _0x0348;
  vuint32_t _0x034c;
  vuint32_t _0x0350;
  vuint32_t _0x0354;
  vuint32_t _0x0358;
  vuint32_t _0x035c;
  vuint32_t _0x0360;
  vuint32_t _0x0364;
  vuint32_t _0x0368;
  vuint32_t _0x036c;
  vuint32_t _0x0370;
  vuint32_t _0x0374;
  vuint32_t _0x0378;
  vuint32_t _0x037c;

  vuint32_t _0x0380;
  vuint32_t _0x0384;
  vuint32_t _0x0388;
  vuint32_t _0x038c;
  vuint32_t _0x0390;
  vuint32_t _0x0394;
  vuint32_t _0x0398;
  vuint32_t _0x039c;
  vuint32_t _0x03a0;
  vuint32_t _0x03a4;
  vuint32_t _0x03a8;
  vuint32_t _0x03ac;
  vuint32_t _0x03b0;
  vuint32_t _0x03b4;
  vuint32_t _0x03b8;
  vuint32_t _0x03bc;

  vuint32_t _0x03c0;
  vuint32_t _0x03c4;
  vuint32_t _0x03c8;
  vuint32_t _0x03cc;
  vuint32_t _0x03d0;
  vuint32_t _0x03d4;
  vuint32_t _0x03d8;
  vuint32_t _0x03dc;
  vuint32_t _0x03e0;
  vuint32_t _0x03e4;
  vuint32_t _0x03e8;
  vuint32_t _0x03ec;
  vuint32_t _0x03f0;
  vuint32_t _0x03f4;
  vuint32_t _0x03f8;
  vuint32_t _0x03fc;
} imxrt_32t_offset;

//
//
//
// 16BIT REGISTER  OFFSETS
//
typedef struct {
  vuint16_t _0x0000;
  vuint16_t _0x0002;
  vuint16_t _0x0004;
  vuint16_t _0x0006;
  vuint16_t _0x0008;
  vuint16_t _0x000a;
  vuint16_t _0x000c;
  vuint16_t _0x000e;
  vuint16_t _0x0010;
  vuint16_t _0x0012;
  vuint16_t _0x0014;
  vuint16_t _0x0016;
  vuint16_t _0x0018;
  vuint16_t _0x001a;
  vuint16_t _0x001c;
  vuint16_t _0x001e;

  vuint16_t _0x0020;
  vuint16_t _0x0022;
  vuint16_t _0x0024;
  vuint16_t _0x0026;
  vuint16_t _0x0028;
  vuint16_t _0x002a;
  vuint16_t _0x002c;
  vuint16_t _0x002e;
  vuint16_t _0x0030;
  vuint16_t _0x0032;
  vuint16_t _0x0034;
  vuint16_t _0x0036;
  vuint16_t _0x0038;
  vuint16_t _0x003a;
  vuint16_t _0x003c;
  vuint16_t _0x003e;

  vuint16_t _0x0040;
  vuint16_t _0x0042;
  vuint16_t _0x0044;
  vuint16_t _0x0046;
  vuint16_t _0x0048;
  vuint16_t _0x004a;
  vuint16_t _0x004c;
  vuint16_t _0x004e;
  vuint16_t _0x0050;
  vuint16_t _0x0052;
  vuint16_t _0x0054;
  vuint16_t _0x0056;
  vuint16_t _0x0058;
  vuint16_t _0x005a;
  vuint16_t _0x005c;
  vuint16_t _0x005e;

  vuint16_t _0x0060;
  vuint16_t _0x0062;
  vuint16_t _0x0064;
  vuint16_t _0x0066;
  vuint16_t _0x0068;
  vuint16_t _0x006a;
  vuint16_t _0x006c;
  vuint16_t _0x006e;
  vuint16_t _0x0070;
  vuint16_t _0x0072;
  vuint16_t _0x0074;
  vuint16_t _0x0076;
  vuint16_t _0x0078;
  vuint16_t _0x007a;
  vuint16_t _0x007c;
  vuint16_t _0x007e;

  vuint16_t _0x0080;
  vuint16_t _0x0082;
  vuint16_t _0x0084;
  vuint16_t _0x0086;
  vuint16_t _0x0088;
  vuint16_t _0x008a;
  vuint16_t _0x008c;
  vuint16_t _0x008e;
  vuint16_t _0x0090;
  vuint16_t _0x0092;
  vuint16_t _0x0094;
  vuint16_t _0x0096;
  vuint16_t _0x0098;
  vuint16_t _0x009a;
  vuint16_t _0x009c;
  vuint16_t _0x009e;

  vuint16_t _0x00a0;
  vuint16_t _0x00a2;
  vuint16_t _0x00a4;
  vuint16_t _0x00a6;
  vuint16_t _0x00a8;
  vuint16_t _0x00aa;
  vuint16_t _0x00ac;
  vuint16_t _0x00ae;
  vuint16_t _0x00b0;
  vuint16_t _0x00b2;
  vuint16_t _0x00b4;
  vuint16_t _0x00b6;
  vuint16_t _0x00b8;
  vuint16_t _0x00ba;
  vuint16_t _0x00bc;
  vuint16_t _0x00be;

  vuint16_t _0x00c0;
  vuint16_t _0x00c2;
  vuint16_t _0x00c4;
  vuint16_t _0x00c6;
  vuint16_t _0x00c8;
  vuint16_t _0x00ca;
  vuint16_t _0x00cc;
  vuint16_t _0x00ce;
  vuint16_t _0x00d0;
  vuint16_t _0x00d2;
  vuint16_t _0x00d4;
  vuint16_t _0x00d6;
  vuint16_t _0x00d8;
  vuint16_t _0x00da;
  vuint16_t _0x00dc;
  vuint16_t _0x00de;

  vuint16_t _0x00e0;
  vuint16_t _0x00e2;
  vuint16_t _0x00e4;
  vuint16_t _0x00e6;
  vuint16_t _0x00e8;
  vuint16_t _0x00ea;
  vuint16_t _0x00ec;
  vuint16_t _0x00ee;
  vuint16_t _0x00f0;
  vuint16_t _0x00f2;
  vuint16_t _0x00f4;
  vuint16_t _0x00f6;
  vuint16_t _0x00f8;
  vuint16_t _0x00fa;
  vuint16_t _0x00fc;
  vuint16_t _0x00fe;

  vuint16_t _0x0100;
  vuint16_t _0x0102;
  vuint16_t _0x0104;
  vuint16_t _0x0106;
  vuint16_t _0x0108;
  vuint16_t _0x010a;
  vuint16_t _0x010c;
  vuint16_t _0x010e;
  vuint16_t _0x0110;
  vuint16_t _0x0112;
  vuint16_t _0x0114;
  vuint16_t _0x0116;
  vuint16_t _0x0118;
  vuint16_t _0x011a;
  vuint16_t _0x011c;
  vuint16_t _0x011e;

  vuint16_t _0x0120;
  vuint16_t _0x0122;
  vuint16_t _0x0124;
  vuint16_t _0x0126;
  vuint16_t _0x0128;
  vuint16_t _0x012a;
  vuint16_t _0x012c;
  vuint16_t _0x012e;
  vuint16_t _0x0130;
  vuint16_t _0x0132;
  vuint16_t _0x0134;
  vuint16_t _0x0136;
  vuint16_t _0x0138;
  vuint16_t _0x013a;
  vuint16_t _0x013c;
  vuint16_t _0x013e;

  vuint16_t _0x0140;
  vuint16_t _0x0142;
  vuint16_t _0x0144;
  vuint16_t _0x0146;
  vuint16_t _0x0148;
  vuint16_t _0x014a;
  vuint16_t _0x014c;
  vuint16_t _0x014e;
  vuint16_t _0x0150;
  vuint16_t _0x0152;
  vuint16_t _0x0154;
  vuint16_t _0x0156;
  vuint16_t _0x0158;
  vuint16_t _0x015a;
  vuint16_t _0x015c;
  vuint16_t _0x015e;

  vuint16_t _0x0160;
  vuint16_t _0x0162;
  vuint16_t _0x0164;
  vuint16_t _0x0166;
  vuint16_t _0x0168;
  vuint16_t _0x016a;
  vuint16_t _0x016c;
  vuint16_t _0x016e;
  vuint16_t _0x0170;
  vuint16_t _0x0172;
  vuint16_t _0x0174;
  vuint16_t _0x0176;
  vuint16_t _0x0178;
  vuint16_t _0x017a;
  vuint16_t _0x017c;
  vuint16_t _0x017e;

  vuint16_t _0x0180;
  vuint16_t _0x0182;
  vuint16_t _0x0184;
  vuint16_t _0x0186;
  vuint16_t _0x0188;
  vuint16_t _0x018a;
  vuint16_t _0x018c;
  vuint16_t _0x018e;
  vuint16_t _0x0190;
  vuint16_t _0x0192;
  vuint16_t _0x0194;
  vuint16_t _0x0196;
  vuint16_t _0x0198;
  vuint16_t _0x019a;
  vuint16_t _0x019c;
  vuint16_t _0x019e;

  vuint16_t _0x01a0;
  vuint16_t _0x01a2;
  vuint16_t _0x01a4;
  vuint16_t _0x01a6;
  vuint16_t _0x01a8;
  vuint16_t _0x01aa;
  vuint16_t _0x01ac;
  vuint16_t _0x01ae;
  vuint16_t _0x01b0;
  vuint16_t _0x01b2;
  vuint16_t _0x01b4;
  vuint16_t _0x01b6;
  vuint16_t _0x01b8;
  vuint16_t _0x01ba;
  vuint16_t _0x01bc;
  vuint16_t _0x01be;

  vuint16_t _0x01c0;
  vuint16_t _0x01c2;
  vuint16_t _0x01c4;
  vuint16_t _0x01c6;
  vuint16_t _0x01c8;
  vuint16_t _0x01ca;
  vuint16_t _0x01cc;
  vuint16_t _0x01ce;
  vuint16_t _0x01d0;
  vuint16_t _0x01d2;
  vuint16_t _0x01d4;
  vuint16_t _0x01d6;
  vuint16_t _0x01d8;
  vuint16_t _0x01da;
  vuint16_t _0x01dc;
  vuint16_t _0x01de;

  vuint16_t _0x01e0;
  vuint16_t _0x01e2;
  vuint16_t _0x01e4;
  vuint16_t _0x01e6;
  vuint16_t _0x01e8;
  vuint16_t _0x01ea;
  vuint16_t _0x01ec;
  vuint16_t _0x01ee;
  vuint16_t _0x01f0;
  vuint16_t _0x01f2;
  vuint16_t _0x01f4;
  vuint16_t _0x01f6;
  vuint16_t _0x01f8;
  vuint16_t _0x01fa;
  vuint16_t _0x01fc;
  vuint16_t _0x01fe;

} imxrt_16t_offset;

//
//
//
// 8BIT REGISTER  OFFSETS
//
typedef struct {
  vuint8_t _0x000;
  vuint8_t _0x001;
  vuint8_t _0x002;
  vuint8_t _0x003;
  vuint8_t _0x004;
  vuint8_t _0x005;
  vuint8_t _0x006;
  vuint8_t _0x007;
  vuint8_t _0x008;
  vuint8_t _0x009;
  vuint8_t _0x00a;
  vuint8_t _0x00b;
  vuint8_t _0x00c;
  vuint8_t _0x00d;
  vuint8_t _0x00e;
  vuint8_t _0x00f;

  vuint8_t _0x010;
  vuint8_t _0x011;
  vuint8_t _0x012;
  vuint8_t _0x013;
  vuint8_t _0x014;
  vuint8_t _0x015;
  vuint8_t _0x016;
  vuint8_t _0x017;
  vuint8_t _0x018;
  vuint8_t _0x019;
  vuint8_t _0x01a;
  vuint8_t _0x01b;
  vuint8_t _0x01c;
  vuint8_t _0x01d;
  vuint8_t _0x01e;
  vuint8_t _0x01f;

  vuint8_t _0x020;
  vuint8_t _0x021;
  vuint8_t _0x022;
  vuint8_t _0x023;
  vuint8_t _0x024;
  vuint8_t _0x025;
  vuint8_t _0x026;
  vuint8_t _0x027;
  vuint8_t _0x028;
  vuint8_t _0x029;
  vuint8_t _0x02a;
  vuint8_t _0x02b;
  vuint8_t _0x02c;
  vuint8_t _0x02d;
  vuint8_t _0x02e;
  vuint8_t _0x02f;

  vuint8_t _0x030;
  vuint8_t _0x031;
  vuint8_t _0x032;
  vuint8_t _0x033;
  vuint8_t _0x034;
  vuint8_t _0x035;
  vuint8_t _0x036;
  vuint8_t _0x037;
  vuint8_t _0x038;
  vuint8_t _0x039;
  vuint8_t _0x03a;
  vuint8_t _0x03b;
  vuint8_t _0x03c;
  vuint8_t _0x03d;
  vuint8_t _0x03e;
  vuint8_t _0x03f;

} imxrt_8t_offset;

#define MAP_8BIT_REGISTER(ADDRESS) (*(imcrt_8t_offset * ADDRESS)
#define MAP_16BIT_REGISTER(ADDRESS) (*(imcrt_16t_offset * ADDRESS)
#define MAP_32BIT_REGISTER(ADDRESS) (*(imcrt_32t_offset * ADDRESS)
#define _OFFSET0(A, B) (A + B)

#endif // REGISTERS_H