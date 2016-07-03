#include "LoadShaders.h"

void readShader(const char* filename, std::string &s);

GLuint LoadShaders(ShaderInfo* shaders)
{
	printf("load shaders start\n");
	GLuint vShader = glCreateShader(GL_VERTEX_SHADER);
	GLuint fShader = glCreateShader(GL_FRAGMENT_SHADER);
	std::string vSrc;
	std::string fSrc;
	readShader(shaders[0].filename, vSrc);
	readShader(shaders[1].filename, fSrc);
	printf("read complete\n");
	const char *vSrcRef = vSrc.c_str();
	const char *fSrcRef = fSrc.c_str();
	glShaderSource(vShader, 1, &vSrcRef, 0);
	glShaderSource(fShader, 1, &fSrcRef, 0);
	printf("start compile\n");
	glCompileShader(fShader);
	glCompileShader(vShader);
	
    
	GLint vStatus;
	GLint fStatus;
	glGetShaderiv(vShader,GL_COMPILE_STATUS,&vStatus);
	glGetShaderiv(fShader,GL_COMPILE_STATUS,&fStatus);
	printf("vStatus:%d\n",vStatus);
	printf("fStatus:%d\n",fStatus);
    
    GLsizei vInfoLen;
	GLsizei fInfoLen;
	glGetShaderiv(vShader,GL_INFO_LOG_LENGTH,&vInfoLen);
	glGetShaderiv(fShader,GL_INFO_LOG_LENGTH,&fInfoLen);
    char *vInfoLog = (char*)malloc(vInfoLen);
	char *fInfoLog = (char*)malloc(fInfoLen);
    glGetShaderInfoLog(vShader,vInfoLen,&vInfoLen,vInfoLog);
	glGetShaderInfoLog(fShader,fInfoLen,&fInfoLen,fInfoLog);
    printf("Compile Vertex Shader:%s\n",vInfoLog);
	printf("Compile Fragment Shader:%s\n",fInfoLog);
    
	if(vStatus == GL_FALSE) {
        printf("vertex shader compile failed!\n");
        return 0;
    }
	if(fStatus == GL_FALSE) {
        printf("fragment shader compile failed!\n");
        return 0;
    }
	printf("compile complete\n");

	printf("start create program\n");
	GLuint program = glCreateProgram();
	if(program == 0)
	{
		printf("Create Program Error!");
		return 0;
	}
	glAttachShader(program,vShader);
	glAttachShader(program,fShader);
	printf("attach shader complete\n");
	glLinkProgram(program);
	printf("link program complete\n");
	GLint linked;
	glGetProgramiv(program,GL_LINK_STATUS,&linked);
	GLsizei infoLen;
	char infoLog[100];
	glGetProgramInfoLog(program,100,&infoLen,infoLog);
	printf("Link program:%s\n",infoLog);
	printf("glGetProgramiv complete\n");
	if(linked == GL_TRUE)
	{
		printf("Link success!");
		return program;
	}
	else
	{
		printf("Link failed!");
		return 0;
	}
}

void readShader(const char* filename, std::string &s)
{
	std::string line("");
	std::ifstream shaderFile(filename);
	if (shaderFile.is_open())
	{
		while(std::getline(shaderFile, line))
		{
			s += line + "\n";
		}
		shaderFile.close();
	}
}

