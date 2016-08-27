/*
 * TextRenderProgram.h
 *
 *  Created on: Aug 27, 2016
 *      Author: vladfedchenko
 */

#ifndef TEXTRENDERPROGRAM_H_
#define TEXTRENDERPROGRAM_H_

#include <SDL2/SDL_ttf.h>
#include "../ShaderProgram.h"

namespace VladFedchenko{
namespace GL{
namespace ShaderPrograms{

	class TextRenderProgram : public ShaderProgram
	{
	private:
		TTF_Font *font = nullptr;
		SDL_Color textCol;
		GLuint textTexture = 0;
		GLuint textVAO;
		GLuint textVBO;

		int width;
		int height;
		glm::mat4 orthoProjMatr;

		void FillColors(const SDL_Surface* textSurface, VladFedchenko::GL::Helpers::PixelColor *colors, int count);

	protected:
		void EnableUniformLocations();

	public:
		TextRenderProgram(const ShaderInfo *shaders, int shaderCount, const std::string fontPath, int fontSize, const SDL_Color &textCol, int width, int height);
		~TextRenderProgram();

		void RenderText(int x, int y, std::string text);
	};

}}}



#endif /* TEXTRENDERPROGRAM_H_ */
