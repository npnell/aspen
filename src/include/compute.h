#ifndef COMPUTE_H
#define COMPUTE_H

#include <glad/glad.h>

#include <vector>

class Compute
{
public:
	const GLuint num_groups_x;
	const GLuint num_groups_y;
	const GLuint num_groups_z;

	Compute(GLuint num_groups_x, GLuint num_groups_y, GLuint num_groups_z)
		: num_groups_x(num_groups_x), num_groups_y(num_groups_y), num_groups_z(num_groups_z) { }

	inline void dispatch();
	inline void wait();
	inline void set_values(float*);
	std::vector<float> get_values();
};

inline void Compute::dispatch()
{
	glDispatchCompute(num_groups_x, num_groups_y, num_groups_z);
}

inline void Compute::wait()
{
	glMemoryBarrier(GL_ALL_BARRIER_BITS);
}

inline void Compute::set_values(float* values)
{
	glTexImage2D(GL_TEXTURE_2D, 0, GL_R32F, num_groups_x, num_groups_y, 0, GL_RED, GL_FLOAT, values);
}

std::vector<float> Compute::get_values()
{
	unsigned int collection_size = num_groups_x * num_groups_y;
	std::vector<float> compute_data(collection_size);
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RED, GL_FLOAT, compute_data.data());
	return compute_data;
}

#endif COMPUTE_H