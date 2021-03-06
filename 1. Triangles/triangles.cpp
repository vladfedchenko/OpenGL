#include "LoadShaders.h"

#define Triangles 0
#define NumVAOs 1

#define ArrayBuffer 0
#define NumBuffers 1

#define vPosition 0

GLuint VAOs[NumVAOs];
GLuint Buffers[NumBuffers];

const GLuint NumVertices = 6;

void init(void)
{
	printf("init start\n");
	glGenVertexArrays(NumVAOs, VAOs);
	glBindVertexArray(*VAOs);
	
	GLfloat vertices[NumVertices][2] = {
		{-0.90, -0.90},
		{0.85, -0.90},
		{-0.90, 0.85},
		{0.90, 0.90},
		{0.90, -0.85},
		{-0.85, 0.90}
	};
	
	glGenBuffers(NumBuffers, Buffers);
	glBindBuffer(GL_ARRAY_BUFFER, *Buffers);
	//printf ("%l - size\n", sizeof(vertices));
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	
	ShaderInfo shaders[] = {
		{GL_VERTEX_SHADER, "./triangles.vert"},
		{GL_FRAGMENT_SHADER, "./triangles.frag"},
		{GL_NONE, NULL}
	};

	printf("init 2\n");
	GLuint program = LoadShaders(shaders);
	if(program == 0) return;
	glUseProgram(program);
	
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, (const void*)(0));
	glEnableVertexAttribArray(0);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawArrays(GL_TRIANGLES, 0, NumVertices);
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

