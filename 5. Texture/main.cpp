#include "LoadShaders.h"
#include <iostream>
#include <stdio.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png.h>

#define WIDTH 1366
#define HEIGHT 768

typedef struct PixelColorStruct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} PixelColor;

GLuint VAO;
GLuint buffer;
GLuint element_array_buffer;
GLuint tex;

bool readPngRows(png_bytep* &rows, int w, int h)
{
	FILE *fp = fopen("Saturn.png", "rb");

	png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if(!png)
	{
		fclose(fp);
		return false;
	}

	png_infop info = png_create_info_struct(png);
	if(!info) abort();

	if(setjmp(png_jmpbuf(png))) abort();

	png_init_io(png, fp);

	png_read_info(png, info);

	int width      = png_get_image_width(png, info);
	int height     = png_get_image_height(png, info);
	png_byte color_type = png_get_color_type(png, info);
	png_byte bit_depth  = png_get_bit_depth(png, info);

	assert(width == w && height == h);

	if(bit_depth == 16)
	    png_set_strip_16(png);

	if(color_type == PNG_COLOR_TYPE_PALETTE)
	png_set_palette_to_rgb(png);

	// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
	if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
	png_set_expand_gray_1_2_4_to_8(png);

	if(png_get_valid(png, info, PNG_INFO_tRNS))
	png_set_tRNS_to_alpha(png);

	// These color_type don't have an alpha channel then fill it with 0xff.
	if(color_type == PNG_COLOR_TYPE_RGB ||
	 color_type == PNG_COLOR_TYPE_GRAY ||
	 color_type == PNG_COLOR_TYPE_PALETTE)
	png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

	if(color_type == PNG_COLOR_TYPE_GRAY ||
	 color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
	png_set_gray_to_rgb(png);

	png_read_update_info(png, info);

	rows = (png_bytep*)malloc(sizeof(png_bytep) * height);
	for(int y = 0; y < height; y++) {
		rows[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
	}

	png_read_image(png, rows);

	fclose(fp);

	return true;
}

void readColors(png_bytep* &rows, PixelColor* &colors, int width, int height)
{
	for(int y = 0; y < height; y++) {
		png_bytep row = rows[y];
		for(int x = 0; x < width; x++) {
		  png_bytep px = &(row[x * 4]);

		  colors[(height - 1 - y) * width + x].r = px[0];
		  colors[(height - 1 - y) * width + x].g = px[1];
		  colors[(height - 1 - y) * width + x].b = px[2];
		  colors[(height - 1 - y) * width + x].a = px[3];
		}
	}
}

void generateAndFIllTexture(GLuint &tex)
{
	glActiveTexture(GL_TEXTURE0);
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); //base mipmap level
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0); //max mipmap levels
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	png_bytep *rows;
	if (!readPngRows(rows, WIDTH, HEIGHT))
	{
		std::cerr << "Cannot read PNG file";
		exit(EXIT_FAILURE);
	}

	PixelColor *colors = new PixelColor[WIDTH * HEIGHT];

	readColors(rows, colors, WIDTH, HEIGHT);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, 1366, 768, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors);

	delete colors;
}

void init(void) {
	ShaderInfo shaders[] = { { GL_VERTEX_SHADER, "./triangles.vert" }, {
			GL_FRAGMENT_SHADER, "./triangles.frag" }, { GL_NONE, NULL } };

	GLuint program = LoadShaders(shaders);
	if (program == 0) {
		return;
	}
	glUseProgram(program);

	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint indexes[4] = {0, 1, 2, 3};

	glGenBuffers(1, &element_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	GLfloat vertices[4][3] = { {-6.83, -3.84, 0.0}
	, {-6.83, 3.84, 0.0}
	, {6.83, 3.84, 0.0}
	, {6.83, -3.84, 0.0}};

	GLfloat tex_positions[4][2] = { {0.0, 0.0}
	, {0.0, 1.0}
	, {1.0, 1.0}
	, {1.0, 0.0}};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(tex_positions), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(tex_positions), tex_positions);

	int vertex_loc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vertex_loc);

	int tex_loc = glGetAttribLocation(program, "in_tex_coord");
	glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vertices));
	glEnableVertexAttribArray(tex_loc);

	glm::vec3 eye(0.0, -10.0, 10.0);
	glm::vec3 center(0.0, 0.0, 0.0);
	glm::vec3 up(0.0, 0.0, 1.0);
	glm::mat4 view_mat = glm::lookAt(eye, center, glm::normalize(up));

	glm::mat4 mvp_mat = glm::perspective(45.0f, 16.0f/9.0f, 1.0f, 100.0f) * view_mat;

	GLuint mvp_matr_loc = glGetUniformLocation(program, "mvp_matrix");

	glUniformMatrix4fv(mvp_matr_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));

	generateAndFIllTexture(tex);

	GLint tex1_uniform_loc = glGetUniformLocation(program, "tex");
	glUniform1i(tex1_uniform_loc, 0);
//	glActiveTexture(GL_TEXTURE0);
//	glBindTexture(GL_TEXTURE_2D, tex);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT,
			(const void*) (0 * sizeof(GLuint)));
	glFlush();
}

int main(int argc, char** argv) {
	printf("%i", GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(1280, 720);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);

	glewExperimental = GL_TRUE;

	if (glewInit()) {
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutMainLoop();

	glDeleteTextures(1, &tex);
}
