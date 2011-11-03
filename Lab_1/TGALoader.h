#ifndef _TGALOADER_H_
#define _TGALOADER_H_

#include "TGAImage.h"

class TGALoader {
public:
	static bool loadTGA( char *fileName, TGAImage *tgaImage );
};

#endif