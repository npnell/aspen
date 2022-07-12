#version 430 core

struct complex {
	float re;
	float im;
};

layout (local_size_x = 1, local_size_y = 1, local_size_z = 1) in;
layout (std430, binding = 0) buffer data {
	complex x[];
};

const float pi = 3.1415926;
const float twopi = 6.2831853;

complex addc(complex a, complex b)
{
	return complex( a.re + b.re, a.im + b.im );
}

complex subc(complex a, complex b)
{
	return complex( a.re - b.re, a.im - b.im );
}

complex mulc(complex a, complex b)
{
	return complex( a.re * b.re - a.im * b.im, a.re * b.im + a.im * b.re );
}

void butterfly(uint s)
{
	uint pos = gl_GlobalInvocationID.x;
	uint n = 1 << s;
	uint p = pos & (n - 1);
	float w = p * (twopi / n);
	if( p < n / 2) {
		const complex a = x[pos];
		const complex b = x[pos + n / 2];
		x[pos] = addc(a, b);
	}
	else {
		const complex Wn = complex( cos( w ), -sin( w ) ); 
		const complex b = x[pos];
		const complex a = x[pos - n / 2];
		x[pos] = mulc(subc(a, b), Wn);
	}
}

void main()
{
	for(int s = int(log2(x.length())); s >= 1; --s)
		butterfly(s);
}