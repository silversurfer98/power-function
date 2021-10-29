//g++ -c ln_test.cpp && g++ -o ln_test.exe ln_test.o && del ln_test.o
#include<iostream>

static double
ln2_hi  =  6.93147180369123816490e-01,	/* 3fe62e42 fee00000 */
ln2_lo  =  1.90821492927058770002e-10,	/* 3dea39ef 35793c76 */
L1 = 6.666666666666735130e-01,  /* 3FE55555 55555593 */
L2 = 3.999999999940941908e-01,  /* 3FD99999 9997FA04 */
L3 = 2.857142874366239149e-01,  /* 3FD24924 94229359 */
L4 = 2.222219843214978396e-01,  /* 3FCC71C5 1D8E78AF */
L5 = 1.818357216161805012e-01,  /* 3FC74664 96CB03DE */
L6 = 1.531383769920937332e-01,  /* 3FC39A09 D078C69F */
L7 = 1.479819860511658591e-01;  /* 3FC2F112 DF3E5244 */


template <typename t>
void print(const char* a, t x)
{
	std::cout << "\n the " << a << " : " << x << "\n\n";
}


float ln(float x)
{ 

	unsigned hx = *(int*)&x; // quake III FISR alogorithm
	
//------- check whether we can find ln ---------
// can be skipped if we are using ln for only positive nos	
	unsigned hxx = hx;
	int xsb = (hxx>>31)&1;		/* sign bit of x */
	if(hx == 0x00000000 || xsb ==1)
	{
		std::cerr<<"\n loosu punda \n";
		exit(0);
	}
//--------------------------------------------------

/*	Argument Reduction: find k and f such that 
 *			x = 2^k * (1+f), 
 *	   where  sqrt(2)/2 < 1+f < sqrt(2) .*/

	int k = hx;
	k = k >> 23; // last 23 bits are significant of a float refer IEEE 754
	k -= 127; // IEEE754 2 ^ exponent + 127 (biased power of 2)
	//print("k is",k);

    hx = (hx & 0x7FFFFF) + 0x3F800000;  //0x7FFFFF --> masks out exponent and add 0x3F800000 --> which is 2^0 to get the mantissa
    x = *(float*)&hx;
	//print("mantissa is",x);

//------------ tedious algorithm starts here i have not considered if's, gone for maximum precision

/*   2. Approximation of log(1+f).
 *	Let s = f/(2+f) ; based on log(1+f) = log(1+s) - log(1-s)
 *		 = 2s + 2/3 s**3 + 2/5 s**5 + .....,
 *	     	 = 2s + s*R
 *      We use a special Reme algorithm on [0,0.1716] to generate 
 * 	a polynomial of degree 14 to approximate R The maximum error 
 *	of this polynomial approximation is bounded by 2**-58.45. In
 *	other words,
 *		        2      4      6      8      10      12      14
 *	    R(z) ~ Lg1*s +Lg2*s +Lg3*s +Lg4*s +Lg5*s  +Lg6*s  +Lg7*s
 *  	(the values of Lg1 to Lg7 are listed in the program)
 *	and
 *	    |      2          14          |     -58.45
 *	    | Lg1*s +...+Lg7*s    -  R(z) | <= 2 
 *	    |                             |
 *	Note that 2s = f - s*f = f - hfsq + s*hfsq, where hfsq = f*f/2.
 *	In order to guarantee error in log below 1ulp, we compute log
 *	by
 *		log(1+f) = f - s*(f - R)	(if f is not too large)
 *		log(1+f) = f - (hfsq - s*(hfsq+R)).	(better accuracy)
 *	
 *	3. Finally,  log(x) = k*ln2 + log(1+f).  
 *			    = k*ln2_hi+(f-(hfsq-(s*(hfsq+R)+k*ln2_lo)))
 *	   Here ln2 is split into two floating point number: 
 *			ln2_hi + ln2_lo,
*/
	float f = x - 1.0;
	float s = f / (2.0 + f);
	float hfsq = f * f * 0.5;

	float z = s * s;
	float w = z * z;
	float t1 = w * (L2 + w * (L4 + w * L6));
	float t2 = z * (L1 + w * (L3 + w * (L5 + w * L7)));

	float r = t1 + t2;
	float kk = (float)k;

	return kk * (float)ln2_hi - ((hfsq - (s * (hfsq + r) + kk * (float)ln2_lo)) - f);

// ----------------- And it all ends here --------------------------

}
int main()
{
    std::cout<<" This is test to find ln \n\n";
    float x;
    std::cout<<" enter the number : ";
    std::cin>>x;
    std::cout<<" \n\n The ans of ln("<<x<<") is "<<ln(x)<<"\n\n";
    return 0;
}