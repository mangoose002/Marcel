using namespace std;
#include "Texture.hh"
#include "ColorTexture.hh"
#include "Color.hh"
#include "Image.hh"
#include "Utils/ImageUtils.hh"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

namespace Marcel{
	ColorTexture::ColorTexture(){
		map = NULL;
	}

	void ColorTexture::LoadFile(string Filename){
		_isLoaded = ImageUtils::readFile(Filename,&map,&XSize,&YSize);
	}

	ColorTexture::ColorTexture(char *id, string Filename, int x, int y)
	{
		XTiling = x;
		YTiling = y;
		ID = string(id);

		map = NULL;

		cout << "Loading Color file: " << Filename << " as ID: " << id << "...";
		_isLoaded = ImageUtils::readFile(Filename,&map,&XSize,&YSize);
	}

	ColorTexture::~ColorTexture(){
		if (map != NULL){
			for (int i = 0; i < XSize; i++)
				if (map[i] != NULL)
					delete[] map[i];
			delete[] map;
		}
	}

	Color ColorTexture::getColor(int x, int y){
		if(map!=NULL){
			int _x = ((((x % XSize) + XSize) % XSize) * XTiling) % XSize;
			int _y = ((((y % YSize) + YSize) % YSize) * YTiling) % YSize;
			return map[_x][_y];
		}

		return Color(1,1,1);
	}

	Color ColorTexture::getColor(double u, double v){
		if(map!=NULL){
			int _x = ((((int)(u * XSize * XTiling)) % XSize) + XSize) % XSize;
			int _y = ((((int)(v * YSize * YTiling)) % YSize) + XSize) % XSize;
			return map[_x][_y];
		}

		return Color(1,1,1);
	}


	void ColorTexture::Animate(int f) {}

}