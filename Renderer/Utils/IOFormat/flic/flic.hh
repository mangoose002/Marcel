#ifndef _FLIC_HH_
#define _FLIC_HH_

#include <string>
#include <stdio.h>
#include <stdint.h>

typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;

#define CEL_MAGIC 0x9119

struct CEL_HEADER {
	WORD    type;
	WORD    width;         /* FLIC width in pixels */
	WORD    height;        /* FLIC height in pixels */
	WORD    xcoord;        /* Not used in CEL files */
	WORD    ycoord;        /* Not used in CEL files */
	BYTE    bpp;
	BYTE    compress;      // Compression flag; always zero.
	DWORD   datasize;      // Size of the image data in bytes.
	WORD    filler[8];     // reserved  Unused space; set to zeroes.
};

struct CEL_COLOR {
	uint8_t red;
	uint8_t green;
	uint8_t blue;
};

class FlicImg {
private:
	FILE    *fp;
	CEL_COLOR palette[256];
	uint16_t  width;
	uint16_t  height;
	uint8_t   bpp;
	uint32_t  datasize;


	bool ReadHeader();
	BYTE c6to8(BYTE);
	CEL_COLOR *map;
public:
	FlicImg(std::string);
	uint16_t   getWidth();
	uint16_t   getHeight();
	CEL_COLOR* getMap();
};

#endif