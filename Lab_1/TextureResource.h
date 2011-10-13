#ifndef _TEXTURERESOURCE_H_
#define _TEXTURERESOURCE_H_

#include "resource.h"
#include <string>
#include "SDL_opengl.h"

// Define Interface Data Types / Structures
//
typedef enum {
	txUnknown	= 0,	// images
	txBmp		= 1,
	txJpg		= 2,
	txPng		= 3,
	txTga		= 4,
	txGif		= 5,
	txIco		= 6,
	txEmf		= 7,
	txWmf		= 8,
	// add new ones at the end
} eglTexType;

typedef enum {
	txNoFilter	= 0,
	txBilinear	= 1,
	txTrilinear	= 2,
	// add new ones at the end
} eglTexFilterType;

typedef	struct
{
	GLuint		TextureID;									// Texture ID Used To Select A Texture
	eglTexType	TexType;									// Texture Format
	GLuint		Width;										// Image Width
	GLuint		Height;										// Image Height
	GLuint		Type;										// Image Type (GL_RGB, GL_RGBA)
	GLuint		Bpp;										// Image Color Depth In Bits Per Pixel
} glTexture;

typedef struct {
	float	s;
	float	t;
} _glTexturCord;


typedef struct {
	_glTexturCord TopRight;
	_glTexturCord TopLeft;
	_glTexturCord BottomLeft;
	_glTexturCord BottomRight;
} glTexturCordTable;

// Define Private Structurs
//

// Header
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
} _TGAHeader;

class TextureResource :
	public Resource
{
private:
	// variables
	glTexture			*pglTexture;

	GLboolean			m_fAlphaConversion;
	GLboolean			m_fHighQualityTextures;
	GLboolean			m_fMipMapping;
	eglTexFilterType	m_TextureFilterType;

	GLubyte				m_RedAlphaMatch;
	GLubyte				m_GreenAlphaMatch;
	GLubyte 			m_BlueAlphaMatch;

	int TextureResource::LoadTGAFromDisk(char *pszFileName, glTexture *pglTexture);
	int BuildTexture(char *szPathName, glTexture *pglTexture);
	int GenerateTexture(glTexture *pglTexture, GLubyte *pImgData);
	void ExtensionFromFilename(char *szFileName, char *szExtension);
	//char *TextureResource::ExtensionFromFilename(char *szFileName/*, char szExtension[]*/);

public:

	TextureResource(const int handle, char* szFullPath);
	~TextureResource(void);

	inline glTexture* getTexture() { return this->pglTexture; }

	void SetAlphaMatch(GLboolean fEnabled, GLubyte RedAlphaMatch, GLubyte GreenAlphaMatch, GLubyte BlueAlphaMatch);
	void SetHighQualityTextures(GLboolean fEnabled);
	void SetMipMapping(GLboolean fEnabled);
	void SetTextureFilter(eglTexFilterType type);

	void FreeTexture(glTexture *pglTexture);
};

#endif