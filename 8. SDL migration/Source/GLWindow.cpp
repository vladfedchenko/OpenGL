/*
 * GLWindow.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: vladfedchenko
 */

#include "GLWindow.h"
#include <iostream>
#include "ShaderPrograms/ClassicLMTexGenShader.h"
#include "ShaderPrograms/ClassicLMTexLoadShader.h"
#include "RenderObjects/CubeRenderObject.h"
#include "RenderObjects/FloorRenderObject.h"

using namespace VladFedchenko::GL;

namespace VladFedchenko{
namespace GL{

	GLWindow::GLWindow()
		: cameraKeyHandler(nullptr), cameraMouseHandler(nullptr)
	{
		if (!this->Init())
		{
			std::cerr << "Cannot initialize window!" << std::endl;
			exit(EXIT_FAILURE);
		}
	}

	GLWindow::~GLWindow()
	{
		if (this->cameraKeyHandler != nullptr)
		{
			delete this->cameraKeyHandler;
			this->cameraKeyHandler = nullptr;
		}

		if (this->cameraMouseHandler != nullptr)
		{
			delete this->cameraMouseHandler;
			this->cameraMouseHandler = nullptr;
		}

		SDL_GL_DeleteContext(this->mainContext);
		SDL_DestroyWindow(this->mainWindow);
		SDL_Quit();
	}

	bool GLWindow::Init()
	{
		if (SDL_Init(SDL_INIT_VIDEO) < 0)
		{
			std::cerr << "Failed to init SDL\n";
			return false;
		}

		this->mainWindow = SDL_CreateWindow("VladFedchenko", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WIDTH, HEIGHT, SDL_WINDOW_OPENGL);

		if (!mainWindow )
		{
			std::cerr << "Unable to create window\n"<< std::endl;;
			this->CheckSDLError();
			return false;
		}

		this->SetOpenGLAttributes();

		this->mainContext = SDL_GL_CreateContext(this->mainWindow);

		SDL_GL_SetSwapInterval(1);

		glewExperimental = GL_TRUE;
		if (glewInit())
		{
			std::cerr << "Unable to initialize GLEW ... exiting" << std::endl;
			return false;
		}

		glEnable(GL_DEPTH_TEST);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

		return true;
	}

	void GLWindow::CheckSDLError()
	{
		std::string error = SDL_GetError();

		if (error != "")
		{
			std::cerr << "SLD Error : " << error << std::endl;

			SDL_ClearError();
		}
	}

	bool GLWindow::SetOpenGLAttributes()
	{
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		return true;
	}

	void GLWindow::AddProgram(VladFedchenko::GL::ShaderProgram *program)
	{
		this->programs.push_back(program);
	}

	bool GLWindow::RemoveProgram(VladFedchenko::GL::ShaderProgram *program)
	{
		for (std::vector<ShaderProgram*>::iterator iter = this->programs.begin(); iter != this->programs.end(); ++iter)
		{
			if (*iter == program)
			{
				this->programs.erase(iter);
				return true;
			}
		}
		return false;
	}

	void GLWindow::RegisterCameraKeyHandler(VladFedchenko::GL::Camera *camera)
	{
		if (this->cameraKeyHandler != nullptr)
		{
			delete this->cameraKeyHandler;
			this->cameraKeyHandler = nullptr;
		}

		this->cameraKeyHandler = new VladFedchenko::GL::Helpers::CameraKeyMoveHandler(camera);
	}

	void GLWindow::RegisterCameraMouseHandler(VladFedchenko::GL::Camera *camera, const glm::vec3 &rotCenter)
	{
		if (this->cameraMouseHandler != nullptr)
		{
			delete this->cameraMouseHandler;
			this->cameraMouseHandler = nullptr;
		}

		this->cameraMouseHandler = new VladFedchenko::GL::Helpers::CameraMouseMoveHandler(camera, rotCenter);
	}

	void GLWindow::MainLoop()
	{
		bool loop = true;

		while (loop)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				if (this->cameraKeyHandler != nullptr)
				{
					this->cameraKeyHandler->HandleEvent(event);
				}
				if (this->cameraMouseHandler != nullptr)
				{
					this->cameraMouseHandler->HandleEvent(event);
				}

				if (event.type == SDL_QUIT)
				{
					loop = false;
				}
				if (event.type == SDL_KEYDOWN)
				{
					switch (event.key.keysym.sym)
					{
					case SDLK_ESCAPE:
						loop = false;
						break;

					default:
						break;
					}
				}
			}

			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			unsigned long timeSpan = SDL_GetTicks();

			if (this->cameraKeyHandler != nullptr)
			{
				//std::cout << "Moving camera\n";
				this->cameraKeyHandler->MoveCameraIfNeeded(timeSpan - this->prevFrameTime);
			}

			for (ShaderProgram *prog : this->programs)
			{
				prog->RenderObjects(timeSpan);
			}

			this->prevFrameTime = timeSpan;

			SDL_GL_SwapWindow(this->mainWindow);
		}
	}

}}

int main(int argc, char** argv)
{
	GLWindow window;

	ShaderInfo shadersCube[] = { {GL_VERTEX_SHADER, "Source/GLSLShaders/cube.vert" },
		{GL_FRAGMENT_SHADER, "Source/GLSLShaders/cube.frag" }};

	ShaderInfo shadersFloor[] = { {GL_VERTEX_SHADER, "Source/GLSLShaders/floor.vert" },
			{GL_FRAGMENT_SHADER, "Source/GLSLShaders/floor.frag" }};


	glm::vec3 eye(0.0f, -15.0f, 15.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 15.0f, 15.0f);
	Camera *cam = new Camera(eye, center, up, 45.0f, (float)WIDTH/ (float)HEIGHT, 1.0f, 100.0f);

	window.RegisterCameraKeyHandler(cam);
	window.RegisterCameraMouseHandler(cam, glm::vec3(0.0f, 0.0f, 0.0f));

	ShaderProgram *prog = new VladFedchenko::GL::ShaderPrograms::ClassicLMTexGenShader(shadersCube, 2, cam);
	ShaderProgram *floorProg = new VladFedchenko::GL::ShaderPrograms::ClassicLMTexLoadShader(shadersFloor, 2, cam);

	VladFedchenko::GL::RenderObjects::CubeRenderObject *obj = new VladFedchenko::GL::RenderObjects::CubeRenderObject(cam);
	VladFedchenko::GL::RenderObjects::FloorRenderObject *floorObj = new VladFedchenko::GL::RenderObjects::FloorRenderObject(cam, "Source/Resources/Images/Floor.png", 1366, 768);

	prog->AddObject(obj);
	floorProg->AddObject(floorObj);

	window.AddProgram(floorProg);
	window.AddProgram(prog);

	window.MainLoop();

	delete prog;
	delete obj;

	delete floorProg;
	delete floorObj;

	delete cam;

	return 0;
}

