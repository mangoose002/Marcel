using namespace std;
#include "Core/Image.hh"
#include "Core/Color.hh"
#include "Utils/ImageUtils.hh"

#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define PPM 1
#define JPG 2

namespace Marcel{
    Image::Image(const char *n, int x, int y){
        Filename = n;
        FilePart = Filename.substr(0,Filename.rfind("."));

        XResolution = x;
        YResolution = y;

        File = new Color*[x];
        for (int i = 0; i < x; i++)
            File[i] = new Color[y];

    }

    Image::~Image(){
        for (int i = 0; i < XResolution; i++){
	    if(File[i] != NULL){
	            delete File[i];
	    }
	}
        delete[] File;
    }

    void Image::saveImage(){
        ImageUtils::saveFile(Filename,File,XResolution,YResolution);
    }

    void Image::saveLine(int y){
        //Only for constistant code between local and client/server behaviour
    }

    void Image::setColor(Color C, int x, int y){
        if(x < XResolution && y < YResolution)
            File[x][y] = C;
    }

    Color Image::getColor(int x, int y){
        return File[x][y];
    }

    void Image::setOutputFile(const char *f){
        Filename = f;
        FilePart = Filename.substr(0,Filename.rfind("."));
    }

    void Image::setFrame(int i){
        stringstream sstm;

        sstm << FilePart << i << "." << Filename.substr(Filename.find_last_of(".") + 1);
        Filename = sstm.str();
    }

    /////////////////////////////////////////////////////////////////////////
    void Image::setBlock(Color c,int size, int x, int y){
        for (int i = 0; i < size; i++)
            for (int j = 0; j < size; j++){
                File[x + i][y + j] = c;
            }
    }

    float Image::ComputeMaxDifference(int block_size, int x, int y){
        float max = 0;

        for (int i = -1; i <= 1; i++)
            for (int j = -1; j <= 1; j++)
            {
                if (x + i * block_size > 0 && x + i * block_size < XResolution && y + j * block_size > 0 && y + j * block_size < YResolution && i != 0 && j != 0)
                {
                    if (fabs(File[x][y].getRed() - File[x + i * block_size][y + j * block_size].getRed()) > max)
                        max = fabs(File[x][y].getRed() - File[x + i * block_size][y + j * block_size].getRed());

                    if (fabs(File[x][y].getGreen() - File[x + i * block_size][y + j * block_size].getGreen()) > max)
                        max = fabs(File[x][y].getGreen() - File[x + i * block_size][y + j * block_size].getGreen());

                    if (fabs(File[x][y].getBlue() - File[x + i * block_size][y + j * block_size].getBlue()) > max)
                        max = fabs(File[x][y].getBlue() - File[x + i * block_size][y + j * block_size].getBlue());
                }
            }
        return max;
    }
}
