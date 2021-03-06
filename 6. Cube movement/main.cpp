#include "LoadShaders.h"
#include <iostream>
#include <stdio.h>
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <png.h>
#include <ctime>
#include <cmath>
#include <sys/time.h>
#include <string>

#define WIDTH 1366
#define HEIGHT 768
#define SPEED 3500.0f
#define CAMERA_SPEED 8000.0f

#define FPS_RENEW_CYCLE 1000.0f //in miliseconds

typedef struct PixelColorStruct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} PixelColor;

unsigned long prevTime;
int framesRendered = 0;

GLuint floorProgram;
GLuint cubeProgram;

GLuint VAO;
GLuint buffer;
GLuint element_array_buffer;
GLuint tex;

GLuint cubeVAO;
GLuint cubeBuffers[6];
GLuint cubeElArrayBuffer;
GLuint cubeTextures[6];

bool readPngRows(png_bytep* &rows, int w, int h)
{
	FILE *fp = fopen("Floor.png", "rb");

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
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

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

void initFloor()
{
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	GLuint indexes[4] = {0, 1, 2, 3};

	glGenBuffers(1, &element_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glGenBuffers(1, &buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	GLfloat vertices[4][3] = { {-8.0, -4.5, 0.0}
	, {-8.0, 4.5, 0.0}
	, {8.0, 4.5, 0.0}
	, {8.0, -4.5, 0.0}};

	GLfloat tex_positions[4][2] = { {0.0, 0.0}
	, {0.0, 1.0}
	, {1.0, 1.0}
	, {1.0, 0.0}};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(tex_positions), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(tex_positions), tex_positions);

	glActiveTexture(GL_TEXTURE0);
	generateAndFIllTexture(tex);
}

void initCube()
{
	glGenVertexArrays(1, &cubeVAO);
	glBindVertexArray(cubeVAO);

	GLuint indexes[4] = {0, 1, 2, 3};
	glGenBuffers(1, &cubeElArrayBuffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeElArrayBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glGenBuffers(6, cubeBuffers);

	GLfloat vertices[24][3] =
	{
		{-1.0f, -1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, -1.0f, 1.0f},
		{-1.0f, -1.0f, -1.0f}, {-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, -1.0f}, {1.0f, -1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, 1.0f}, {1.0f, -1.0f, -1.0f},
		{-1.0f, 1.0f, -1.0f}, {-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, -1.0f},
		{-1.0f, -1.0f, -1.0f}, {-1.0f, -1.0f, 1.0f}, {-1.0f, 1.0f, 1.0f}, {-1.0f, 1.0f, -1.0f},
		{1.0f, -1.0f, -1.0f}, {1.0f, -1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, -1.0f},
	};

	GLfloat normals[24][3] =
	{
		{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, -1.0},
		{0.0, -1.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, -1.0, 0.0},
		{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
		{-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0},
		{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}
	};

	GLfloat tex_positions[4][2] = { {0.0, 0.0}
		, {0.0, 1.0}
		, {1.0, 1.0}
		, {1.0, 0.0}};

	for (int i = 0; i < 6; ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, cubeBuffers[i]);

		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 + sizeof(GLfloat) * 12 + sizeof(GLfloat) * 8, NULL, GL_STATIC_DRAW);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(GLfloat) * 12, &vertices[i * 4]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12, sizeof(GLfloat) * 12, &normals[i * 4]);
		glBufferSubData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 12 * 2, sizeof(GLfloat) * 8, tex_positions);
	}
}

void init(void) {

	glEnable(GL_DEPTH_TEST);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	ShaderInfo shaders[] = { { GL_VERTEX_SHADER, "./floor.vert" }, {
			GL_FRAGMENT_SHADER, "./floor.frag" }, { GL_NONE, NULL } };

	floorProgram = LoadShaders(shaders);
	if (floorProgram == 0) {
		return;
	}

	ShaderInfo cubeShaders[] = { { GL_VERTEX_SHADER, "./cube.vert" }, {
				GL_FRAGMENT_SHADER, "./cube.frag" }, { GL_NONE, NULL } };

	cubeProgram = LoadShaders(cubeShaders);
	if (cubeProgram == 0) {
		return;
	}

	initFloor();

	initCube();
}

void renderFloor(unsigned long timeSpan)
{
	glUseProgram(floorProgram);
	glBindVertexArray(VAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, element_array_buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	glActiveTexture(GL_TEXTURE0);

	float translaionCoof = 10.0f + std::sin(timeSpan / CAMERA_SPEED * M_PI / 2.0) * 13.0f;

	glm::vec3 eye(0.0f, -15.0f, translaionCoof);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 0.0f, 1.0f);
	glm::mat4 view_mat = glm::lookAt(eye, center, glm::normalize(up));

	glm::mat4 mvp_mat = glm::perspective(45.0f, 16.0f/9.0f, 1.0f, 100.0f) * view_mat;

	GLuint mvp_matr_loc = glGetUniformLocation(floorProgram, "mvp_matrix");

	glUniformMatrix4fv(mvp_matr_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));

	GLint tex1_uniform_loc = glGetUniformLocation(floorProgram, "tex");
	glUniform1i(tex1_uniform_loc, 0);

	int vertex_loc = glGetAttribLocation(floorProgram, "position");
	glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vertex_loc);

	int tex_loc = glGetAttribLocation(floorProgram, "in_tex_coord");
	glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(GLfloat) * 12));
	glEnableVertexAttribArray(tex_loc);

	glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (const void*) (0 * sizeof(GLuint)));
}

void renderCube(unsigned long timeSpan)
{
	glUseProgram(cubeProgram);
	glBindVertexArray(cubeVAO);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeElArrayBuffer);

	float translaionCoof = std::sin(timeSpan / SPEED * M_PI / 2.0) * 7;

	float rotateAngle = M_PI * timeSpan / SPEED / 2.0;

	float camTranslaionCoof = 10.0f + std::sin(timeSpan / CAMERA_SPEED * M_PI / 2.0) * 13.0f;
		
	//std::cout << "rotate angle: " << rotateAngle << std::endl;

	//std::cout << "span: " << timeSpan << ", coof: " << translaionCoof << std::endl;

	glm::mat4 identity(1.0f);
	glm::mat3 iden3(1.0f);
	glm::mat4 model_matr = glm::translate(identity, glm::vec3(translaionCoof, 0.0f, 2.0f));

	glm::mat4 rotate_matr = glm::rotate(identity, rotateAngle * 360.0f / (float)M_PI, glm::vec3(0.0, 0.0, 1.0));
	glm::mat3 rotate3(rotate_matr);

	glm::vec3 eye(0.0f, -15.0f, camTranslaionCoof);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 0.0f, 1.0f);
	glm::mat4 view_mat = glm::lookAt(eye, center, glm::normalize(up));

	glm::mat4 mvp_mat = glm::perspective(45.0f, 16.0f/9.0f, 1.0f, 100.0f) * view_mat * model_matr * rotate_matr;

	GLuint normal_transform_matr_loc = glGetUniformLocation(cubeProgram, "normal_transform_matr");
	glUniformMatrix3fv(normal_transform_matr_loc, 1, GL_FALSE, glm::value_ptr(rotate3));

	GLuint vertex_mvp_matr_loc = glGetUniformLocation(cubeProgram, "vertex_mvp_matr");
	glUniformMatrix4fv(vertex_mvp_matr_loc, 1, GL_FALSE, glm::value_ptr(mvp_mat));

	glm::vec3 toEye = glm::normalize(eye - center);
	glm::vec3 ambient(0.1f, 0.1f, 0.1f);
	glm::vec3 lightCol(1.0f, 1.0f, 1.0f);
	glm::vec3 toLightDir(0.5, -0.5, 0.5);
	toLightDir = glm::normalize(toLightDir);
	glm::vec3 halfVector = glm::normalize(toEye + glm::normalize(toLightDir));

	float shininess = 32.0f;
	float strenght = 1.5f;

	GLuint amb_loc = glGetUniformLocation(cubeProgram, "Ambient");
	glUniform3fv(amb_loc, 1, glm::value_ptr(ambient));

	GLuint light_col_loc = glGetUniformLocation(cubeProgram, "LightColor");
	glUniform3fv(light_col_loc, 1, glm::value_ptr(lightCol));

	GLuint toLight_loc = glGetUniformLocation(cubeProgram, "ToLightDir");
	glUniform3fv(toLight_loc, 1, glm::value_ptr(toLightDir));

	GLuint half_vec_loc = glGetUniformLocation(cubeProgram, "HalfVector");
	glUniform3fv(half_vec_loc, 1, glm::value_ptr(halfVector));

	GLuint shine_loc = glGetUniformLocation(cubeProgram, "Shininess");
	glUniform1f(shine_loc, shininess);

	GLuint str_loc = glGetUniformLocation(cubeProgram, "ReflStrength");
	glUniform1f(str_loc, strenght);

	for (int i = 0; i < 6; ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, cubeBuffers[i]);

		int vertex_loc = glGetAttribLocation(cubeProgram, "position");
		glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
		glEnableVertexAttribArray(vertex_loc);

		int normal_loc = glGetAttribLocation(cubeProgram, "normal");
		glVertexAttribPointer(normal_loc, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(GLfloat) * 12));
		glEnableVertexAttribArray(normal_loc);

		int tex_loc = glGetAttribLocation(cubeProgram, "in_tex_coord");
		glVertexAttribPointer(tex_loc, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(GLfloat) * 12 * 2));
		glEnableVertexAttribArray(tex_loc);

		glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_INT, (const void*) (0 * sizeof(GLuint)));
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//timeval tv;
	//gettimeofday(&tv, 0);

	//unsigned long timeSpan = (((unsigned long)tv.tv_sec * (unsigned long)1000) + tv.tv_usec / 1000) - startTime;
	
	unsigned long timeSpan = glutGet(GLUT_ELAPSED_TIME);

	++framesRendered;
	if (timeSpan - prevTime > FPS_RENEW_CYCLE)
	{
		float fps = (float)framesRendered * 1000.0f /(float)(timeSpan - prevTime);
		glutSetWindowTitle(("FPS: " + std::to_string(fps)).c_str());

		prevTime = timeSpan;
		framesRendered = 0;
	}

	renderFloor(timeSpan);

	renderCube(timeSpan);
	glFlush();
	glutSwapBuffers ();
}

int main(int argc, char** argv) {

	//timeval tv;
	//gettimeofday(&tv, 0);

	//startTime = ((unsigned long)tv.tv_sec * (unsigned long)1000) + tv.tv_usec / 1000;

	//std::cout << startTime << " - strat time\n";

	prevTime = glutGet(GLUT_ELAPSED_TIME);

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(854, 480); //480p
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
	glutIdleFunc(glutPostRedisplay);
	glutMainLoop();

	glDeleteTextures(1, &tex);
}
