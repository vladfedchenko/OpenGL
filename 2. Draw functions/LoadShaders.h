#include <GL/glew.h>
#include <GL/freeglut.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#include <iostream>
#include <cstdio>
#include <fstream>
#include <string>

typedef struct {
	GLenum type;
	const char* filename;
} ShaderInfo;

GLuint LoadShaders(ShaderInfo* shaders);
