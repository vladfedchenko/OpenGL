#include "LoadShaders.h"

#define VERT_NUM 4

GLuint VAO[1];
GLuint buffer[1];
GLuint element_array_buffer[1];

void init(void)
{
	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	GLuint indexes[6] = { 0, 1, 2, 0, 2, 3};

	glGenBuffers(1, element_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *element_array_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, *buffer);

	GLfloat vertices[VERT_NUM][2] =
	{
		{ -0.95, 0.95},
		{ -0.95, -0.95},
		{ 0.95, -0.95},
		{ 0.95, 0.95}
	};

	GLfloat colors[VERT_NUM][4] =
	{
		{ 1.0, 0.0, 0.0, 1.0},
		{ 0.0, 1.0, 0.0, 1.0},
		{ 0.0, 0.0, 1.0, 1.0},
		{ 1.0, 1.0, 1.0, 1.0}
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), NULL, GL_STATIC_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);

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

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, NULL);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, (const void*)sizeof(vertices));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);


}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);

	glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, NULL);
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
