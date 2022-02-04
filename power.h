#pragma once

// https://www.h-schmidt.net/FloatConverter/IEEE754.html
// im using IEEE 754 stds

# define MAX_PRECISION_TRIG 7

template <typename t>
void print(const char* a, t x)
{
	std::cout << "\n the " << a << " : " << x << "\n\n";
}

// const needed to calculate ln(x)
static double
ln2_hi = 6.93147180369123816490e-01,	/* 3fe62e42 fee00000 */
ln2_lo = 1.90821492927058770002e-10,	/* 3dea39ef 35793c76 */
L1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
L2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
L3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
L4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
L5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
L6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
L7 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */


// const needed to find exp(x)
static double
one = 1.0,
half[2] = { 0.5,-0.5, },
ln2HI[2] = { 6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */
		 -6.93147180369123816490e-01, },/* 0xbfe62e42, 0xfee00000 */
	ln2LO[2] = { 1.90821492927058770002e-10,  /* 0x3dea39ef, 0x35793c76 */
				-1.90821492927058770002e-10, },/* 0xbdea39ef, 0x35793c76 */
	invln2 = 1.44269504088896338700e+00, /* 0x3ff71547, 0x652b82fe */
	P1 = 1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
	P2 = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
	P3 = 6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
	P4 = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
	P5 = 4.13813679705723846039e-08; /* 0x3E663769, 0x72BEA4D0 */

float mexp(float x);
float mln(float x);
float msq(float base);
float mpower(float x, float y);
float minvsq(float number);
