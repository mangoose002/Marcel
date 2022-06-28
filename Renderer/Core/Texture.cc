using namespace std;
#include "Texture.hh"
#include "Color.hh"
#include <string.h>
#include <iostream>

namespace Marcel{

	char *Texture::getID() {
		return ID;
	}

	void Texture::setTiling(int x, int y) {
		XTiling = x;
		YTiling = y;
	}

	void Texture::setAnimate() {
		Animation = 1;
	}

	void Texture::setUscale(int x) {
		XTiling = x;
	}
	void Texture::setVscale(int y) {
		YTiling = y;
	}
	void Texture::setStrength(double s) {
		Strength = s;
	}

	void Texture::ConvertCoordinates(double *u, double *v) {
		(*u) = ((int)((*u) * XSize * XTiling)) % XSize;
		(*v) = ((int)((*v) * YSize * YTiling)) % YSize;
	}

	double Texture::getGray(int x, int y) {
		int _x = ((((x % XSize) + XSize) % XSize) * XTiling) % XSize;
		int _y = ((((y % YSize) + YSize) % YSize) * YTiling) % YSize;

		double r = (map[_x][_y]).getRed();
		double g = (map[_x][_y]).getGreen();
		double b = (map[_x][_y]).getBlue();

		return r * 0.299 + g * 0.587 + b * 0.114;
	}

	double Texture::getGray(double u, double v) {
		int _x, _y;
		_x = ((((int)(u * XSize * XTiling)) % XSize) + XSize) % XSize;
		_y = ((((int)(v * YSize * YTiling)) % YSize) + XSize) % XSize;

		double r = (map[_x][_y]).getRed();
		double g = (map[_x][_y]).getGreen();
		double b = (map[_x][_y]).getBlue();

		return  r * 0.299 + g * 0.587 + b * 0.114;
	}

	bool Texture::isLoaded() {
		return _isLoaded;
	}
}