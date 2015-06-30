/* -------------------------------------------------------------------

	Title:
		WMPolyFit

	Description:
		Polymomial fit routine using Singular Value Decomposition (SVD).  
		Adapted from "Numerical Recipes" by W.H.Press. 

	Author:
		Stephen C. Wardlaw, M.D.
		Yale University School Of Medicine
		462-CB  Y-NHH
		20 York St.
		New Haven, CT  06504

	Edit History:
		27 December, 1997
			Adapted for BeOS
		20 Jan, 94
			Re-written for MetroWerks C++
		19 Mar, 92
			Recompiled under MW Vers 4.0
		08 Aug, 91
			Adapted to Macintosh model
		28 Mar, 89
			First IBM version

------------------------------------------------------------------- */

#include "WMPolyFit.h"
#include <math.h>
#include <stdio.h>

// -------------------------------------------------------------------
//	*	Public Methods
// -------------------------------------------------------------------

WMPolyFit::WMPolyFit(DataFitSet* dArray, int maxData) 
{
	mFitData = dArray;
	mMaxArray = maxData - 1;
	mReqParams = 3;
	mDataPts = 0;
	mFitted = false;
}
// -------------------------------------------------------------------
//	*	Public Methods
// -------------------------------------------------------------------
// func: Return values of the basis functions at x.
//       Using each basis function X[jj](), the jjth element of p is
//       set to X[jj](x).  e.g. if x=2, then for basis functions
//            1, x, x**2,..., x**jj,...,x**(reqParams-1)
//            p = 1, 2, 4,...,2**jj,...,2**(reqParams-1)
// NOTE! This may be overridden to provide non-power functions
void
WMPolyFit::BasisFunc(float x, Params &p)
{
	int jj;
	
	p[1] = 1.0;
	for(jj = 2; jj <= mReqParams; jj++) {
		p[jj] = p[jj - 1] * x;
		}
}
// -------------------------------------------------------------------
// Returns the Y value calculated from X using the current reqParams.
float
WMPolyFit::CalcYFromX(float x)	
{
	float sum;
	int n;
	Params afunct;

	mMathErr = 0;
	
	if(!mFitted) {
		mMathErr = 1;
		return 0;
	}
	BasisFunc(x,afunct);
	sum = 0.0;
	for(n = 1; n <= mReqParams; n++) {
		sum += pArray[n]*afunct[n];
	}
	return sum;
}
// -------------------------------------------------------------------
// Returns the Chi Square value of the fit.
float
WMPolyFit::ChiSqValue()	{

	float chiSq;

	if(!mFitted) {
		mMathErr = 1;
		return 0;
	}
	
	chiSq = 0.0;
	for(int n = 0; n <= mMaxArray; n++) {
		if(mFitData[n].data.use) {
			chiSq += sqrt(fabs((mFitData[n].data.y - CalcYFromX(mFitData[n].data.x))/mFitData[n].sig));
		}
	}
	return chiSq;
}
// -------------------------------------------------------------------
// Fits data for the number of parameters requested
void
WMPolyFit::FitData()	{

	const float	tol = 1.0E-5;

	int 		n,jj,ii,index;
	float	wmax,tmp,thresh;
	DataArr	b;
	Params	afunct;

	mFitted = false;
	mMathErr = 1;
	
	for(n = 0; n <= k_maxFitParams; n++) {
		pArray[n] = 0;
	}
	ii = 0;
	mDataPts = 0;
	index = 1;
	while(ii <= mMaxArray) {
		if(mFitData[ii].data.use && index < k_maxFitData) {
			BasisFunc(mFitData[ii].data.x,afunct);
			tmp = 1.0/mFitData[ii].sig;
			for(jj = 1; jj <= mReqParams; jj++) {
				u[index][jj] = afunct[jj]*tmp;
			}
			b[index] = mFitData[ii].data.y*tmp;
			mDataPts++;
			index++;
		}
		ii++;
	}	// while ii
	if(mDataPts < mReqParams) {
		return;	// *** EXIT HERE if not enough data
	}
	SVDecomp(mDataPts,mReqParams);
	wmax = 0.0;
	for(jj = 1; jj <= mReqParams; jj++) {
		if(w[jj] > wmax) {
			wmax = w[jj];
		}
	}
	thresh = tol*wmax;
	for(jj = 1; jj <= mReqParams; jj++) {
		if(w[jj] < thresh) {
			w[jj] = 0.0;
		}
	}
	SVBackSub(mDataPts,mReqParams,b);
	mFitted = true;
	mMathErr = 0;
}
// -------------------------------------------------------------------
// Gives the covariance array of the fit.
void
WMPolyFit::GetCovarData(Covar &covar)	
{

}
// -------------------------------------------------------------------
// Gives the parameters of the fit.
void
WMPolyFit::GetFitParams(Params &reqData)	
{
	int n;
	
	if(mFitted) {
		for(n = 0; n <= mReqParams; n++) {
			reqData[n] = pArray[n];
		}
		mMathErr = 0;
	} else {
		mMathErr = 1;
	}
}
// -------------------------------------------------------------------
// Gets the current number of fit parameters.
int
WMPolyFit::NumParamsFitted()	
{
	return mReqParams;
}
// -------------------------------------------------------------------
// Sets the number of parameters in the fit (default is 3).
void
WMPolyFit::SetNumParameters(int numParams) 
{

	mReqParams = numParams > k_maxFitParams ? k_maxFitParams : numParams;
	if(!numParams) 
		mReqParams = 1;
}
// -------------------------------------------------------------------
//	*	Private Methods
// -------------------------------------------------------------------
float
WMPolyFit::sign(float a, float b)
{
	if(b >= 0.0) {
		return fabs(a);
	} else {
		return -fabs(a);
	}
}
// -------------------------------------------------------------------
// SVBackSub: Singular Value Decomposition -- Back-substitution
void
WMPolyFit::SVBackSub(int dataPts, int numParams, DataArr &b)
{
	int jjj,jj,ii;
	float s;
	Params tmp;
   
	for(jj = 1; jj <= numParams; jj++) {
		s = 0.0;
		if(w[jj] != 0.0) {
			for(ii = 1; ii <= dataPts; ii++) {
				s += u[ii][jj]*b[ii];
				}
			s = s/w[jj];
			}
		tmp[jj] = s;
		}
	for(jj = 1; jj <= numParams; jj++) {
		s = 0.0;
		for(jjj = 1;jjj <= numParams; jjj++) {
			s += v[jj][jjj]*tmp[jjj];
			} 
		pArray[jj] = s;
		}
}

// -------------------------------------------------------------------
// Procedure for single value decomposition.
void
WMPolyFit::SVDecomp(int dataPts, int numParam)
{
	const int k_NMax = 100;

	int nm,indx,kk,jj,jjj,its,ii;
	float z,y,x,scale,s,h,g,f,c,anorm;
	float rvi[k_NMax + 1];

		// -----------------------------------------
		// Householder reduction to bidiagonal form
		// -----------------------------------------
	g = 0.0;
	scale = 0.0;
	anorm = 0.0;
	for(ii = 1; ii <= numParam; ii++) {
		indx = ii+1;
		rvi[ii] = scale*g;
		g = 0.0;
		s = 0.0;
		scale = 0.0;
		if(ii <= dataPts) {
			for(kk = ii; kk <= dataPts; kk++) {
				scale += fabs(u[kk][ii]);
			}
			if(scale != 0.0) {
				for(kk = ii; kk <= dataPts; kk++) {
					u[kk][ii] = u[kk][ii]/scale;
					s += u[kk][ii]*u[kk][ii];
				}  // for kk
				f = u[ii][ii];
				g = -sign(sqrt(s),f);
				h = f*g - s;
				u[ii][ii] = f - g;
				if(ii != numParam) {
					for(jj = indx; jj <= numParam; jj++) {
						s = 0.0;
						for(kk = ii; kk <= dataPts; kk++) {
							s += u[kk][ii]*u[kk][jj];
						}
						f = s/h;
						for(kk = ii; kk <= dataPts; kk++) {
							u[kk][jj] += f*u[kk][ii];
						}
					}	// for jj
				}	// if ii 
				for(kk = ii; kk <= dataPts; kk++) {
					u[kk][ii] = scale*u[kk][ii];
				} 
			}	// if scale
		}	// if ii < dataPts
		w[ii] = scale*g;
		g = 0.0;
		s = 0.0;
		scale = 0.0;
		if((ii <= dataPts) && (ii != numParam)) {
			for(kk = indx; kk <= numParam; kk++) {
				scale += fabs(u[ii][kk]);
			}
			if(scale != 0.0) {
				for(kk = indx; kk <= numParam; kk++) {
					u[ii][kk] = u[ii][kk]/scale;
					s += u[ii][kk]*u[ii][kk];
				}	// for kk
				f = u[ii][indx];
				g = -sign(sqrt(s),f);
				h = f*g - s;
				u[ii][indx] = f - g;
				for(kk = indx; kk <= numParam; kk++) {
					rvi[kk] = u[ii][kk]/h;
				}
				if(ii != dataPts) {
					for(jj = indx; jj <= dataPts; jj++) {
						s = 0.0;
						for(kk = indx; kk <= numParam; kk++) {
							s += u[jj][kk]*u[ii][kk];
						}
						for(kk = indx; kk <= numParam; kk++) {
							u[jj][kk] += s*rvi[kk];
						}
					}	// for jj
				}	// if ii
				for(kk = indx; kk <= numParam; kk++) {
					u[ii][kk] = scale*u[ii][kk];
				}
			}	// if scale
		}	// if ii
		anorm = max(anorm,(fabs(w[ii]) + fabs(rvi[ii])));
	}	// for ii
	   
		// -----------------------------------------
		// Accumulation of right-hand transformation
		// -----------------------------------------

	for(ii = numParam; ii >= 1; ii--) {
		if(ii < numParam) {
			if(g != 0.0) {
				for(jj = indx; jj <= numParam; jj++) {
					v[jj][ii] = (u[ii][jj]/u[ii][indx])/g;
				}
				for(jj = indx; jj <= numParam; jj++) { 
					s = 0.0;
					for(kk = indx; kk <= numParam; kk++) { 
	               s += u[ii][kk]*v[kk][jj];
					}
					for(kk = indx; kk <= numParam; kk++) {
						v[kk][jj] += s*v[kk][ii];
					}
				}	// for jj
			}	// if g
			for(jj = indx; jj <= numParam; jj++) {
				v[ii][jj] = 0.0;
				v[jj][ii] = 0.0;
			}	// for jj
		}	// if ii
		v[ii][ii] = 1.0;
		g = rvi[ii];
		indx = ii;
	}	// for ii

		// -----------------------------------------
		// Accumulation of left-hand transformation
		// -----------------------------------------

	for(ii = numParam; ii >= 1; ii--) {
		indx = ii + 1;
		g = w[ii];
		if(ii < numParam) {
			for(jj = indx; jj <= numParam; jj++) {
	      	u[ii][jj] = 0.0;
			}
		}
		if(g != 0.0) {
	      g = 1.0/g;
			if(ii != numParam) {
				for(jj = indx; jj <= numParam; jj++) {
					s = 0.0;
					for(kk = indx; kk <= dataPts; kk++) {
	               s += u[kk][ii]*u[kk][jj];
					}
					f = (s/u[ii][ii])*g;
					for(kk = ii; kk <= dataPts; kk++) {
						u[kk][jj] += f * u[kk][ii];
					}
				}	// for jj
			}	// if ii
			for(jj = ii; jj <= dataPts; jj++) {
				u[jj][ii] = u[jj][ii] * g;
			}
		} else {	// If g is zero
			for(jj = ii; jj <= dataPts; jj++) {
				u[jj][ii] = 0.0;
			}
		}
		u[ii][ii]++;
	}	//  for ii
	
		// -----------------------------------------
		//  Diagonalization of the bidiagonal form
		// -----------------------------------------
		
	for(kk = numParam; kk >= 1; kk--) { // Loop over singular values
		for(its = 1; its <= 30; its++) { // Loop over allowed iterations
			indx = kk + 1;
			do {	// Test for splitting
				indx--;
				nm = indx - 1;      // Note that rvi[1] = 0
			}
			while (((fabs(rvi[indx]) + anorm) != anorm)
						&& ((fabs(w[nm]) + anorm) != anorm)
						&& (indx != 1));
			if(((fabs(rvi[indx]) + anorm) != anorm)) {
				c = 0.0;            //Cancelation of rvi[indx] if indx>1
				s = 1.0;
				for(ii = indx; ii <= kk; ii++) {
					f = s * rvi[ii];
					if((fabs(f) + anorm) != anorm) {
						g = w[ii];
						h = sqrt(f*f + g*g);
						w[ii] = h;
						h = 1.0/h;
						c = g*h;
						s = -(f*h);
						for(jj = 1; jj <= dataPts; jj++) {
							y = u[jj][nm];
							z = u[jj][ii];
							u[jj][nm] = (y*c) + (z*s);
							u[jj][ii] = -(y*s) + (z*c);
						}	// for jj
					}	// if fabs(f)
				}	// for ii
			}	// if (!((fabs(rvi[ etc.
			z = w[kk];
			if(indx == kk) {	// Convergence
				if(z < 0.0) {	// Singular value is made non-negative
					w[kk] = -z;
					for(jj = 1; jj <= numParam; jj++) {
						v[jj][kk] = -v[jj][kk];
					}
				}	// if z
				its = 30;
			} else {  // If indx != kk
				if(its == 30) {
					mMathErr = 1; // ERROR! No Fit
				}
				x = w[indx];   // Shift from bottom 2-by-2 minor
				nm = kk - 1;
				y = w[nm];
				g = rvi[nm];
				h = rvi[kk];
				f = ((y - z)*(y + z)+(g - h)*(g + h))/(2.0*h*y);
				g = sqrt(f*f + 1.0);
				f = ((x - z)*(x + z) + h*((y/(f + sign(g,f))) - h))/x;
				
				// Next QR transformation
				c = 1.0;
				s = 1.0;
				for(jj = indx; jj <= nm; jj++) {
					ii = jj + 1;
					g = rvi[ii];
					y = w[ii];
					h = s*g;
					g = c*g;
					z = sqrt(f*f + h*h);
					rvi[jj] = z;
					c = f/z;
					s = h/z;
					f = (x*c) + (g*s);
					g = (g*c) - (x*s);
					h = y*s;
					y = y*c;
					for(jjj = 1; jjj <= numParam; jjj++) {
						x = v[jjj][jj];
						z = v[jjj][ii];
						v[jjj][jj] = x*c + z*s;
						v[jjj][ii] = z*c - x*s;
					}	// for jjj
					z = sqrt(f*f + h*h);
					w[jj] = z;   // Rotation can be arbitrary if z = 0
					if(z != 0.0) {
						z = 1.0/z;
						c = f*z;
						s = h*z;
					}
					f = c*g + s*y;
					x = c*y - s*g;
					for(jjj = 1; jjj <= dataPts; jjj++) {
						y = u[jjj][jj];
						z = u[jjj][ii];
						u[jjj][jj] = y*c + z*s;
						u[jjj][ii] = z*c - y*s;
					}	// for jjj
				}	// for jj
				rvi[indx] = 0.0;
				rvi[kk] = f;
				w[kk] = x;
			}	// else indx != kk
		}	// for its
	}	// for kk
} // SVDecomp
