/*
 * TextRenderProgram.cpp
 *
 *  Created on: Aug 27, 2016
 *      Author: vladfedchenko
 */

#include "TextRenderProgram.h"
#include "glm/gtc/type_ptr.hpp"

namespace VladFedchenko{
namespace GL{
namespace ShaderPrograms{

	TextRenderProgram::TextRenderProgram(const ShaderInfo *shaders, int shaderCount, const std::string fontPath, int fontSize, const SDL_Color &textCol, int width, int height)
		: ShaderProgram(shaders, shaderCount), textCol(textCol), width(width), height(height)
	{
		this->font = TTF_OpenFont(fontPath.c_str(), fontSize);
		if (this->font == nullptr)
		{
			std::cerr << "Cannot load font!\n";
			throw std::exception();
		}
		this->orthoProjMatr = glm::ortho(0.0f, (float)width, 0.0f, (float)height);

		glGenVertexArrays(1, &this->textVAO);

		glBindVertexArray(this->textVAO);
		glGenBuffers(1, &this->textVBO);
		glBindBuffer(GL_ARRAY_BUFFER, this->textVBO);

		glBufferData(GL_ARRAY_BUFFER, 20 * sizeof(GLfloat), NULL, GL_DYNAMIC_DRAW);

		glGenTextures(1, &this->textTexture);
		glBindTexture(GL_TEXTURE_2D, this->textTexture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0); //base mipmap level
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 0); //max mipmap levels
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		glBindTexture(GL_TEXTURE_2D, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	TextRenderProgram::~TextRenderProgram()
	{
		TTF_CloseFont(this->font);
		this->font = nullptr;

		glDeleteTextures(1, &this->textTexture);
		glDeleteVertexArrays(1, &this->textVAO);
		glDeleteBuffers(1, &this->textVBO);
	}

	void TextRenderProgram::RenderText(int x, int y, std::string text)
	{
		SDL_Surface* textSurface = TTF_RenderUTF8_Blended(this->font, text.c_str(), this->textCol);
		if( textSurface == NULL )
		{
			std::cerr << "Unable to render text surface! SDL_ttf Error: " << TTF_GetError() << std::endl;
		}
		else
		{
			glUseProgram(this->program);
			glBindVertexArray(this->textVAO);

			this->EnableUniformLocations();

			int textW = textSurface->w;
			int textH = textSurface->h;

			VladFedchenko::GL::Helpers::PixelColor *colors = new VladFedchenko::GL::Helpers::PixelColor[textW * textH];

			this->FillColors(textSurface, colors, textW * textH);

			glActiveTexture(GL_TEXTURE0);

			glBindTexture(GL_TEXTURE_2D, this->textTexture);
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA32F, textW, textH, 0, GL_RGBA, GL_UNSIGNED_BYTE, colors);

			GLuint tex1_uniform_loc = glGetUniformLocation(this->program, "text1");
			glUniform1i(tex1_uniform_loc, 0);

			SDL_FreeSurface(textSurface);

			float vertices[4][3] =
			{
				{(float)x, (float)(this->height - y - textH), 0.0f},
				{(float)x, (float)(this->height - y), 0.0f},
				{(float)(x + textW), (float)(this->height - y), 0.0f},
				{(float)(x + textW), (float)(this->height - y - textH), 0.0f}
			};

			float texCoords[4][2] =
			{
				{0.0, 1.0},
				{0.0, 0.0},
				{1.0, 0.0},
				{1.0, 1.0}
			};

			glBindBuffer(GL_ARRAY_BUFFER, this->textVBO);
			glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
			glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(texCoords), texCoords);

			int posLoc = glGetAttribLocation(this->program, "position");
			glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
			glEnableVertexAttribArray(posLoc);

			int texLoc = glGetAttribLocation(this->program, "in_tex_coord");
			glVertexAttribPointer(texLoc, 2, GL_FLOAT, GL_FALSE, 0, (const GLvoid*)sizeof(vertices));
			glEnableVertexAttribArray(texLoc);

			glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

			glBindBuffer(GL_ARRAY_BUFFER, 0);
			glBindVertexArray(0);
		}
	}

	void TextRenderProgram::EnableUniformLocations()
	{
		GLuint vertex_mvp_matr_loc = glGetUniformLocation(this->program, "vertex_mvp_matr");
		glUniformMatrix4fv(vertex_mvp_matr_loc, 1, GL_FALSE, glm::value_ptr(this->orthoProjMatr));
	}

	void TextRenderProgram::FillColors(const SDL_Surface* textSurface, VladFedchenko::GL::Helpers::PixelColor *colors, int count)
	{
		const unsigned int* pixels = (const unsigned int*)textSurface->pixels;

		for (int i = 0; i < count; ++i)
		{
			unsigned int pix = pixels[i];
			//std::cout << pix << std::endl;

			unsigned int r = pix & textSurface->format->Rmask;
			while (r > 255)
			{
				r >>= 8;
			}
			colors[i].r = (unsigned char)r;

			unsigned int g = pix & textSurface->format->Gmask;
			while (g > 255)
			{
				g >>= 8;
			}
			colors[i].g = (unsigned char)g;

			unsigned int b = pix & textSurface->format->Bmask;
			while (b > 255)
			{
				b >>= 8;
			}
			colors[i].b = (unsigned char)b;

			unsigned int a = pix & textSurface->format->Amask;
			while (a > 255)
			{
				a >>= 8;
			}
			colors[i].a = (unsigned char)a;

			//std::cout << (int)colors[i].r << ", " << (int)colors[i].g << ", " << (int)colors[i].b << ", " << (int)colors[i].a << std::endl;
		}
	}

}}}


