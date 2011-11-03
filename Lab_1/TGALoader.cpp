#include "TGALoader.h"
#include "SDL_opengl.h"
#include <stdio.h>

bool TGALoader::loadTGA( char *fileName, TGAImage *tgaImage ) {
	FILE		*fTGA;				// File pointer to texture file
	TGAHeader	header;
	
	fTGA = fopen(fileName, "rb");	// Open file for reading
	if(fTGA == NULL) return false;	// If it didn't open, exit function

	if(fread(&header, sizeof(TGAHeader), 1, fTGA) == 0) {				// Attempt to read 12 byte header from file
		if(fTGA != NULL) fclose(fTGA);									// Check to seeiffile is still open. If it is, close it
		return false;													// Exit function
	}

	tgaImage->initialize( header );

	// Validate header info
	if( !tgaImage->isValid() || tgaImage->image_ == NULL ) {
		// invalid header, bomb out
		fclose( fTGA );
		return false;
	}

	// Skip image ident field
	if( header.ImgIdent > 0 )
		fseek(fTGA, header.ImgIdent, SEEK_CUR);

	// un-compresses image ?
	if (tgaImage->imageType_ == 2) {
		if(fread(tgaImage->image_, 1, tgaImage->imageSize_, fTGA) != tgaImage->imageSize_) {			// Attempt to read image data
			fclose(fTGA);												// Close file
			return false;												// Return failed
		}

		// Byte Swapping Optimized By Steve Thomas
		for(GLuint cswap = 0; cswap < tgaImage->imageSize_; cswap += tgaImage->imageBytesPerPel_) {
			tgaImage->image_[cswap] ^= tgaImage->image_[cswap+2] ^=
			tgaImage->image_[cswap] ^= tgaImage->image_[cswap+2];
		}
	} else {
		// compressed image
		GLuint pixelcount	= tgaImage->imageHeight_ * tgaImage->imageWidth_;	// Nuber of pixels in the image
		GLuint currentpixel	= 0;												// Current pixel being read
		GLuint currentbyte	= 0;												// Current byte
		GLubyte *colorbuffer = new GLubyte[tgaImage->imageBytesPerPel_];		// Storage for 1 pixel

		do {
			GLubyte chunkheader = 0;									// Storage for "chunk" header
			if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0) {	// Read in the 1 byte header
				if(fTGA != NULL) fclose(fTGA);							// If file is open, close file
				return false;											// Return failed
			}

			if(chunkheader < 128) {											// If the header is < 128, it means the that is the number of RAW color packets minus 1 that follow the header
				chunkheader++;												// add 1 to get number of following color values
				for(short counter = 0; counter < chunkheader; counter++) {	// Read RAW color values
					if(fread(colorbuffer, 1, tgaImage->imageBytesPerPel_, fTGA) != tgaImage->imageBytesPerPel_) { // Try to read 1 pixel
						if(fTGA != NULL) fclose(fTGA);					// See if file is open. If so, close file
						if(colorbuffer != NULL) delete[] colorbuffer;	// See if colorbuffer has data in it. If so, delete it
						return false;									// Return failed
					}
																				// write to memory
					tgaImage->image_[currentbyte		] = colorbuffer[2];		// Flip R and B vcolor values around in the process
					tgaImage->image_[currentbyte + 1	] = colorbuffer[1];
					tgaImage->image_[currentbyte + 2	] = colorbuffer[0];

					if (tgaImage->imageBytesPerPel_ == 4)						// if its a 32 bpp image
						tgaImage->image_[currentbyte + 3] = colorbuffer[3];		// copy the 4th byte

					currentbyte += tgaImage->imageBytesPerPel_;			// Increase thecurrent byte by the number of bytes per pixel
					currentpixel++;										// Increase current pixel by 1

					if(currentpixel > pixelcount) {						// Make sure we havent read too many pixels
						if(fTGA != NULL) fclose(fTGA);					// If there is a file open, close file
						if(colorbuffer != NULL)	delete[] colorbuffer;	// If there is data in colorbuffer, delete it
						return false;									// Return failed
					}
				}
			}
			else														// chunkheader > 128 RLE data, next color reapeated chunkheader - 127 times
			{
				chunkheader -= 127;										// Subtract 127 to get rid of the ID bit
				if(fread(colorbuffer, 1, tgaImage->imageBytesPerPel_, fTGA) != tgaImage->imageBytesPerPel_) {	// Attempt to read following color values
					if(fTGA != NULL) fclose(fTGA);						// If thereis a file open
					if(colorbuffer != NULL) delete[] colorbuffer;		// If there is data in the colorbuffer
					return FALSE;										// return failed
				}

				for(short counter = 0; counter < chunkheader; counter++) {	// copy the color into the image data as many times as dictated by the header
					tgaImage->image_[currentbyte		] = colorbuffer[2];	// switch R and B bytes areound while copying
					tgaImage->image_[currentbyte + 1	] = colorbuffer[1];
					tgaImage->image_[currentbyte   + 2	] = colorbuffer[0];

					if(tgaImage->imageBytesPerPel_ == 4)						// If TGA images is 32 bpp
						tgaImage->image_[currentbyte + 3] = colorbuffer[3];		// Copy 4th byte

					currentbyte += tgaImage->imageBytesPerPel_;			// Increase current byte by the number of bytes per pixel
					currentpixel++;										// Increase pixel count by 1

					if(currentpixel > pixelcount) {						// Make sure we havent written too many pixels
						if(fTGA != NULL) fclose(fTGA);					// If there is a file open, close file
						if(colorbuffer != NULL)	delete[] colorbuffer;	// If there is data in colorbuffer, delete it
						return false;									// Return failed
					}
				} // for(counter)
			} // if(chunkheader)
		}
		while(currentpixel < pixelcount);								// Loop while there are still pixels left
	} // if (tgaImage->imageType_ == 2)

	fclose (fTGA);
	return true;
};
