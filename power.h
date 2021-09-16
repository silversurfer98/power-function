#pragma once

/*this is a very basic structure to calculate a^ b in float precision
to be developed for double precision
need more knowledge to bit wise manipulation of double*/

// https://www.netlib.org/fdlibm/e_log.c
// https://www.netlib.org/fdlibm/e_exp.c

static double
ln2HI = 6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */
ln2LO = 1.90821492927058770002e-10,  /* 0x3dea39ef, 0x35793c76 */
invln2 = 1.44269504088896338700e+00, /* 0x3ff71547, 0x652b82fe */
P1 = 1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
P2 = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
P3 = 6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
P4 = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
P5 = 4.13813679705723846039e-08, /* 0x3E663769, 0x72BEA4D0 */
L1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
L2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
L3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
L4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
L5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
L6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
L7 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */

float ln(float f);
float epower(float x);
float mypower(float x, float y);