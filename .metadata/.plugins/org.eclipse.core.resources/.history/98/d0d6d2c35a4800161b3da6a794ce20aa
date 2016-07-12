#include "LoadShaders.h"
#include <glm/common.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define VERT_NUM 24
#define INSTANCE_NUM 4

GLuint VAO[1];
GLuint buffer[3];
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

	glEnable(GL_DEPTH_TEST);

	glGenVertexArrays(1, VAO);
	glBindVertexArray(*VAO);

	GLuint indexes[36];
	for (int i = 0; i < 6; ++i)
	{
		indexes[i * 6] = i * 4;
		indexes[i * 6 + 1] = indexes[i * 6] + 1;
		indexes[i * 6 + 2] = indexes[i * 6] + 2;
		indexes[i * 6 + 3] = indexes[i * 6];
		indexes[i * 6 + 4] = indexes[i * 6] + 2;
		indexes[i * 6 + 5] = indexes[i * 6] + 3;
	}

	glGenBuffers(1, element_array_buffer);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, *element_array_buffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

	glGenBuffers(1, buffer);
	glBindBuffer(GL_ARRAY_BUFFER, buffer[0]);

	GLfloat vertices[VERT_NUM][3] =
	{
		{-0.5, -0.5, 0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, -0.5, 0.5},
		{-0.5, -0.5, -0.5}, {-0.5, 0.5, -0.5}, {0.5, 0.5, -0.5}, {0.5, -0.5, -0.5},
		{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {0.5, -0.5, 0.5}, {0.5, -0.5, -0.5},
		{-0.5, 0.5, -0.5}, {-0.5, 0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},
		{-0.5, -0.5, -0.5}, {-0.5, -0.5, 0.5}, {-0.5, 0.5, 0.5}, {-0.5, 0.5, -0.5},
		{0.5, -0.5, -0.5}, {0.5, -0.5, 0.5}, {0.5, 0.5, 0.5}, {0.5, 0.5, -0.5},
	};

	GLfloat colors[VERT_NUM][4] =
	{
		{1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0},
		{1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0}, {1.0, 0.0, 0.0, 1.0},
		{0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0},
		{0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {0.0, 1.0, 0.0, 1.0},
		{0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0},
		{0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}, {0.0, 0.0, 1.0, 1.0}
	};

	GLfloat normals[VERT_NUM][3] =
	{
		{0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, 1.0},
		{0.0, 0.0, -1.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, -1.0},
		{0.0, -1.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, -1.0, 0.0}, {0.0, -1.0, 0.0},
		{0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 1.0, 0.0},
		{-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0}, {-1.0, 0.0, 0.0},
		{1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}, {1.0, 0.0, 0.0}
	};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors) + sizeof(normals), NULL, GL_STATIC_DRAW);

	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(colors), colors);
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(colors), sizeof(normals), normals);

	int vertex_loc = glGetAttribLocation(program, "position");
	glVertexAttribPointer(vertex_loc, 3, GL_FLOAT, GL_FALSE, 0, NULL);
	glEnableVertexAttribArray(vertex_loc);

	int color_loc = glGetAttribLocation(program, "color");
	glVertexAttribPointer(color_loc, 4, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vertices));
	glEnableVertexAttribArray(color_loc);

	int norm_loc = glGetAttribLocation(program, "normal");
	glVertexAttribPointer(norm_loc, 3, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)(sizeof(vertices) + sizeof(colors)));
	glEnableVertexAttribArray(norm_loc);

	glm::vec3 eye(2.0, 2.0, 2.0);
	glm::vec3 norm(-1.0, -1.0, -1.0);
	glm::vec3 up(0.0, 0.0, 1.0);
	glm::mat4 view_mat = glm::lookAt(eye, norm, up);

	glm::mat4 project_mat = glm::frustum(-1.0f, 1.0f, -1.0f, 1.0f, 1.0f, 100.0f);

	GLuint view_matr_loc = glGetUniformLocation(program, "view_matrix");
	GLuint proj_matr_loc = glGetUniformLocation(program, "projection_matrix");

	glUniformMatrix4fv(view_matr_loc, 1, GL_FALSE, glm::value_ptr(view_mat));
	glUniformMatrix4fv(proj_matr_loc, 1, GL_FALSE, glm::value_ptr(project_mat));

	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT);
	glDrawElements(GL_TRIANGLES, 30, GL_UNSIGNED_INT, (const void*)(0 * sizeof(GLuint)));
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
