#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <jpeglib.h>
#include <gif_lib.h>
#include <setjmp.h>
#include <math.h>
#include <algorithm>

#include "Utils/ImageUtils.hh"
#include "Utils/IOFormat/tga/tga.h"
#include "Utils/IOFormat/flic/flic.hh"
#include "Utils/IOFormat/png/lodepng.h"
#include "Core/Color.hh"

using namespace std;

namespace Marcel{

	METHODDEF(void)
	my_error_exit (j_common_ptr cinfo)
	{
		/* cinfo->err really points to a my_error_mgr struct, so coerce pointer */
		my_error_ptr myerr = (my_error_ptr) cinfo->err;
		/* Always display the message. */
		/* We could postpone this until after returning, if we chose. */
		(*cinfo->err->output_message) (cinfo);
		/* Return control to the setjmp point */
		longjmp(myerr->setjmp_buffer, 1);
	}

	int ImageUtils::getFileExtension(string Filename) {
		std::transform(Filename.begin(), Filename.end(), Filename.begin(), ::tolower);

		if (Filename.substr(Filename.find_last_of(".") + 1) == "ppm") {
			return PPM_FORMAT;
		} else if (Filename.substr(Filename.find_last_of(".") + 1) == "jpg") {
			return JPG_FORMAT;
		} else if (Filename.substr(Filename.find_last_of(".") + 1) == "bmp") {
			return BMP_FORMAT;
		} else if (Filename.substr(Filename.find_last_of(".") + 1) == "tga") {
			return TGA_FORMAT;
		} else if (Filename.substr(Filename.find_last_of(".") + 1) == "cel") {
			return FLI_FORMAT;
		} else if (Filename.substr(Filename.find_last_of(".") + 1) == "png") {
			return PNG_FORMAT;
		} else if (Filename.substr(Filename.find_last_of(".") + 1) == "gif") {
			return GIF_FORMAT;
		}

		return -1;
	}

	bool ImageUtils::readFile(string filename, Color ***File, int *XResolution, int *YResolution) {

		switch (getFileExtension(filename)) {
		case GIF_FORMAT:
			return ImageUtils::readGIF(filename, File, XResolution, YResolution);
			break;
		case BMP_FORMAT:
			return ImageUtils::readBMP(filename, File, XResolution, YResolution);
			break;
		case JPG_FORMAT:
			return ImageUtils::readJPG(filename, File, XResolution, YResolution);
			break;
		case PPM_FORMAT:
			return ImageUtils::readPPM(filename, File, XResolution, YResolution);
			break;
		case TGA_FORMAT:
			return ImageUtils::readTGA(filename, File, XResolution, YResolution);
			break;
		case FLI_FORMAT:
			return ImageUtils::readFLI(filename, File, XResolution, YResolution);
			break;
		case PNG_FORMAT:
			return ImageUtils::readPNG(filename, File, XResolution, YResolution);
			break;
		default:
			cout << filename << " cannot be loaded. Not supported file format." << endl;
			return false;
			break;
		}
	}

	bool ImageUtils::saveFile(const string filename, Color **File, const int XResolution, const int YResolution) {
		cout << XResolution << " x " << YResolution << endl;
		switch (getFileExtension(filename)) {
		case BMP_FORMAT:
			return ImageUtils::saveBMP(filename, File, XResolution, YResolution);
			break;
		case JPG_FORMAT:
			return ImageUtils::saveJPG(filename, File, XResolution, YResolution);
			break;
		case PPM_FORMAT:
			return ImageUtils::savePPM(filename, File, XResolution, YResolution);
			break;
		default:
			cout << filename << " cannot be saved. Not supported file format." << endl;
			break;
		}

		return false;
	}

	bool ImageUtils::readPPM(const string filename, Color ***File, int *XResolution, int *YResolution) {
		FILE* fp;

		int MaxColorValue;

		fp = fopen(filename.c_str(), "r");
		if (fp == NULL)	{
			cerr << "Unable to open map file: " << filename << endl;
			return false;
		}

		int Type;
		double R, G, B;
		int i, j;

		fscanf(fp, "P%d\n", &Type);
		if (Type != 6 && Type != 5) { //Binary
			cerr << "Bad File Format." << endl;
			return false;
		}

		fscanf(fp, "%d %d\n", XResolution, YResolution);
		fscanf(fp, "%d\n", &MaxColorValue);

		(*File) = new Color*[(*XResolution)];
		for (i = 0; i < (*XResolution); i++)
			(*File)[i] = new Color[(*YResolution)];


		for (j = 0; j < (*YResolution); j++)
			for (i = 0; i < (*XResolution); i++) {
				if (Type == 6) {
					R = fgetc(fp) / ((double)MaxColorValue);
					G = fgetc(fp) / ((double)MaxColorValue);
					B = fgetc(fp) / ((double)MaxColorValue);
					File[i][j]->setColor(R, G, B);
				} else {
					R = fgetc(fp) / ((double)MaxColorValue);
					(*File)[i][j].setColor(R, R, R);
				}
			}
		fclose(fp);
		return true;
	}

	bool ImageUtils::savePPM(const string filename, Color **File, const int XResolution, const int YResolution) {
		FILE *fp = fopen((filename.c_str()), "w");

		if (fp != NULL) {
			cout << "Saving to " << filename << endl;

			fprintf(fp, "P6\n");
			fprintf(fp, "%d %d\n", XResolution, YResolution);
			fprintf(fp, "255\n");
			for (int y = YResolution - 1; y >= 0; y--)
				for (int x = 0; x < XResolution; x++)
				{
					fputc((char)(File[x][y].getRed()   * 255), fp);
					fputc((char)(File[x][y].getGreen() * 255), fp);
					fputc((char)(File[x][y].getBlue()  * 255), fp);
				}
			fclose(fp);

			return true;
		}

		return false;
	}


	//////////////////////////////// BMP FILES //////////////////////////////////////////////
	bool ImageUtils::readBMP(const string filename, Color ***File, int *XResolution, int *YResolution) {
		BITMAPFILEHEADER fh;
		BITMAPINFOHEADER ih;

		int cline;
		unsigned char* buffer;
		FILE* file = fopen(filename.c_str(), "rb");

		if (file != NULL) { // file opened
			fread(&fh, sizeof(BITMAPFILEHEADER), 1, file); //reading the FILEHEADER
			fread(&ih, sizeof(BITMAPINFOHEADER), 1, file); //reading the INFOHEADER

			if (ih.biCompression != BI_RGB) {
				cout << "Compressed file: Not supported" << endl;
				return false;
			}
			if (ih.biBitCount != 24) {
				cout << "Not 24 bits/pixel. Not supported";
				return false;
			}

			(*XResolution) = ih.biWidth;
			(*YResolution) = ih.biHeight;
			(*File) = new Color*[(*XResolution)];
			for (int i = 0; i < (*XResolution); i++)
				(*File)[i] = new Color[(*YResolution)];

			int row_padded = ((*XResolution) * 3 + 3) & (~3);
			buffer = new unsigned char[row_padded];
			cline = 0;

			while (cline < ih.biHeight) {
				fread(buffer, row_padded, 1, file);
				for (int i = 0; i < ih.biWidth; i++)
					(*File)[i][cline].setColor(255 - buffer[i * 3 + 2], 255 - buffer[i * 3 + 1], 255 - buffer[i * 3 + 0]);
				cline++;
			}

			fclose(file);
			cout << "Done" << endl;
			return true;
		}

		return false;
	}

	bool ImageUtils::saveBMP(const string filename, Color **File, const int XResolution, const int YResolution) {
		BITMAPFILEHEADER fh;
		BITMAPINFOHEADER ih;
		int row_padded = (XResolution * 3 + 3) & (~3);

		//Preparing data
		fh.bfType      = BF_TYPE;
		fh.bfReserved1 = 0;
		fh.bfReserved2 = 0;
		fh.bfSize      = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + row_padded * YResolution;
		fh.bfOffBits   = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);

		ih.biSize        = sizeof(BITMAPINFOHEADER);
		ih.biWidth       = XResolution;
		ih.biHeight      = YResolution;
		ih.biCompression = BI_RGB;
		ih.biBitCount    = 24;

		FILE* file = fopen(filename.c_str(), "wb");

		if (file != NULL) { // file opened
			cout << "Saving to " << filename << endl;

			fwrite(&fh, sizeof(BITMAPFILEHEADER), 1, file); //reading the FILEHEADER
			fwrite(&ih, sizeof(BITMAPINFOHEADER), 1, file); //reading the INFOHEADER

			unsigned char *buffer = new unsigned char[row_padded];
			int cline = 0;
			while (cline < ih.biHeight) {
				for (int i = 0; i < ih.biWidth; i++) {
					buffer[i * 3 + 2] = (unsigned char)(255 * File[i][cline].getRed()) + 255;
					buffer[i * 3 + 1] = (unsigned char)(255 * File[i][cline].getGreen()) + 255;
					buffer[i * 3 + 0] = (unsigned char)(255 * File[i][cline].getBlue()) + 255;
				}
				fwrite(buffer, row_padded, 1, file);
				cline++;
			}

			delete[] buffer;
			fclose(file);
			return true;
		}

		return false;
	}

	bool ImageUtils::readJPG(const string filename, Color ***File, int *XResolution, int *YResolution) {
		/* This struct contains the JPEG decompression parameters and pointers to
			 * working space (which is allocated as needed by the JPEG library).
			 */
		struct jpeg_decompress_struct cinfo;
		/* We use our private extension JPEG error handler.
		 * Note that this struct must live as long as the main JPEG parameter
		 * struct, to avoid dangling-pointer problems.
		 */
		struct my_error_mgr jerr;
		/* More stuff */
		FILE * infile;        /* source file */
		JSAMPARRAY buffer;        /* Output row buffer */
		int row_stride;       /* physical row width in output buffer */
		/* In this example we want to open the input file before doing anything else,
		 * so that the setjmp() error recovery below can assume the file is open.
		 * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
		 * requires it in order to read binary files.
		 */
		if ((infile = fopen(filename.c_str(), "rb")) == NULL) {
			cerr << "Can't open " << filename << endl;
			cout << "Failed" << endl;
			return false;
		}
		/* Step 1: allocate and initialize JPEG decompression object */
		/* We set up the normal JPEG error routines, then override error_exit. */
		cinfo.err = jpeg_std_error(&jerr.pub);
		jerr.pub.error_exit = my_error_exit;
		/* Establish the setjmp return context for my_error_exit to use. */
		if (setjmp(jerr.setjmp_buffer)) {
			/* If we get here, the JPEG code has signaled an error.
			 * We need to clean up the JPEG object, close the input file, and return.
			 */
			jpeg_destroy_decompress(&cinfo);
			fclose(infile);
			cout << "Failed" << endl;
			return false;
		}
		/* Now we can initialize the JPEG decompression object. */
		jpeg_create_decompress(&cinfo);
		/* Step 2: specify data source (eg, a file) */
		jpeg_stdio_src(&cinfo, infile);
		/* Step 3: read file parameters with jpeg_read_header() */
		(void) jpeg_read_header(&cinfo, TRUE);
		/* We can ignore the return value from jpeg_read_header since
		 *   (a) suspension is not possible with the stdio data source, and
		 *   (b) we passed TRUE to reject a tables-only JPEG file as an error.
		 * See libjpeg.doc for more info.
		 */
		/* Step 4: set parameters for decompression */
		/* In this example, we don't need to change any of the defaults set by
		 * jpeg_read_header(), so we do nothing here.
		 */
		/* Step 5: Start decompressor */
		(void) jpeg_start_decompress(&cinfo);
		/* We can ignore the return value since suspension is not possible
		 * with the stdio data source.
		 */
		/* We may need to do some setup of our own at this point before reading
		 * the data.  After jpeg_start_decompress() we have the correct scaled
		 * output image dimensions available, as well as the output colormap
		 * if we asked for color quantization.
		 * In this example, we need to make an output work buffer of the right size.
		 */
		/* JSAMPLEs per row in output buffer */
		row_stride = cinfo.output_width * cinfo.output_components;
		/* Make a one-row-high sample array that will go away when done with image */
		buffer = (*cinfo.mem->alloc_sarray)
		         ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);
		/* Step 6: while (scan lines remain to be read) */
		/*           jpeg_read_scanlines(...); */
		/* Here we use the library's state variable cinfo.output_scanline as the
		 * loop counter, so that we don't have to keep track ourselves.
		 */
		(*XResolution) = cinfo.output_width;
		(*YResolution) = cinfo.output_height;

		(*File) = new Color*[(*XResolution)];
		for (int i = 0; i < (*XResolution); i++)
			(*File)[i] = new Color[(*YResolution)];

		while (cinfo.output_scanline < cinfo.output_height) {
			/* jpeg_read_scanlines expects an array of pointers to scanlines.
			 * Here the array is only one element long, but you could ask for
			 * more than one scanline at a time if that's more convenient.
			 */
			(void) jpeg_read_scanlines(&cinfo, buffer, 1);

			for (int i = 0; i < (*XResolution); i++)
				(*File)[i][(*YResolution) - cinfo.output_scanline].setColor(buffer[0][3 * i] / 256.0, buffer[0][3 * i + 1] / 256.0, buffer[0][3 * i + 2] / 256.0);
		}
		/* Step 7: Finish decompression */
		(void) jpeg_finish_decompress(&cinfo);
		/* We can ignore the return value since suspension is not possible
		 * with the stdio data source.
		 */
		/* Step 8: Release JPEG decompression object */
		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_decompress(&cinfo);
		/* After finish_decompress, we can close the input file.
		 * Here we postpone it until after no more JPEG errors are possible,
		 * so as to simplify the setjmp error logic above.  (Actually, I don't
		 * think that jpeg_destroy can do an error exit, but why assume anything...)
		 */
		fclose(infile);
		/* At this point you may want to check to see whether any corrupt-data
		 * warnings occurred (test whether jerr.pub.num_warnings is nonzero).
		 */
		/* And we're done! */
		cout << "Done" << endl;
		return true;
	}

	bool ImageUtils::saveJPG(const string filename, Color **File, const int XResolution, const int YResolution) {

		JSAMPLE * image_buffer;
		image_buffer = new JSAMPLE[XResolution * 3];

		/* This struct contains the JPEG compression parameters and pointers to
		 * working space (which is allocated as needed by the JPEG library).
		 * It is possible to have several such structures, representing multiple
		 * compression/decompression processes, in existence at once.  We refer
		 * to any one struct (and its associated working data) as a "JPEG object".
		 */
		struct jpeg_compress_struct cinfo;
		/* This struct represents a JPEG error handler.  It is declared separately
		 * because applications often want to supply a specialized error handler
		 * (see the second half of this file for an example).  But here we just
		 * take the easy way out and use the standard error handler, which will
		 * print a message on stderr and call exit() if compression fails.
		 * Note that this struct must live as long as the main JPEG parameter
		 * struct, to avoid dangling-pointer problems.
		 */
		struct jpeg_error_mgr jerr;
		/* More stuff */
		FILE * outfile;       /* target file */
		/* Step 1: allocate and initialize JPEG compression object */
		/* We have to set up the error handler first, in case the initialization
		 * step fails.  (Unlikely, but it could happen if you are out of memory.)
		 * This routine fills in the contents of struct jerr, and returns jerr's
		 * address which we place into the link field in cinfo.
		 */
		cinfo.err = jpeg_std_error(&jerr);
		/* Now we can initialize the JPEG compression object. */
		jpeg_create_compress(&cinfo);
		/* Step 2: specify data destination (eg, a file) */
		/* Note: steps 2 and 3 can be done in either order. */
		/* Here we use the library-supplied code to send compressed data to a
		 * stdio stream.  You can also write your own code to do something else.
		 * VERY IMPORTANT: use "b" option to fopen() if you are on a machine that
		 * requires it in order to write binary files.
		 */
		if ((outfile = fopen(filename.c_str(), "wb")) == NULL) {
			fprintf(stderr, "can't open %s\n", filename.c_str());
			return false;
		}
		cout << "Saving to " << filename << endl;
		jpeg_stdio_dest(&cinfo, outfile);
		/* Step 3: set parameters for compression */
		/* First we supply a description of the input image.
		 * Four fields of the cinfo struct must be filled in:
		 */
		cinfo.image_width = XResolution;  /* image width and height, in pixels */
		cinfo.image_height = YResolution;
		cinfo.input_components = 3;       /* # of color components per pixel */
		cinfo.in_color_space = JCS_RGB;   /* colorspace of input image */
		/* Now use the library's routine to set default compression parameters.
		 * (You must set at least cinfo.in_color_space before calling this,
		 * since the defaults depend on the source color space.)
		 */
		jpeg_set_defaults(&cinfo);
		/* Now you can set any non-default parameters you wish to.
		 * Here we just illustrate the use of quality (quantization table) scaling:
		 */
		jpeg_set_quality(&cinfo, 100, TRUE /* limit to baseline-JPEG values */);
		/* Step 4: Start compressor */
		/* TRUE ensures that we will write a complete interchange-JPEG file.
		 * Pass TRUE unless you are very sure of what you're doing.
		 */
		jpeg_start_compress(&cinfo, TRUE);
		/* Step 5: while (scan lines remain to be written) */
		/*           jpeg_write_scanlines(...); */
		/* Here we use the library's state variable cinfo.next_scanline as the
		 * loop counter, so that we don't have to keep track ourselves.
		 * To keep things simple, we pass one scanline per call; you can pass
		 * more if you wish, though.
		 */

		while (cinfo.next_scanline < cinfo.image_height) {
			/* jpeg_write_scanlines expects an array of pointers to scanlines.
			 * Here the array is only one element long, but you could pass
			 * more than one scanline at a time if that's more convenient.
			 */
			for (int i = 0; i < XResolution; i++) {
				image_buffer[i * 3]     = (unsigned char)(File[i][cinfo.image_height - cinfo.next_scanline - 1].getRed()   * 255);
				image_buffer[i * 3 + 1] = (unsigned char)(File[i][cinfo.image_height - cinfo.next_scanline - 1].getGreen() * 255);
				image_buffer[i * 3 + 2] = (unsigned char)(File[i][cinfo.image_height - cinfo.next_scanline - 1].getBlue()  * 255);
			}

			(void) jpeg_write_scanlines(&cinfo, &image_buffer, 1);
		}

		/* Step 6: Finish compression */
		jpeg_finish_compress(&cinfo);
		/* After finish_compress, we can close the output file. */
		fclose(outfile);
		/* Step 7: release JPEG compression object */
		/* This is an important step since it will release a good deal of memory. */
		jpeg_destroy_compress(&cinfo);

		delete image_buffer;
		return true;
	}

	bool ImageUtils::readTGA(const string filename, Color ***File, int *XResolution, int *YResolution) {
		TGA *tga;
		TGAData *data;

		data = (TGAData*)malloc(sizeof(TGAData));
		//data = new TGAData;
		if (!data) {
			//TGA_ERROR((TGA*)NULL, TGA_OOM);
			return 0;
		}

		tga = TGAOpen(filename.c_str(), "r");
		if (!tga || tga->last != TGA_OK) {
			//TGA_ERROR(tga, TGA_OPEN_FAIL);
			cout << "Failed" << endl;
			return false;
		}

		data->flags = TGA_IMAGE_INFO | TGA_IMAGE_ID | TGA_IMAGE_DATA | TGA_RGB;
		if (TGAReadImage(tga, data) != TGA_OK) {
			//TGA_ERROR(tga, TGA_READ_FAIL);
			cout << "Failded" << endl;
			return false;
		}

		if (data->flags & TGA_IMAGE_INFO) {
			(*XResolution) = tga->hdr.width;
			(*YResolution) = tga->hdr.height;

			(*File) = new Color*[(*XResolution)];
			for (int i = 0; i < (*XResolution); i++)
				(*File)[i] = new Color[(*YResolution)];

			int step = tga->hdr.depth / 8;
			int x, y;
			for (y = 0; y < (*YResolution); y++) {
				for (x = 0; x < (*XResolution); x++) {
					(*File)[x][y].setColor(
					    data->img_data[y * (*XResolution)*step + step * x + 0] / 255.0,
					    data->img_data[y * (*XResolution)*step + step * x + 1] / 255.0,
					    data->img_data[y * (*XResolution)*step + step * x + 2] / 255.0
					);
				}
			}

		}
		cout << "Done" << endl;
		return true;
	}

	bool ImageUtils::readFLI(const string filename, Color ***File, int *XResolution, int *YResolution) {
		FlicImg img(filename);
		CEL_COLOR *map = img.getMap();

		if(map == NULL){
			cout << "Failed" << endl;
			return false;
		}

		*(XResolution) = img.getWidth();
		*(YResolution) = img.getHeight();

		(*File) = new Color*[(*XResolution)];
		for (int i = 0; i < (*XResolution); i++)
			(*File)[i] = new Color[(*YResolution)];


		int x, y;
		for (y = 0; y < (*YResolution); y++) {
			for (x = 0; x < (*XResolution); x++) {
				(*File)[x][y].setColor(
				    map[y * (*XResolution) + ((*XResolution) - 1 - x)].red   / 255.0,
				    map[y * (*XResolution) + ((*XResolution) - 1 - x)].green / 255.0,
				    map[y * (*XResolution) + ((*XResolution) - 1 - x)].blue  / 255.0
				);
			}
		}

		cout << "Done" << endl;
		return true;
	}

	bool ImageUtils::readPNG(const string filename, Color ***File, int *XResolution, int *YResolution) {
		std::vector<unsigned char> image; //the raw pixels
		unsigned width, height;

		//decode
		unsigned error = lodepng::lodepng::decode(image, width, height, filename);

		//if there's an error, display it
		if (error){
			std::cout << "decoder error " << error << ": " << lodepng::lodepng_error_text(error) << std::endl;
			return false;
		}

		(*XResolution) = width;
		(*YResolution) = height;

		(*File) = new Color*[(*XResolution)];
		for (int i = 0; i < (*XResolution); i++)
			(*File)[i] = new Color[(*YResolution)];

		int x, y;
		for (y = 0; y < (*YResolution); y++) {
			for (x = 0; x < (*XResolution); x++) {
				(*File)[x][(*YResolution) - y - 1].setColor(
					image.at(y * (*XResolution)*4 + 4 * x + 0) / 255.0,
					image.at(y * (*XResolution)*4 + 4 * x + 1) / 255.0,
					image.at(y * (*XResolution)*4 + 4 * x + 2) / 255.0
				);
			}
		}

		cout << "Done" << endl;
		return true;
	}

	bool ImageUtils::readGIF(const string filename, Color ***File, int *XResolution, int *YResolution) {
		int error;
	    GifFileType* gifFile = DGifOpenFileName(filename.c_str(), &error);
	    if (!gifFile) {
	        std::cout << "DGifOpenFileName() failed - " << error << std::endl;
	        return false;
	    }
	    if (DGifSlurp(gifFile) == GIF_ERROR) {
	        std::cout << "DGifSlurp() failed - " << gifFile->Error << std::endl;
	        DGifCloseFile(gifFile, &error);
	        return false;
	    }

	    ColorMapObject* commonMap = gifFile->SColorMap;
	    std::cout << filename << ": " << gifFile->SWidth << "x" << gifFile->SHeight << std::endl;

	    for (int i = 0; i < 1; ++i) {
	        const SavedImage& saved = gifFile->SavedImages[i];
	        const GifImageDesc& desc = saved.ImageDesc;
	        const ColorMapObject* colorMap = desc.ColorMap ? desc.ColorMap : commonMap;
	      
	        (*XResolution) = desc.Width;
			(*YResolution) = desc.Height;

			(*File) = new Color*[(*XResolution)];
			for (int i = 0; i < (*XResolution); i++)
				(*File)[i] = new Color[(*YResolution)];

	        std::stringstream ss;
	        for (int v = 0; v < desc.Height; ++v) {
	            for (int u = 0; u < desc.Width; ++u) {
	                int c = saved.RasterBits[v * desc.Width + u];
	                if (colorMap) {
	                    GifColorType rgb = colorMap->Colors[c];
	     				(*File)[u][v].setColor(rgb.Red/255.0,rgb.Green/255.0,rgb.Blue/255.0);
	                }
	            }
	        }
	    }

	    DGifCloseFile(gifFile, &error);
	    return true;
	}
}