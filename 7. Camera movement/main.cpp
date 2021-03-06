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
#include <map>

#define WIDTH 1366
#define HEIGHT 768
#define SPEED 3500.0f

#define FPS_RENEW_CYCLE 1000.0f //in miliseconds

#define STD_MOVE_COEF 1.0f
#define SHIFT_MULT 5.0f

#define CAM_ROT_COEF 0.36f // = 180 / 500

typedef struct PixelColorStruct
{
	unsigned char r;
	unsigned char g;
	unsigned char b;
	unsigned char a;
} PixelColor;

unsigned long prevTime, lastFrameTime;
int framesRendered = 0;
float fps = 0.0f;

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

float windowWidth = 854.0f;
float windowHeight = 480.0f;

glm::vec3 eye(0.0f, -15.0f, 15.0f);
glm::vec3 center(0.0f, 0.0f, 0.0f);
glm::vec3 up(0.0f, 15.0f, 15.0f);

glm::vec3 rotationCenter(0.0f, 0.0f, 0.0f);
int prevX, prevY;

std::map<unsigned char, bool> keyPressMap;
bool shiftDown = false;
bool arcballModeActive = true;

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

void positionChanged(glm::vec3 &toMove)
{
	eye += toMove;
	center += toMove;
	glutPostRedisplay();
}

void keyDownHandler(unsigned char key, int x, int y)
{
	//std::cout << "Pressed: " << key << ". x, y: " << x << ", " << y << std::endl;

	switch(key)
	{
	case 'w':
	case 'W':
		keyPressMap['w'] = true;
		break;
	case 's':
	case 'S':
		keyPressMap['s'] = true;
		break;
	case 'a':
	case 'A':
		keyPressMap['a'] = true;
		break;
	case 'd':
	case 'D':
		keyPressMap['d'] = true;
		break;
	case 'q':
	case 'Q':
		keyPressMap['q'] = true;
		break;
	case 'e':
	case 'E':
		keyPressMap['e'] = true;
		break;
	case 'f':
		arcballModeActive = !arcballModeActive;
		break;
	}
}

void keyUpHandler(unsigned char key, int x, int y)
{
	//std::cout << "Released: " << key << ". x, y: " << x << ", " << y << std::endl;
	switch(key)
	{
	case 'w':
	case 'W':
		keyPressMap['w'] = false;
		break;
	case 's':
	case 'S':
		keyPressMap['s'] = false;
		break;
	case 'a':
	case 'A':
		keyPressMap['a'] = false;
		break;
	case 'd':
	case 'D':
		keyPressMap['d'] = false;
		break;
	case 'q':
	case 'Q':
		keyPressMap['q'] = false;
		break;
	case 'e':
	case 'E':
		keyPressMap['e'] = false;
		break;
	}
}

void specialDownHandler(int code, int x, int y)
{
	if (code == GLUT_KEY_SHIFT_L)
	{
		shiftDown = true;
	}
}

void specialUpHandler(int code, int x, int y)
{
	if (code == GLUT_KEY_SHIFT_L)
	{
		shiftDown = false;
	}
}

void initCameraMovement()
{
	up = glm::normalize(up);

	keyPressMap['q'] = false;
	keyPressMap['w'] = false;
	keyPressMap['e'] = false;
	keyPressMap['a'] = false;
	keyPressMap['s'] = false;
	keyPressMap['d'] = false;

	glutKeyboardFunc(keyDownHandler);
	glutKeyboardUpFunc(keyUpHandler);

	glutSpecialFunc(specialDownHandler);
	glutSpecialUpFunc(specialUpHandler);

	glutIgnoreKeyRepeat(1);
}


void init(void) {

	initCameraMovement();

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

	glm::mat4 view_mat = glm::lookAt(eye, center, up);

	glm::mat4 mvp_mat = glm::perspective(45.0f, windowWidth / windowHeight, 1.0f, 100.0f) * view_mat;

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
		
	//std::cout << "rotate angle: " << rotateAngle << std::endl;

	//std::cout << "span: " << timeSpan << ", coof: " << translaionCoof << std::endl;

	glm::mat4 identity(1.0f);
	glm::mat3 iden3(1.0f);
	glm::mat4 model_matr = glm::translate(identity, glm::vec3(translaionCoof, 0.0f, 2.0f));

	glm::mat4 rotate_matr = glm::rotate(identity, rotateAngle * 360.0f / (float)M_PI, glm::vec3(0.0, 0.0, 1.0));
	glm::mat3 rotate3(rotate_matr);

	glm::mat4 view_mat = glm::lookAt(eye, center, up);

	glm::mat4 mvp_mat = glm::perspective(45.0f, windowWidth / windowHeight, 1.0f, 100.0f) * view_mat * model_matr * rotate_matr;

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

	int vertex_loc = glGetAttribLocation(cubeProgram, "position");

	for (int i = 0; i < 6; ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, cubeBuffers[i]);

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

float getMoveCoof(unsigned long timePassed)
{
	float base = shiftDown ? (STD_MOVE_COEF * SHIFT_MULT) : (STD_MOVE_COEF);
	return base * (float)(timePassed / 1000.0f);
}

void moveCameraIfNeeded(unsigned long timePassed)
{
	float moveDistCoof = getMoveCoof(timePassed);

	glm::vec3 tmp;
	glm::vec3 toMove(0.0f, 0.0f, 0.0f);
	if (keyPressMap['a'])
	{
		tmp = glm::normalize(center - eye);
		toMove += glm::normalize(glm::cross(up, tmp)) * moveDistCoof;
	}
	if (keyPressMap['d'])
	{
		tmp = glm::normalize(center - eye);
		toMove -= glm::normalize(glm::cross(up, tmp)) * moveDistCoof;
	}
	if (keyPressMap['w'])
	{
		toMove += glm::normalize(center - eye) * moveDistCoof;
	}
	if (keyPressMap['s'])
	{
		toMove -= glm::normalize(center - eye) * moveDistCoof;
	}
	if (keyPressMap['e'])
	{
		toMove += up * moveDistCoof;
	}
	if (keyPressMap['q'])
	{
		toMove -= up * moveDistCoof;
	}

	positionChanged(toMove);
}

void drawBitmapText(char *string, int x, int y)
{
//	glMatrixMode(GL_PROJECTION);
//	glLoadIdentity();
	char *c;
	glWindowPos2i(x, y);
	glColor3f(1.0f, 1.0f, 1.0f);

	for (c = string; *c != '\0'; ++c)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *c);
	}
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glLoadIdentity();
	
	unsigned long timeSpan = glutGet(GLUT_ELAPSED_TIME);

	moveCameraIfNeeded(timeSpan - lastFrameTime);
	lastFrameTime = timeSpan;

	++framesRendered;
	if (timeSpan - prevTime > FPS_RENEW_CYCLE)
	{
		fps = (float)framesRendered * 1000.0f /(float)(timeSpan - prevTime);
		glutSetWindowTitle(("FPS: " + std::to_string(fps)).c_str());

		prevTime = timeSpan;
		framesRendered = 0;
	}



	renderFloor(timeSpan);

	renderCube(timeSpan);

	drawBitmapText((char*)"Vlad", 20, 20);

	glFlush();
	glutSwapBuffers ();
}

void windowReshaped(int x, int y)
{
	windowWidth = (float)x;
	windowHeight = (float)y;
	glViewport(0, 0, x, y);
	//glutReshapeWindow(windowWidth, windowHeight);
	glutPostRedisplay();
}

void translateAzimuthArcball(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle)
{
	glm::mat4 identity(1.0f);
	glm::mat4 rotMatr = glm::rotate(identity, angle, glm::vec3(0.0f, 0.0f, -1.0f));

	glm::vec4 homo_eye(trans_eye, 1.0);
	glm::vec4 homo_center(trans_center, 1.0);
	glm::vec4 homo_up(trans_up, 1.0);

	trans_eye = glm::vec3(rotMatr * homo_eye);
	trans_center = glm::vec3(rotMatr * homo_center);
	trans_up = glm::vec3(rotMatr * homo_up);
}

void translatePolarArcball(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle)
{
	glm::mat4 identity(1.0f);
	glm::vec3 right = glm::cross(trans_up, glm::normalize(trans_center - trans_eye));
	right = glm::normalize(right);

	glm::mat4 rotMatr = glm::rotate(identity, angle, right);

	glm::vec4 homo_eye(trans_eye, 1.0);
	glm::vec4 homo_center(trans_center, 1.0);
	glm::vec4 homo_up(trans_up, 1.0);

	trans_eye = glm::vec3(rotMatr * homo_eye);
	trans_center = glm::vec3(rotMatr * homo_center);
	trans_up = glm::vec3(rotMatr * homo_up);
}

void translateHorizontalNormalCam(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle)
{
	trans_center -= trans_eye;

	glm::mat4 identity(1.0f);
	glm::mat4 rotMatr = glm::rotate(identity, angle, glm::vec3(0.0f, 0.0f, -1.0f));

	glm::vec4 homo_center(trans_center, 1.0);
	glm::vec4 homo_up(trans_up, 1.0);

	trans_center = glm::vec3(rotMatr * homo_center);
	trans_up = glm::vec3(rotMatr * homo_up);

	trans_center += trans_eye;
}

void translateVerticalNormalCam(glm::vec3 &trans_eye, glm::vec3 &trans_center, glm::vec3 &trans_up, float angle)
{
	glm::vec3 right = glm::cross(trans_up, glm::normalize(trans_center - trans_eye));
	right = glm::normalize(right);

	trans_center -= trans_eye;

	glm::mat4 identity(1.0f);
	glm::mat4 rotMatr = glm::rotate(identity, angle, right);

	glm::vec4 homo_center(trans_center, 1.0);
	glm::vec4 homo_up(trans_up, 1.0);

	trans_center = glm::vec3(rotMatr * homo_center);
	trans_up = glm::vec3(rotMatr * homo_up);

	trans_center += trans_eye;
}

void mouseMotionHandler(int x, int y)
{
	//std::cout << "Coords: " << x << ", " << y << std::endl;

	eye - rotationCenter;
	center - rotationCenter;

	if (x != prevX)
	{
		if (arcballModeActive)
		{
			translateAzimuthArcball(eye, center, up, (x - prevX) * CAM_ROT_COEF);
		}
		else
		{
			translateHorizontalNormalCam(eye, center, up, (x - prevX) * CAM_ROT_COEF);
		}
	}
	if (y != prevY)
	{
		if (arcballModeActive)
		{
			translatePolarArcball(eye, center, up, (y - prevY) * CAM_ROT_COEF);
		}
		else
		{
			translateVerticalNormalCam(eye, center, up, (y - prevY) * CAM_ROT_COEF);
		}
	}

	eye += rotationCenter;
	center += rotationCenter;

	prevX = x;
	prevY = y;

	glutPostRedisplay();
}

void mousePressHandler(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		prevX = x;
		prevY = y;
	}
}

int main(int argc, char** argv) {

	//timeval tv;
	//gettimeofday(&tv, 0);

	//startTime = ((unsigned long)tv.tv_sec * (unsigned long)1000) + tv.tv_usec / 1000;

	//std::cout << startTime << " - strat time\n";

	prevTime = glutGet(GLUT_ELAPSED_TIME);
	lastFrameTime = prevTime;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(windowWidth, windowHeight); //480p
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

	glutReshapeFunc(windowReshaped);

	glutMotionFunc(mouseMotionHandler);
	glutMouseFunc(mousePressHandler);

	glutMainLoop();

	glDeleteTextures(1, &tex);
}
