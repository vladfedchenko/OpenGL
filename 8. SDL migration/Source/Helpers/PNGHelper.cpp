/*
 * PNGHelper.cpp
 *
 *  Created on: Aug 24, 2016
 *      Author: vladfedchenko
 */

#include "PNGHelper.h"
#include "assert.h"

namespace VladFedchenko {
namespace GL {
namespace Helpers{

	bool PNGHelper::ReadPNGColors(std::string filename, int width, int height, PixelColor *colors)
	{
		png_bytep* rows = nullptr;
		if (!PNGHelper::ReadPNGRows(filename, rows, width, height))
		{
			std::cerr << "Cannot read PNG file\n";
			return false;
		}

		for(int y = 0; y < height; y++)
		{
			png_bytep row = rows[y];
			for(int x = 0; x < width; x++) {
			  png_bytep px = &(row[x * 4]);

			  colors[(height - 1 - y) * width + x].r = px[0];
			  colors[(height - 1 - y) * width + x].g = px[1];
			  colors[(height - 1 - y) * width + x].b = px[2];
			  colors[(height - 1 - y) * width + x].a = px[3];
			}
		}

		return true;
	}

	bool PNGHelper::ReadPNGRows(std::string &filename, png_bytep* &rows, int w, int h)
	{
		FILE *fp = fopen(filename.c_str(), "rb");

		png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
		if(!png)
		{
			fclose(fp);
			std::cerr << "Cannot create PNG struct!\n";
			return false;
		}

		png_infop info = png_create_info_struct(png);
		if(!info)
		{
			fclose(fp);
			std::cerr << "Cannot create PNG info!\n";
			return false;
		}

		if(setjmp(png_jmpbuf(png)))
		{
			fclose(fp);
			std::cerr << "setjmp error!\n";
			return false;
		}

		png_init_io(png, fp);

		png_read_info(png, info);

		int width      = png_get_image_width(png, info);
		int height     = png_get_image_height(png, info);
		png_byte color_type = png_get_color_type(png, info);
		png_byte bit_depth  = png_get_bit_depth(png, info);

		assert(width == w && height == h);

		if(bit_depth == 16)
			png_set_strip_16(png);

		if(color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_palette_to_rgb(png);

		// PNG_COLOR_TYPE_GRAY_ALPHA is always 8 or 16bit depth.
		if(color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
			png_set_expand_gray_1_2_4_to_8(png);

		if(png_get_valid(png, info, PNG_INFO_tRNS))
			png_set_tRNS_to_alpha(png);

		// These color_type don't have an alpha channel then fill it with 0xff.
		if(color_type == PNG_COLOR_TYPE_RGB ||
		 color_type == PNG_COLOR_TYPE_GRAY ||
		 color_type == PNG_COLOR_TYPE_PALETTE)
			png_set_filler(png, 0xFF, PNG_FILLER_AFTER);

		if(color_type == PNG_COLOR_TYPE_GRAY ||
		 color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
			png_set_gray_to_rgb(png);

		png_read_update_info(png, info);

		rows = (png_bytep*)malloc(sizeof(png_bytep) * height);
		for(int y = 0; y < height; y++) {
			rows[y] = (png_byte*)malloc(png_get_rowbytes(png,info));
		}

		png_read_image(png, rows);

		fclose(fp);

		return true;
	}

}}}




