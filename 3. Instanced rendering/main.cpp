#include "LoadShaders.h"

#define VERT_NUM 3
#define INSTANCE_NUM 4

GLuint VAO[1];
GLuint buffer[1];
GLuint color_buf[1];
GLuint element_array_buffer[1];

void init(void)
{
	ShaderInfo shaders[] = {
				{GL_VERTEX_SHADER, "./triangles.vert"},
				{GL_FRAGMENT_SHADER, "./triangles.frag"},
				{GL_NONE, NULL}
			};

	GLuint program = LoadShaders(shaders);
	if(program == 0)
	{
		return;
	}
	glUseProgram(program);

	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	GLuint indexes[3] = { 0, 1, 2};

	glGenBuffers(1, element_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *element_array_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);

	GLfloat vertices[VERT_NUM][2] =
	{
		{ -0.9, 0.57735},
		{ 0.1, 0.0},
		{ -0.9, -0.57735}
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	int vertex_loc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(vertex_loc, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vertex_loc);

	glGenBuffers(1, color_buf);
	glBindBuffer(GL_ARRAY_BUFFER, *color_buf);

	GLfloat colors[INSTANCE_NUM][4] =
		{
			{ 1.0, 1.0, 1.0, 1.0},
			{ 1.0, 0.0, 0.0, 1.0},
			{ 0.0, 1.0, 0.0, 1.0},
			{ 0.0, 0.0, 1.0, 1.0}
		};

	glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);

	int color_loc = glGetAttribLocation(program, "color");
	glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(color_loc);
	glVertexAttribDivisor(color_loc, 1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, NULL, INSTANCE_NUM);
	glFlush();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA);
	glutInitWindowSize(512, 512);
	glutInitContextVersion(3, 3);
	glutInitContextProfile(GLUT_CORE_PROFILE);
	glutCreateWindow(argv[0]);

	glewExperimental = GL_TRUE;
	if (glewInit())
	{
		std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
		exit(EXIT_FAILURE);
	}

	init();
	glutDisplayFunc(display);
	glutMainLoop();
}
