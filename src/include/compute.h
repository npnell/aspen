#ifndef COMPUTE_H
#define COMPUTE_H

#include <glad/glad.h>

#include <vector>

class Compute {
public:
	const unsigned int num_groups_x;

	Compute(unsigned int num_groups_x)
		: num_groups_x(num_groups_x) { }

	inline void dispatch();
	inline void wait();
	inline void set_values(unsigned int, float*);
	std::vector<float> get_values(GLenum, unsigned int);
};

inline void Compute::dispatch()
{
	glDispatchCompute(num_groups_x, 1, 1);
}

inline void Compute::wait()
{
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

inline void Compute::set_values(unsigned int buffer_length, float* data)
{
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(float) * buffer_length, data, GL_STATIC_COPY);
}

std::vector<float> Compute::get_values(GLenum target, unsigned int buffer_length)
{
	std::vector<float> compute_data(buffer_length);
	glGetBufferSubData(target, 0, sizeof(float) * buffer_length, compute_data.data());
	return compute_data;
}

#endif COMPUTE_H