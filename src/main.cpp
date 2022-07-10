#include <string>
#include <thread>
#include <iomanip>

#include "window.h"
#include "shader.h"
#include "fft.h"
#include "utils.h"
#include "compute.h"

const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;
const char* WINDOW_TITLE = "FFT";

const GLuint num_groups_x = 10;
const GLuint num_groups_y = 1;
const GLuint num_groups_z = 1;

int initialize()
{
	WindowClass window(WINDOW_WIDTH, WINDOW_HEIGHT, WINDOW_TITLE);
	Shader shader("shaders/compute.glsl");

	std::vector<char> buffer;
	if (read_file(buffer, "input.txt")) {
		exit(EXIT_FAILURE);
	}
	auto tokens = tokenize_buffer(buffer);
	unsigned int n = static_cast<unsigned int>(tokens.size());

	complex* x = new complex[n];
	for (unsigned int i = 0; i < n; ++i)
		x[i].re = tokens[i];

	fft(n, x);

	for (unsigned int i = 0; i < n; ++i)
		std::cout << std::fixed << std::setprecision(2) << x[i].re << " + " << x[i].im << "j\n";

	delete[](x);

	Compute compute(num_groups_x, num_groups_y, num_groups_z);

	// Texture
	unsigned int out_tex;
	glGenTextures(1, &out_tex);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, out_tex);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	
	float values[10] = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };
	compute.set_values(values);
	glBindImageTexture(0, out_tex, 0, GL_FALSE, 0, GL_READ_WRITE, GL_R32F);

	shader.use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, out_tex);
	compute.dispatch();
	compute.wait();

	auto data = compute.get_values();

	for (auto d : data)
		std::cout << d << " ";
	std::cout << std::endl;

	glDeleteProgram(shader.ID);
	
	glfwTerminate();
	return EXIT_SUCCESS;
}

int main(int argc, char* argv[])
{
	initialize();
}