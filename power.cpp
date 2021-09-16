#include"power.h"


float ln(float f)
{
	// https://www.h-schmidt.net/FloatConverter/IEEE754.html
	// im using IEEE 754 stds

	int k = *(int*)&f; // quake III FISR alogorithm to get exponent										
	k = k >> 23; // last 23 bits are significant of a float refer IEEE 754
	k -= 127; // IEEE754 2^exponent+127 (biased power of 2)

	int p = 1;
	p = p << k; // find 2^k by shifting 1 leftward by k 

	f = (f / p) - 1; // refer algorithm in sun microsystems

	float s = f / (2 + f);
	float hfsq = f * f * 0.5;

	float z = s * s;
	float w = z * z;
	float t1 = w * (L2 + w * (L4 + w * L6));
	float t2 = z * (L1 + w * (L3 + w * (L5 + w * L7)));

	float r = t1 + t2;

	return (float)k * (float)ln2HI - ((hfsq - (s * (hfsq + r) + (float)k * (float)ln2LO)) - f);
}

float epower(float x)
{
	float y, hi, lo, c, t;
	int k;

	k = (int)(invln2 * x + 0.5);  // refer algorithm in sun microsystems
	t = k;
	hi = x - t * (float)ln2HI;
	lo = t * (float)ln2LO;
	x = hi - lo;

	t = x * x;
	c = x - t * (P1 + t * (P2 + t * (P3 + t * (P4 + t * P5))));

	y = 1 - ((lo - (x * c) / (2.0 - c)) - hi);

	k = k << 23; // now we found exp(r) so we need to add k factor (2^k) to the exponent of float REFER IEEE 754
	int temp = *(int*)&y; // same quake III FISR methods
	temp = temp + k;		// just add k to the exponent
	y = *(float*)&temp;		// again convert to IEEE 754 float

	return y;

}

//float mypower(float x, float y)
//{
//	return epower(y * ln(x));
//}

float mypower(float x, float y)
{
	// x>1 always if x<1 ln return value in negative
	// epower cant handle negative values and a '0' value
	// y>1 always since ln(x)*y > ln 2
	if (x > 1 && y > 1)
	{
		float l = ln(x);
		l = l * y;
		if (l > ln2HI)
			return epower(l);
		else
			return -1;
	}
	else
		return -1;

}