#include <math.h>
#include <dos.h>
 
long  seed_ = 1l;      /* global seed value  */
 
double urand(void)
{
/*
 Portable random number generator, returns numbers between zero and one.
 This routine is small but approximates a uniform density fairly well.
 The ideal variance is 1/12, and the ideal mean is 1/2.
 
 The related function, uraninit(), may be called to control the global seed
 value, seed_.
 
 Calling urand() without first calling uraninit() gives the same random
 sequence as first calling uraninit(seed = 1) and then calling urand().
 
*/
    /* prime modulus multiplicative linear congruential   */
    /* random number generator                            */
    /* f(z) = (16807 * z) mod 2147483647;                 */
    /* Reference: Communications of the ACM vol 31 no 10  */
    extern long seed_;
    long low, high, test;
    high = seed_ / 127773l;    /* 127773 is 2147483647 / 16807 */
    low = seed_ % 127773l;
    test = 16807 * low - 2836l * high;   /* 2836 is 2147483647 % 16807 */
    if (test > 0l) seed_ = test;
    else seed_ = test + 2147483647l;
    return (double) seed_ / 2147483647.;
}
 
/* ------------------------------------------------------------ */
 
void uraninit(long seed)
{
/*
This routine initializes the seed for the urand() uniform number generator.
Its purpose is similar to that of the system srand() function.
 
  if (seed < 1)  then the global variable seed_ is chosen from the system clock
  if (seed >= 1)  then the global variable seed_ = seed
 
*/
   extern long seed_;
   union REGS r;
   union {
      long lval;
      struct {
         unsigned lsword;
         unsigned msword;
         } words;
      } val;
   if (seed < 1) {
      r.x.ax = 0x2c00;
      intdos(&r,&r);
      val.words.lsword = r.x.dx;
      val.words.msword = r.x.cx;
      seed_ = val.lval;
      }
   else seed_ = seed;
}
