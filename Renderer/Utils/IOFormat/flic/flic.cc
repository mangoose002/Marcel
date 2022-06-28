#include "flic.hh"
#include <stdio.h>
#include <iostream>

using namespace std;

BYTE FlicImg::c6to8(BYTE b) {
	if (b > 0) {
		return ((b + 1) << 2) - 1;
	}
	return 0;
}

bool FlicImg::ReadHeader() {
	CEL_HEADER header;

	size_t read = fread(&header, sizeof(CEL_HEADER), 1, fp);
	if (header.type == CEL_MAGIC) {
		if (header.bpp != 8) {
			cerr << "Not supported format" << endl;
			return false;
		}
		width    = header.width;
		height   = header.height;
		bpp      = header.bpp;
		datasize = header.datasize;
	}
	return true;
}

FlicImg::FlicImg(string filename) {
	map = NULL;
	fp = fopen(filename.c_str(), "rb");
	if(fp==NULL){
		cerr << "Failed" << endl;
		return;
	}

	if (ReadHeader()) {
		map = new CEL_COLOR[datasize];
		BYTE *buffer = new BYTE[datasize];

		size_t read = fread(&palette, 1, 768, fp);
		for (int i = 0; i < 256; i++) {
			palette[i].red   = c6to8(palette[i].red);
			palette[i].green = c6to8(palette[i].green);
			palette[i].blue  = c6to8(palette[i].blue);
		}

		fread(buffer,1,datasize,fp);
		for(int i=0;i<datasize;i++){
			map[i] = palette[buffer[i]];
		}
		delete[] buffer;
		fclose(fp);
	}
}

uint16_t FlicImg::getWidth(){
	return width;
}
uint16_t FlicImg::getHeight(){
	return height;
}
CEL_COLOR *FlicImg::getMap(){
	return map;
}