#ifndef FFT_H

#include <algorithm>
#include <iterator>

#include "complex.h"

const double pi{ 3.1415926535897932384626433832795 };
const double twopi{ 6.283185307179586476925286766559 };

unsigned int reverse(unsigned int x, unsigned int m)
{
	int _x = 0;
	while (m--) {
		_x = (x & 0x1) | (_x << 1);
		x = x >> 1;
	}
	return _x;
}

void butterfly(int n, int q, complex* x) // Butterfly operation
// n : sequence length
// q : block start point
// x : input/output squence
{
	const int m = n / 2;
	const double theta0 = twopi / n;

	if (n > 1) {
		for (int p = 0; p < m; p++) {
			const complex wp = complex(cos(p * theta0), -sin(p * theta0));
			const complex a = x[q + p + 0];
			const complex b = x[q + p + m];
			x[q + p + 0] = a + b;
			x[q + p + m] = (a - b) * wp;
		}
		butterfly(n / 2, q + 0, x);
		butterfly(n / 2, q + m, x);
	}
}

void fft(int n, complex* x) // Fourier transform
// n : sequence length
// x : input/output sequence
{
	int m = static_cast<int>(log2(n));
	complex* _x = new complex[n];
	butterfly(n, 0, x);
	for (int i = 0; i < n; ++i)
		_x[reverse(i, m)] = x[i];
	std::copy(_x, &_x[n] + 1, x);
	delete [] (_x);
}

#endif // ! FFT_H