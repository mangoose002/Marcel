#ifndef _IMAGE_UTILS_H_
#define _IMAGE_UTILS_H_

using namespace std;

#include <stdio.h>
#include <string>
#include <jpeglib.h>
#include <setjmp.h>
#include "Core/Color.hh"

namespace Marcel{

    #define PPM_FORMAT 1
    #define PGM_FORMAT 2
    #define JPG_FORMAT 3
    #define BMP_FORMAT 4
    #define TGA_FORMAT 5
    #define FLI_FORMAT 6
    #define PNG_FORMAT 7

    #pragma pack(2) // Add this
    typedef struct                       /**** BMP file header structure ****/
    {
        unsigned short bfType;           /* Magic number for file */
        unsigned int   bfSize;           /* Size of file */
        unsigned short bfReserved1;      /* Reserved */
        unsigned short bfReserved2;      /* ... */
        unsigned int   bfOffBits;        /* Offset to bitmap data */
    } BITMAPFILEHEADER;
    #pragma pack() // and this

    #  define BF_TYPE 0x4D42             /* "MB" */

    typedef struct                       /**** BMP file info structure ****/
    {
        unsigned int   biSize;           /* Size of info header */
        int            biWidth;          /* Width of image */
        int            biHeight;         /* Height of image */
        unsigned short biPlanes;         /* Number of color planes */
        unsigned short biBitCount;       /* Number of bits per pixel */
        unsigned int   biCompression;    /* Type of compression to use */
        unsigned int   biSizeImage;      /* Size of image data */
        int            biXPelsPerMeter;  /* X pixels per meter */
        int            biYPelsPerMeter;  /* Y pixels per meter */
        unsigned int   biClrUsed;        /* Number of colors used */
        unsigned int   biClrImportant;   /* Number of important colors */
    } BITMAPINFOHEADER;

    /*
     * Constants for the biCompression field...
     */

    #  define BI_RGB       0             /* No compression - straight BGR data */
    #  define BI_RLE8      1             /* 8-bit run-length compression */
    #  define BI_RLE4      2             /* 4-bit run-length compression */
    #  define BI_BITFIELDS 3             /* RGB bitmap with RGB masks */

    typedef struct                       /**** Colormap entry structure ****/
    {
        unsigned char  rgbBlue;          /* Blue value */
        unsigned char  rgbGreen;         /* Green value */
        unsigned char  rgbRed;           /* Red value */
        unsigned char  rgbReserved;      /* Reserved */
    } RGBQUAD;

    struct my_error_mgr {
        struct jpeg_error_mgr pub;    /* "public" fields */
        jmp_buf setjmp_buffer;    /* for return to caller */
    };
    typedef struct my_error_mgr * my_error_ptr;

    class ImageUtils{

    private:
    	static int getFileExtension(string Filename);

    	static bool readJPG(string, Color ***, int*,int*);
    	static bool readPPM(string, Color ***, int*,int*);
        static bool readBMP(string, Color ***, int*,int*);
        static bool readTGA(string, Color ***, int*,int*);
        static bool readFLI(string, Color ***, int*,int*);
    	static bool readPNG(string, Color ***, int*,int*);

    	static bool saveJPG(string, Color **,int,int);
    	static bool savePPM(string, Color **,int,int);
        static bool saveBMP(string, Color **,int,int);
    public:
    	static bool readFile(string filename,Color ***,int*,int*);
    	static bool saveFile(string filename,Color **,int,int);
    };
}
#endif