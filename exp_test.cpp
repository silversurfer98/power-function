//g++ -c exp_test.cpp && g++ -o exp_test.exe exp_test.o && del exp_test.o
#include<iostream>

static double
one	= 1.0,
half[2]	= {0.5,-0.5,},
ln2HI[2]   ={ 6.93147180369123816490e-01,  /* 0x3fe62e42, 0xfee00000 */
	     -6.93147180369123816490e-01,},/* 0xbfe62e42, 0xfee00000 */
ln2LO[2]   ={ 1.90821492927058770002e-10,  /* 0x3dea39ef, 0x35793c76 */
	     -1.90821492927058770002e-10,},/* 0xbdea39ef, 0x35793c76 */
invln2 =  1.44269504088896338700e+00, /* 0x3ff71547, 0x652b82fe */
P1   =  1.66666666666666019037e-01, /* 0x3FC55555, 0x5555553E */
P2   = -2.77777777770155933842e-03, /* 0xBF66C16C, 0x16BEBD93 */
P3   =  6.61375632143793436117e-05, /* 0x3F11566A, 0xAF25DE2C */
P4   = -1.65339022054652515390e-06, /* 0xBEBBBD41, 0xC5D26BF1 */
P5   =  4.13813679705723846039e-08; /* 0x3E663769, 0x72BEA4D0 */

template <typename t>
void print(const char* a, t x)
{
	std::cout << "\n the " << a << " : " << x << "\n\n";
}


float exp(float x)
{
    float y;
    unsigned hx = *(unsigned int*)&x;
    int xsb = (hx>>31)&1;		/* sign bit of x */

	hx &= 0x7fffffff;		/* high word of |x| 0x7fffffff --> 0 11111111 1111111111111111111111 --> this masks out sign bit*/

    float hi, lo;
    int k;
    if(hx > 0x3eb17218) //0.5*ln2  but library used 0x3fd62e42
    {
        if(hx < 0x3f851592) //1.5*ln2  but library used 0x3FF0A2B2
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
            k = (int)(x*(float)invln2 + half[xsb]);

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
    if(k == 0)
        return (float)one-((x*c)/(c-2.0)-x);
    else
	    y = 1 - ((lo - (x * c) / (2.0 - c)) - hi);   

 // COMPUTATION OF EXP(r) ENDS --------------------

 // NOW WE NEED TO ADD EXPONENT k TO y based on sign bit

    int temp = *(int*)&y; // same quake III FISR methods
    temp = temp + (k<<23);

	y = *(float*)&temp;		// again convert to IEEE 754 float

    return y;

}
int main()
{
    std::cout<<" This is test to find -ve exp \n\n";
    float x;
    std::cout<<" enter the number : ";
    std::cin>>x;
    std::cout<<" \n\n The ans of Exp("<<x<<") is "<<exp(x)<<"\n\n";
    return 0;
}