#pragma once

#define NTAB 32
#define IA 16807
#define IM 2147483647
#define IQ 127773
#define IR 2836
#define NDIV (1+(IM-1)/NTAB)
#define MAX_RANDOM_RANGE 0x7FFFFFFFUL

// fran1 -- return a random floating-point number on the interval [0,1)
//
#define AM (1.0/IM)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)



class c_uniform_random_stream
{
public:
	virtual void	set_seed(int iSeed) = 0;
	virtual float	random_float(float flMinVal = 0.0f, float flMaxVal = 1.0f) = 0;
	virtual int		random_int(int iMinVal, int iMaxVal) = 0;
	virtual float	random_float_exp(float flMinVal = 0.0f, float flMaxVal = 1.0f, float flExponent = 1.0f) = 0;
public:
	int generate_random_number();
	int m_idum;
	int m_iy;
	int m_iv[NTAB];

	char PAD0[4];
};