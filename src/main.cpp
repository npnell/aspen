#include <string>
#include <thread>
#include <iomanip>

#include "window.h"
#include "shader.h"
#include "fft.h"
#include "utils.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const char* WINDOW_TITLE = "FFT";

static const struct {
	GLuint num_groups_x;
	GLuint num_groups_y;
	GLuint num_groups_z;
} dispatch_params = { 16, 16, 1 };

int initialize()
{
	WindowClass window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);

	Shader shader("shaders/compute.glsl");

	shader.use();

	unsigned int dispatch_buffer;
	glGenBuffers(1, &dispatch_buffer);

	glBindBuffer(GL_DISPATCH_INDIRECT_BUFFER, dispatch_buffer);
	glBufferData(GL_DISPATCH_INDIRECT_BUFFER, sizeof(dispatch_params), &dispatch_params, GL_STATIC_DRAW);

	glDispatchComputeIndirect(0);

	std::vector<char> buffer;
	if (read_file(buffer, "input.txt")) {
		exit(EXIT_FAILURE);
	}

	auto tokens = tokenize_buffer(buffer);

	unsigned int N = static_cast<unsigned int>(tokens.size());
	complex* x = new complex[N];
	for (unsigned int i = 0; i < N; ++i)
		x[i].re = tokens[i];

	fft(N, x);

	for (unsigned int i = 0; i < N; ++i)
		std::cout << std::fixed << std::setprecision(2) << x[i].re << " + " << x[i].im << "j\n";

	delete [] (x);
	
	glfwTerminate();
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	initialize();
}