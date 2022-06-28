#include "Utils/ImageUtils.hh"
#include "Core/Color.hh"
#include <iostream>

using namespace std;

int main(void){

	Color **File;

	int x,y;

	ImageUtils::readFile("toto.png",&File,&x,&y);

	ImageUtils::saveFile("toto.jpg",File,x,y);
	ImageUtils::saveFile("toto.bmp",File,x,y);
	//ImageUtils::saveFile("Marcel-new.ppm",File,x,y);
	//ImageUtils::saveFile("Marcel-new.bmp",File,x,y);

	return true;
}