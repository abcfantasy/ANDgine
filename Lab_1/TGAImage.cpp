#include "TGAImage.h"
#include "SDL_opengl.h"
#include <stdio.h>

// Loads the image from a file
TGAImage::TGAImage( const int handle, char *fileName ) : Resource( handle, fileName ) {
	this->load();
};

// Frees the memory
TGAImage::~TGAImage() {
	if( this->image_ != NULL ) {
		delete[] this->image_;
		this->image_ = NULL;
	}
};

// Reads the image from the file
// This is called in the constructor
bool TGAImage::load() {
	FILE		*fTGA;											// File pointer to the image
	TGAHeader	header;											// The header of the file
	
	fopen_s( &fTGA, this->getFilename(), "rb");					// Open file for reading
	if(fTGA == NULL) return false;								// If it didn't open, exit function

	if(fread(&header, sizeof(TGAHeader), 1, fTGA) == 0) {		// Attempt to read 12 byte header from file
		if(fTGA != NULL) fclose(fTGA);							// Check to see if file is still open. If it is, close it
		return false;											// Exit function
	}

	// Set the header info
	this->initialize( &header );

	// Validate header info
	if( !this->isValid() || this->image_ == NULL ) {
		// invalid header, bomb out
		fclose( fTGA );
		return false;
	}

	// Skip image ident field
	if( header.ImgIdent > 0 )
		fseek(fTGA, header.ImgIdent, SEEK_CUR);

	// un-compresses image ?
	if (this->type_ == 2) {
		if(fread(this->image_, 1, this->size_, fTGA) != this->size_) {	// Attempt to read image data
			fclose(fTGA);												// Close file
			return false;												// Return failed
		}

		// Byte Swapping Optimized By Steve Thomas
		for(GLuint cswap = 0; cswap < this->size_; cswap += this->bytesPerPx_) {
			this->image_[cswap] ^= this->image_[cswap+2] ^=
			this->image_[cswap] ^= this->image_[cswap+2];
		}
	} else {
		// compressed image
		GLuint pixelcount	= this->height_ * this->width_;				// Number of pixels in the image
		GLuint currentpixel	= 0;										// Current pixel being read
		GLuint currentbyte	= 0;										// Current byte
		GLubyte *colorbuffer = new GLubyte[this->bytesPerPx_];			// Storage for 1 pixel

		do {
			GLubyte chunkheader = 0;									// Storage for "chunk" header
			if(fread(&chunkheader, sizeof(GLubyte), 1, fTGA) == 0) {	// Read in the 1 byte header
				if(fTGA != NULL) fclose(fTGA);							// If file is open, close file
				return false;											// Return failed
			}

			if(chunkheader < 128) {																// If the header is < 128, it means the that is the number of RAW color packets minus 1 that follow the header
				chunkheader++;																	// add 1 to get number of following color values
				for(short counter = 0; counter < chunkheader; counter++) {						// Read RAW color values
					if(fread(colorbuffer, 1, this->bytesPerPx_, fTGA) != this->bytesPerPx_) {	// Try to read 1 pixel
						if(fTGA != NULL) fclose(fTGA);					// See if file is open. If so, close file
						if(colorbuffer != NULL) delete[] colorbuffer;	// See if colorbuffer has data in it. If so, delete it
						return false;									// Return failed
					}
																		// write to memory
					this->image_[currentbyte	] = colorbuffer[2];		// Flip R and B vcolor values around in the process
					this->image_[currentbyte + 1] = colorbuffer[1];
					this->image_[currentbyte + 2] = colorbuffer[0];

					if (this->bytesPerPx_ == 4)							// if its a 32 bpp image
						this->image_[currentbyte + 3] = colorbuffer[3];	// copy the 4th byte

					currentbyte += this->bytesPerPx_;					// Increase the current byte by the number of bytes per pixel
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
				chunkheader -= 127;															// Subtract 127 to get rid of the ID bit
				if(fread(colorbuffer, 1, this->bytesPerPx_, fTGA) != this->bytesPerPx_) {	// Attempt to read following color values
					if(fTGA != NULL) fclose(fTGA);						// If thereis a file open
					if(colorbuffer != NULL) delete[] colorbuffer;		// If there is data in the colorbuffer
					return false;										// return failed
				}

				for(short counter = 0; counter < chunkheader; counter++) {	// copy the color into the image data as many times as dictated by the header
					this->image_[currentbyte		] = colorbuffer[2];		// switch R and B bytes areound while copying
					this->image_[currentbyte + 1	] = colorbuffer[1];
					this->image_[currentbyte   + 2	] = colorbuffer[0];

					if(this->bytesPerPx_ == 4)							// If TGA images is 32 bpp
						this->image_[currentbyte + 3] = colorbuffer[3];	// Copy 4th byte

					currentbyte += this->bytesPerPx_;				// Increase current byte by the number of bytes per pixel
					currentpixel++;										// Increase pixel count by 1

					if(currentpixel > pixelcount) {						// Make sure we havent written too many pixels
						if(fTGA != NULL) fclose(fTGA);					// If there is a file open, close file
						if(colorbuffer != NULL)	delete[] colorbuffer;	// If there is data in colorbuffer, delete it
						return false;									// Return failed
					}
				} // for(counter)
			} // if(chunkheader)
		} while(currentpixel < pixelcount);								// Loop while there are still pixels left
	} // if (tgaImage->type_ == 2)

	fclose (fTGA);
	return true;
};

// Initializes the image from the header
// This is called during load()
void TGAImage::initialize( TGAHeader *header ) {
	this->type_			= header->ImgType;
	this->width_		= header->WidthLo  + header->WidthHi  * 256;
	this->height_		= header->HeightLo + header->HeightHi * 256;
	this->bytesPerPx_	= header->Bpp / 8;
	this->size_			= this->width_ * this->height_ * this->bytesPerPx_;
	this->image_		= new GLubyte[ this->size_ ];
};

// Checks to see if the header is valid
bool TGAImage::isValid() {
	if( ( this->type_ != 2 && this->type_ != 10 ) ||
	    ( this->width_ == 0 ) || ( this->height_ == 0 ) ||
		( this->bytesPerPx_ != 3 && this->bytesPerPx_ != 4 ) )
		return false;
	return true;
};