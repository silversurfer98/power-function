#include "include.h"

float mexp(float x)
{
    float y;
    unsigned hx = *(unsigned int*)&x;
    int xsb = (hx >> 31) & 1;		/* sign bit of x */

    hx &= 0x7fffffff;		/* high word of |x| 0x7fffffff --> 0 11111111 1111111111111111111111 --> this masks out sign bit*/

    float hi, lo;
    int k;
    if (hx > 0x3eb17218) //0.5*ln2  but library used 0x3fd62e42
    {
        if (hx < 0x3f851592) //1.5*ln2  but library used 0x3FF0A2B2
        {
            hi = x - ln2HI[xsb];
            lo = ln2LO[xsb];
            k = 1 - xsb - xsb;
            //print("im here l2bound",k);
        }
        else
        {
            // ARGUMENT REDUCTION STARTS ---------------------------   
                // find exponent k such that x = k*ln2 + r where r<= 0.5*ln2
                // and follows mathematics
                // multiply by 1/ln2 -->  x/ln2 = k + r/ln2 
                // now r/ln2 <= 0.5
                // to always make that above statement true add 0.5 to x/ln2
            k = (int)(x * (float)invln2 + half[xsb]);

            // now find r which is x - k*ln2
            // to get better accuracy we split ln as ln2HI and ln2LO

            // ln2 = ln2HI + ln2LO
            // r = x - k*ln2HI - k*ln2LO

            hi = x - (float)k * (float)ln2HI[0];
            lo = (float)k * (float)ln2LO[0];

            //ARGUMENT REDUCTION ENDS -----------------------------
            //print("im here in core AR",k);
        }

        x = hi - lo;
    }
    else
        k = 0;


    // COMPUTATION OF EXP(r) BEGINS -------------------------

       /*   2. Approximation of exp(r) by a special rational function on
       *	the interval [0,0.34658]:
       *	Write
       *	    R(r**2) = r*(exp(r)+1)/(exp(r)-1) = 2 + r*r/6 - r**4/360 + ...
       *      We use a special Remes algorithm on [0,0.34658] to generate
       * 	a polynomial of degree 5 to approximate R. The maximum error
       *	of this polynomial approximation is bounded by 2**-59. In
       *	other words,
       *	    R(z) ~ 2.0 + P1*z + P2*z**2 + P3*z**3 + P4*z**4 + P5*z**5
       *  	(where z=r*r, and the values of P1 to P5 are listed below)
       *	and
       *	    |                  5          |     -59
       *	    | 2.0+P1*z+...+P5*z   -  R(z) | <= 2
       *	    |                             |
       *	The computation of exp(r) thus becomes
       *                             2*r
       *		exp(r) = 1 + -------
       *		              R - r
       *                                 r*R1(r)
       *		       = 1 + r + ----------- (for better accuracy)
       *		                  2 - R1(r)
       *	where
       *			         2       4             10
       *		R1(r) = r - (P1*r  + P2*r  + ... + P5*r   ). */

    float t = x * x;
    float c = x - t * ((float)P1 + t * ((float)P2 + t * ((float)P3 + t * ((float)P4 + t * (float)P5))));
    if (k == 0)
        return (float)one - ((x * c) / (c - 2.0) - x);
    else
        y = 1 - ((lo - (x * c) / (2.0 - c)) - hi);

    // COMPUTATION OF EXP(r) ENDS --------------------

    // NOW WE NEED TO ADD EXPONENT k TO y based on sign bit

    int temp = *(int*)&y; // same quake III FISR methods
    temp = temp + (k << 23);

    y = *(float*)&temp;		// again convert to IEEE 754 float

    return y;

}

float mln(float x)
{

    unsigned hx = *(int*)&x; // quake III FISR alogorithm

//------- check whether we can find ln ---------
// can be skipped if we are using ln for only positive nos	
    unsigned hxx = hx;
    int xsb = (hxx >> 31) & 1;		/* sign bit of x */
    if (hx == 0x00000000 || xsb == 1)
    {
        std::cerr << "\n loosu punda \n";
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

float mysq(float base)
{
    return mexp(0.5 * mln(base));
}