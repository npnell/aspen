#ifndef FFT_H

#include <algorithm>
#include <iterator>

#include "complex.h"

const double pi{ 3.1415926535897932384626433832795 };
const double twopi{ 6.283185307179586476925286766559 };

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

void bit_reverse(int n, complex* x) // Bitreversal operation
// n : squence length
// x : input/output sequence
{
	for (int i = 0, j = 1; j < n - 1; j++) {
		for (int k = n >> 1; k > (i ^= k); k >>= 1);
		if (i < j) std::swap(x[i], x[j]);
	}
}

void fft(int n, complex* x) // Fourier transform
// n : sequence length
// x : input/output sequence
{
	butterfly(n, 0, x);
	bit_reverse(n, x);
}

#endif // ! FFT_H