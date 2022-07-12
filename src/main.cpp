#include <iomanip>

#include "window.h"
#include "shader.h"
#include "utils.h"
#include "compute.h"
#include "complex.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const char* WINDOW_TITLE = "FFT";

const unsigned int num_groups_x = 8;
const unsigned int buffer_length = 8;

void bit_reverse(unsigned int n, complex* x)
{
	for (unsigned int i = 0, j = 1; j < n - 1; j++) {
		for (unsigned int k = n >> 1; k > (i ^= k); k >>= 1);
		if (i < j) std::swap(x[i], x[j]);
	}
}

int initialize()
{
	WindowClass window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	Shader shader("shaders/compute.glsl");
	Compute compute(num_groups_x);

	std::vector<char> buffer;
	if (read_file(buffer, "input.txt")) {
		exit(EXIT_FAILURE);
	}
	auto tokens = tokenize_buffer(buffer);
	complex* x = (complex*)tokens.data();

	shader.use();

	unsigned int ssbo;
	glGenBuffers(1, &ssbo);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, ssbo);
	compute.set_values(buffer_length * 2, (float *)x);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, ssbo);

	compute.dispatch();
	compute.wait();
	memcpy(x, compute.get_values(GL_SHADER_STORAGE_BUFFER, buffer_length * 2).data(), sizeof(float) * buffer_length * 2);
	
	bit_reverse(buffer_length, x);

	std::stringstream output_stream;
	for (unsigned int i = 0; i < buffer_length; ++i)
		output_stream << std::fixed << std::setprecision(2) << x[i].re << " + " << x[i].im << "j\n";
	
	if (write_file(output_stream.str(), "output.txt")) {
		exit(EXIT_FAILURE);
	}

	shader.del();

	glfwTerminate();
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	if (!initialize())
		std::cout << "done.\n";
}