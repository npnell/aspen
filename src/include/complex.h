#ifndef COMPLEX_H
#define COMPLEX_H

#include <cmath>

class complex
{
public:
	double re;
	double im;

	complex(double a = 0.0f, double b = 0.0f)
		: re(a), im(b) { }

	double mod();
};

inline double complex::mod() {
	return sqrt(re * re + im * im);
}

inline complex operator+(const complex& a, const complex& b)
{
	return complex(a.re + b.re, a.im + b.im);
}

inline complex operator-(const complex& a, const complex& b)
{
	return complex(a.re - b.re, a.im - b.im);
}

inline complex operator*(const complex& a, double b)
{
	return complex(a.re * b, a.im * b);
}

inline complex operator*(double a, const complex& b)
{
	return b * a;
}

inline complex operator*(const complex& a, const complex& b)
{
	return complex(a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re);
}

inline complex operator/(const complex& a, double b)
{
	return complex(a.re / b, a.im / b);
}

#endif // ! COMPLEX_H