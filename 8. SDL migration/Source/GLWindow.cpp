/*
 * GLWindow.cpp
 *
 *  Created on: Aug 22, 2016
 *      Author: vladfedchenko
 */

#include "GLWindow.h"
#include <iostream>
#include <string>
#include <memory>
#include "Helpers/EventHandling/ShaderProgramManager.h"

using namespace VladFedchenko::GL;
using namespace VladFedchenko::GL::Helpers::EventHandling;
using namespace VladFedchenko::GL::RenderObjects;
using namespace VladFedchenko::GL::Helpers::RenderHandling;

namespace VladFedchenko{
namespace GL{

	GLWindow::GLWindow()
	{
		if (!this->Init())
		{
			std::cerr << "Cannot initialize window!" << std::endl;
			throw std::exception();
		}
		if (TTF_Init() == -1)
		{
			std::cerr << "SDL_ttf could not initialize! SDL_ttf Error:" << TTF_GetError() << std::endl;
			throw std::exception();
		}
	}

	GLWindow::~GLWindow()
	{
		this->eventHandlers.clear();
		this->renderHandlers.clear();
		this->programs.clear();

		if (this->textRenderer != nullptr)
		{
			delete this->textRenderer;
			this->textRenderer = nullptr;
		}

		SDL_GL_DeleteContext(this->mainContext);
		SDL_DestroyWindow(this->mainWindow);
		TTF_Quit();
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
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
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

	void GLWindow::AddProgram(VladFedchenko::GL::RenderObjectShaderProgram *program)
	{
		this->programs.push_back(program);
	}

	void GLWindow::RemoveProgram(VladFedchenko::GL::RenderObjectShaderProgram *program)
	{
		this->programs.remove(program);
	}

	void GLWindow::RegisterEventHandler(VladFedchenko::GL::Helpers::EventHandling::BaseEventHanler* handler)
	{
		this->eventHandlers.push_back(handler);
	}

	void GLWindow::UnregisterEventHandler(VladFedchenko::GL::Helpers::EventHandling::BaseEventHanler* handler)
	{
		this->eventHandlers.remove(handler);
	}

	void GLWindow::RegisterRenderHandler(VladFedchenko::GL::Helpers::RenderHandling::BaseRenderHandler* handler)
	{
		this->renderHandlers.push_back(handler);
	}

	void GLWindow::UnregisterRenderHandler(VladFedchenko::GL::Helpers::RenderHandling::BaseRenderHandler* handler)
	{
		this->renderHandlers.remove(handler);
	}

	void GLWindow::RegisterTextRenderer(const ShaderInfo *shaders, int shaderCount, std::string fontPath, int fontSize, SDL_Color textCol)
	{
		if (this->textRenderer != nullptr)
		{
			delete this->textRenderer;
			this->textRenderer = nullptr;
		}
		this->textRenderer = new VladFedchenko::GL::ShaderPrograms::TextRenderProgram(shaders, shaderCount, fontPath, fontSize, textCol, WIDTH, HEIGHT);
	}

	void GLWindow::RenderText(int x, int y, std::string text)
	{
		if (this->textRenderer != nullptr)
		{
			this->textRenderer->RenderText(x, y, text);
		}
		else
		{
			std::cerr << "Register text renderer first\n";
		}
	}

	void GLWindow::MainLoop()
	{
		bool loop = true;

		while (loop)
		{
			SDL_Event event;
			while (SDL_PollEvent(&event))
			{
				for (Helpers::EventHandling::BaseEventHanler *handler : this->eventHandlers)
				{
					handler->HandleEvent(event);
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

			for (BaseRenderHandler *handler : this->renderHandlers)
			{
				handler->BeforeFrameRender(timeSpan - this->prevFrameStartedTime, timeSpan - this->prevFrameFinishedTime);
			}

			for (RenderObjectShaderProgram *prog : this->programs)
			{
				prog->RenderObjects(timeSpan);
			}

			for (BaseRenderHandler *handler : this->renderHandlers)
			{
				handler->OnFrameRendering();
			}

			fpsTimer += timeSpan - this->prevFrameStartedTime;
			++fpcChechCount;
			if (fpsTimer > FPS_RENEW_CYCLE)
			{
				this->fps = (float)fpcChechCount / ((float)fpsTimer / FPS_RENEW_CYCLE);

				fpsTimer = 0;
				fpcChechCount = 0;
			}

			this->RenderText(5, 5, "FPS: " + std::to_string(fps));

			this->prevFrameStartedTime = timeSpan;
			unsigned long prevFinishTmp = this->prevFrameFinishedTime;
			this->prevFrameFinishedTime = SDL_GetTicks();

			for (BaseRenderHandler *handler : this->renderHandlers)
			{
				handler->AfterFrameRender(this->prevFrameFinishedTime - timeSpan, this->prevFrameFinishedTime - prevFinishTmp);
			}

			SDL_GL_SwapWindow(this->mainWindow);
		}
	}

}}

int main(int argc, char** argv)
{
	GLWindow window;

	ShaderInfo shadersText[] = { {GL_VERTEX_SHADER, "Source/GLSLShaders/text.vert" },
				{GL_FRAGMENT_SHADER, "Source/GLSLShaders/text.frag" }};

	SDL_Color textCol;
	textCol.r = 36;
	textCol.g = 133;
	textCol.b = 12;
	textCol.a = 255;
	window.RegisterTextRenderer(shadersText, 2, "Source/Resources/Fonts/helvetica.ttf", 14, textCol);

	glm::vec3 eye(0.0f, -15.0f, 15.0f);
	glm::vec3 center(0.0f, 0.0f, 0.0f);
	glm::vec3 up(0.0f, 15.0f, 15.0f);
	std::shared_ptr<Camera> cam(new Camera(eye, center, up, 45.0f, (float)WIDTH/ (float)HEIGHT, 1.0f, 100.0f));

	std::shared_ptr<CameraKeyMoveHandler> cameraKeyMoveHandler(new CameraKeyMoveHandler(cam.get()));
	window.RegisterEventHandler(cameraKeyMoveHandler.get());
	window.RegisterRenderHandler(cameraKeyMoveHandler.get());

	std::shared_ptr<CameraMouseMoveHandler> cameraMouseMoveHandler(new CameraMouseMoveHandler(cam.get(), glm::vec3(0.0f, 0.0f, 0.0f)));
	window.RegisterEventHandler(cameraMouseMoveHandler.get());

	std::shared_ptr<ShaderProgramManager> shaderManager(new ShaderProgramManager(window, cam.get()));
	window.RegisterEventHandler(shaderManager.get());
	window.RegisterRenderHandler(shaderManager.get());

	window.MainLoop();

	return 0;
}

