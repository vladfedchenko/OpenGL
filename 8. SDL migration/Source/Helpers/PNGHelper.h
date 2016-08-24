/*
 * PNGHelper.h
 *
 *  Created on: Aug 24, 2016
 *      Author: vladfedchenko
 */

#ifndef PNGHELPER_H_
#define PNGHELPER_H_

#include <png.h>
#include <iostream>
#include <string>

namespace VladFedchenko {
namespace GL {
namespace Helpers{

	typedef struct PixelColorStruct
	{
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;
	} PixelColor;



	class PNGHelper
	{
	private:
		static bool ReadPNGRows(std::string &filename, png_bytep* &rows, int w, int h);

	public:
		static bool ReadPNGColors(std::string filename, int width, int height, PixelColorStruct *colors);
	};

}}}



#endif /* PNGHELPER_H_ */
