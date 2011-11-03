#ifndef _TGAIMAGE_H_
#define _TGAIMAGE_H_

#include "Resource.h"
#include "SDL_opengl.h"

// TGA Header structure; it's not used anywhere else
typedef struct {
	unsigned char ImgIdent;
	unsigned char ignored[ 1 ];
	unsigned char ImgType;
	unsigned char ignored2[ 9 ];
	unsigned char WidthLo;
	unsigned char WidthHi;
	unsigned char HeightLo;
	unsigned char HeightHi;
	unsigned char Bpp;
	unsigned char ignored3[ 1 ];
} TGAHeader;

// Class that loads and manages a TGA image
// Since it's a resource, it will go through the Resource Manager
class TGAImage : Resource {
public:
	unsigned int type_;
	unsigned int width_;
	unsigned int height_;
	unsigned int bytesPerPx_;
	unsigned int size_;
	GLubyte *image_;

	// Loads the image from a file
	TGAImage( const int handle, char *fileName );
	// Frees the memory
	~TGAImage();

	// Reads the image from the file
	// This is called in the constructor
	bool load();
	// Initializes the image from the header
	// This is called during load()
	void initialize( TGAHeader *header );
	// Checks to see if the header is valid
	bool isValid();
};

#endif