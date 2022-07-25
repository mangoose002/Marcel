using namespace std;
#include "Texture.hh"
#include "GrayTexture.hh"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace Marcel{
GrayTexture::GrayTexture(char *id, char *Filename, int x, int y)
{
  XTiling = x;
  YTiling = y;
  ID = string(id);

  FILE* fp;

  cout << "Loading Gray file: " << Filename;
  fp = fopen(Filename, "r");
  if (fp == NULL)
  {
    cout << endl << "Unable to open map file: " << Filename << endl;
    exit(0);
  }

  int Type;
  double R, G, B;
  int i, j;
  FileName = string(Filename);

  fscanf(fp, "P%d\n", &Type);
  if (Type != 6 && Type != 5) // Binaire
  {
    cout << endl << "Bad File Format." << endl;
    exit(0);
  }

  fscanf(fp, "%d %d\n", &XSize, &YSize);
  fscanf(fp, "%d\n", &MaxColorValue);

  cout << "  " << XSize << "x" << YSize;

  map = new double*[XSize];
  for (i = 0; i < XSize; i++)
    map[i] = new double[YSize];


  for (j = 0; j < YSize; j++)
    for (i = 0; i < XSize; i++)
    {
      if (Type == 6)
      {
        R = fgetc(fp) / ((double)MaxColorValue);
        G = fgetc(fp) / ((double)MaxColorValue);
        B = fgetc(fp) / ((double)MaxColorValue);
        map[i][j] = R * 0.299 + G * 0.587 + B * 0.114;
      }
      else
      {
        R = fgetc(fp) / ((double)MaxColorValue);
        map[i][j] = R;
      }
    }

  cout << " Done" << endl;
  fclose(fp);
}

GrayTexture::~GrayTexture()
{
  for (int i = 0; i < XSize; i++)
    if (map[i] != NULL)
      delete[] map[i];
  delete[] map;
}

double GrayTexture::getGray(int x, int y)
{
  return map[(((x + XSize) % XSize) * XTiling) % XSize][(((y + YSize) % YSize) * YTiling) % YSize];
}

double GrayTexture::getGray(double u, double v)
{
  int _x, _y;
  _x = ((int)(u * XSize * XTiling)) % XSize;
  _y = ((int)(v * YSize * YTiling)) % YSize;
  return map[_x][_y];
}

Color GrayTexture::getColor(int x, int y)
{
  double a = map[(((x + XSize) % XSize) * XTiling) % XSize][(((y + YSize) % YSize) * YTiling) % YSize];

  return Color(a * 255, a * 255, a * 255);
}

Color GrayTexture::getColor(double u, double v)
{
  int _x, _y;
  _x = ((int)(u * XSize * XTiling)) % XSize;
  _y = ((int)(v * YSize * YTiling)) % YSize;
  double a = map[_x][_y];

  return Color(a * 255, a * 255, a * 255);
}

void GrayTexture::Animate(int f)
{
}
}