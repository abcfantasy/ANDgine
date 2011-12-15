#include "TextureResource.h"
#include "ResourceManager.h"
#include "TGAImage.h"

#include <olectl.h>											// Header File For The OLE Controls Library

TextureResource::TextureResource(const int handle, char* szFileName )
		: Resource(handle, szFileName)
{
	// default to alpha matching BLACK
	SetAlphaMatch(TRUE, 0, 0, 0);										// Set the Alpha Matching State

	// default to full sized textures
	SetHighQualityTextures(TRUE);

	// no mipmap textures
	SetMipMapping(FALSE);

	// no texture filtering
	SetTextureFilter(txNoFilter);

	// load from disk
	char szFullPath[MAX_PATH+1];										// Full Path To Picture
	char szExtension[16];												// Extenstion of Picture

	pglTexture = new glTexture();
	//if (std::strstr(path, "http://"))									// If PathName Contains http:// Then...
	//{
	//	strcpy(szFullPath, szFileName);									// Append The PathName To FullPath
	//}
	//else																// Otherwise... We Are Loading From A File
	//{
		GetCurrentDirectory(MAX_PATH, szFullPath);						// Get Our Working Directory
		strcat_s(szFullPath, (MAX_PATH + 1), "\\");										// Append "\" After The Working Directory
		strcat_s(szFullPath, (MAX_PATH + 1), szFileName);									// Append The PathName
	//}

	ExtensionFromFilename(szFileName, szExtension);
	
	// if the file is a TGA then use the TGA file loader
	if (lstrcmpi(szExtension, "tga") == 0)
	{
		LoadTGAFromDisk(szFullPath, pglTexture);				// Load TGA (Compressed/Uncompressed)
	}
	else
	{
		// else load BMP, JPG, GIF
		
		pglTexture->TexType = txUnknown;
		if (lstrcmpi(szExtension, "bmp") == 0)
		{
			pglTexture->TexType = txBmp;
		}
		else if ((lstrcmpi(szExtension, "jpg") == 0) ||
				 (lstrcmpi(szExtension, "jpeg") == 0) )
		{
			pglTexture->TexType = txJpg;
		}
		else if (lstrcmpi(szExtension, "gif") == 0)
		{
			pglTexture->TexType = txGif;
		}
		else if (lstrcmpi(szExtension, "ico") == 0)
		{
			pglTexture->TexType = txIco;
		}
		else if (lstrcmpi(szExtension, "emf") == 0)
		{
			pglTexture->TexType = txEmf;
		}
		else if (lstrcmpi(szExtension, "wmf") == 0)
		{
			pglTexture->TexType = txWmf;
		}

		BuildTexture(szFullPath, pglTexture);					// Load BMP, JPG, GIF etc..
	}
}


TextureResource::~TextureResource(void)
{
	glDeleteTextures(1, &pglTexture->TextureID);
}


// Set Alpha Matching State and Match Colour
//
void TextureResource::SetAlphaMatch(GLboolean fEnabled, GLubyte RedAlphaMatch, GLubyte GreenAlphaMatch, GLubyte BlueAlphaMatch)
{
	m_fAlphaConversion	= fEnabled;
	// only set the colour match if the conversion is enabled
	if (fEnabled == TRUE)
	{
		m_RedAlphaMatch 	= RedAlphaMatch;
		m_GreenAlphaMatch 	= GreenAlphaMatch;
		m_BlueAlphaMatch 	= BlueAlphaMatch;
	}
}


// set the high quality texture flag
//
void TextureResource::SetHighQualityTextures(GLboolean fEnabled)
{
	m_fHighQualityTextures = fEnabled;
}


// set the mipmapping flag
//
void TextureResource::SetMipMapping(GLboolean fEnabled)
{
	m_fMipMapping = fEnabled;
}


// set the texture filtering flag
//
void TextureResource::SetTextureFilter(eglTexFilterType type)
{
	m_TextureFilterType = type;
}

// Load a TGA file
//
int TextureResource::LoadTGAFromDisk(char *pszFileName, glTexture *pglTexture)
{
	TGAImage *tgaImage = ResourceManager::instance()->get<TGAImage>( pszFileName );
	if( tgaImage == NULL ) return false;
	GLint	glMaxTexDim;	// Holds Maximum Texture Size

	pglTexture->Width  = tgaImage->width_;
	pglTexture->Height = tgaImage->height_;
	pglTexture->Bpp	   = tgaImage->bytesPerPx_ * 8;

	if(pglTexture->Bpp == 24) pglTexture->Type = GL_RGB;	// If the BPP of the image is 24 set Image type to GL_RGB
	else pglTexture->Type = GL_RGBA;						// Else if its 32 BPP set image type to GL_RGBA

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);		// Get Maximum Texture Size Supported

	int lWidthPixels  = tgaImage->width_;
	int lHeightPixels = tgaImage->height_;

	// Resize Image To Closest Power Of Two
	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
		lWidthPixels = glMaxTexDim;

	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
		lHeightPixels = glMaxTexDim;

	// if low quality textures then make them halve the size which saved 4 times the texture space
	if ((m_fHighQualityTextures == FALSE) && (lWidthPixels > 64)) {
		lWidthPixels /= 2;
		lHeightPixels /= 2;
	}

	// if the size needs to change, the rescale the raw image data
	if ( (lWidthPixels  != (int)tgaImage->width_)	&&
		 (lHeightPixels != (int)tgaImage->height_) ) {
		// allocated the some memory for the new texture
		GLubyte	*pNewImgData = new GLubyte[ lWidthPixels * lHeightPixels * tgaImage->bytesPerPx_ ];

		GLenum format;
		if (tgaImage->bytesPerPx_ == 4) format = GL_RGBA;
		else format = GL_RGB;

		gluScaleImage(format, tgaImage->width_, tgaImage->height_, GL_UNSIGNED_BYTE, tgaImage->image_,
							  lWidthPixels, lHeightPixels, GL_UNSIGNED_BYTE, pNewImgData);

		// free the original image data
		delete[] tgaImage->image_;

		// old becomes new..
		tgaImage->image_ = pNewImgData;

		// update our texture structure
		pglTexture->Width  = lWidthPixels;
		pglTexture->Height = lHeightPixels;
	}

	// Typical Texture Generation Using Data From The TGA loader
	glGenTextures(1, &pglTexture->TextureID);						// Create The Texture

	// generate the texture using the filtering model selected
	(void)GenerateTexture(pglTexture, (BYTE *)tgaImage->image_);

	ResourceManager::instance()->remove( tgaImage );

	return true;	// All went well, continue on
}

// Load BMP, GIF and JPG and Convert To A Texture
//
int TextureResource::BuildTexture(char *szPathName, glTexture *pglTexture)
{
	HDC			hdcTemp;												// The DC To Hold Our Bitmap
	HBITMAP		hbmpTemp;												// Holds The Bitmap Temporarily
	IPicture	*pPicture;												// IPicture Interface
	OLECHAR		wszPath[MAX_PATH+1];									// Full Path To Picture (WCHAR)
	long		lWidth;													// Width In Logical Units
	long		lHeight;												// Height In Logical Units
	long		lWidthPixels;											// Width In Pixels
	long		lHeightPixels;											// Height In Pixels
	GLint		glMaxTexDim;											// Holds Maximum Texture Size

	MultiByteToWideChar(CP_ACP, 0, szPathName, -1, wszPath, MAX_PATH);	// Convert From ASCII To Unicode
	HRESULT hr = OleLoadPicturePath(wszPath,							// Path
									NULL,								// punkCaller
									0,									// Reserved
									0,									// Reserved Transparent Colour
									IID_IPicture,						// riid of Inertface to return
									(void**)&pPicture);					// pointer to returned interface

	if(FAILED(hr))														// If Loading Failed
	{
		return FALSE;													// Return False
	}

	hdcTemp = CreateCompatibleDC(GetDC(0));								// Create The Windows Compatible Device Context
	if(!hdcTemp)														// Did Creation Fail?
	{
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &glMaxTexDim);					// Get Maximum Texture Size Supported

	pPicture->get_Width(&lWidth);										// Get IPicture Width (Convert To Pixels)
	lWidthPixels	= MulDiv(lWidth, GetDeviceCaps(hdcTemp, LOGPIXELSX), 2540);
	pPicture->get_Height(&lHeight);										// Get IPicture Height (Convert To Pixels)
	lHeightPixels	= MulDiv(lHeight, GetDeviceCaps(hdcTemp, LOGPIXELSY), 2540);

	// Resize Image To Closest Power Of Two
	if (lWidthPixels <= glMaxTexDim) // Is Image Width Less Than Or Equal To Cards Limit
	{
		lWidthPixels = 1 << (int)floor((log((double)lWidthPixels)/log(2.0f)) + 0.5f);
	}
	else  // Otherwise  Set Width To "Max Power Of Two" That The Card Can Handle
	{
		lWidthPixels = glMaxTexDim;
	}

	if (lHeightPixels <= glMaxTexDim) // Is Image Height Greater Than Cards Limit
	{
		lHeightPixels = 1 << (int)floor((log((double)lHeightPixels)/log(2.0f)) + 0.5f);
	}
	else  // Otherwise  Set Height To "Max Power Of Two" That The Card Can Handle
	{
		lHeightPixels = glMaxTexDim;
	}

	// if low quality textures then make them halve the size which saved 4 times the texture space
	if ((m_fHighQualityTextures == FALSE) && (lWidthPixels > 64))
	{
		lWidthPixels /= 2;
		lHeightPixels /= 2;
	}

	//	Create A Temporary Bitmap
	BITMAPINFO	bi = {0};												// The Type Of Bitmap We Request
	DWORD		*pBits = 0;												// Pointer To The Bitmap Bits

	bi.bmiHeader.biSize			= sizeof(BITMAPINFOHEADER);				// Set Structure Size
	bi.bmiHeader.biBitCount		= 32;									// 32 Bit
	bi.bmiHeader.biWidth		= lWidthPixels;							// Power Of Two Width
	bi.bmiHeader.biHeight		= lHeightPixels;						// Make Image Top Up (Positive Y-Axis)
	bi.bmiHeader.biCompression	= BI_RGB;								// RGB Encoding
	bi.bmiHeader.biPlanes		= 1;									// 1 Bitplane

	//	Creating A Bitmap This Way Allows Us To Specify Color Depth And Gives Us Imediate Access To The Bits
	hbmpTemp = CreateDIBSection(hdcTemp,
								&bi,
								DIB_RGB_COLORS,
								(void**)&pBits,
								0,
								0);

	if(!hbmpTemp)														// Did Creation Fail?
	{
		DeleteDC(hdcTemp);												// Delete The Device Context
		pPicture->Release();											// Decrements IPicture Reference Count
		return FALSE;													// Return False (Failure)
	}

	SelectObject(hdcTemp, hbmpTemp);									// Select Handle To Our Temp DC And Our Temp Bitmap Object

	// Render The IPicture On To The Bitmap
	pPicture->Render(hdcTemp,
					 0,
					 0,
					 lWidthPixels,
					 lHeightPixels,
					 0,
					 lHeight,
					 lWidth,
					 -lHeight,
					 0);

	// Convert From BGR To RGB Format And Add An Alpha Value Of 255
	if (m_fAlphaConversion == TRUE)
	{
		// it is important to remember that the colour order is different as we are reading in a DWORD
		// so the intel processor will read it in as little endian so the order is reversed (ABGR)
		DWORD AlphaMatch = (((m_RedAlphaMatch) | (m_GreenAlphaMatch << 8)) | (m_BlueAlphaMatch << 16));

		DWORD *pRGBA = (DWORD*)pBits;

		long NumPixels = lWidthPixels * lHeightPixels;
		while(NumPixels--)												// Loop Through All Of The Pixels
		{
			DWORD	tempRGB;

			tempRGB = *pRGBA;
			// swap red and blue over
			tempRGB = ((tempRGB & 0x00FF0000) >> 16) | (tempRGB & 0x0000FF00) | ((tempRGB & 0x000000FF) << 16);

			if (AlphaMatch != tempRGB)
			{
				tempRGB |= 0xFF000000;
			}

			*pRGBA++ = tempRGB;
		}
	}
	else
	{
		BYTE *pPixel = (BYTE*)pBits;									// Grab The Current Pixel

		long NumPixels = lWidthPixels * lHeightPixels;
		while(NumPixels--)
		{

			BYTE  temp	= pPixel[0];									// Store 1st Color In Temp Variable (Blue)
			pPixel[0]	= pPixel[2];									// Move Red Value To Correct Position (1st)
			pPixel[2]	= temp;											// Move Temp Value To Correct Blue Position (3rd)
			pPixel[3]	= 255;											// Set The Alpha Value To 255
			pPixel		+= 4;
		}
	}

	pglTexture->Width = bi.bmiHeader.biWidth;
	pglTexture->Height = bi.bmiHeader.biHeight;
	pglTexture->Bpp = bi.bmiHeader.biBitCount;							// Image Color Depth In Bits Per Pixel
	pglTexture->Type = GL_RGBA;

	glGenTextures(1, &pglTexture->TextureID);							// Create The Texture

	// generate the texture using the filtering model selected
	(void)GenerateTexture(pglTexture, (BYTE *)pBits);

	DeleteObject(hbmpTemp);												// Delete The Object
	DeleteDC(hdcTemp);													// Delete The Device Context

	pPicture->Release();												// Decrements IPicture Reference Count

	return TRUE;														// Return True (All Good)
}


// Set the Texture parameters to match the type of filtering we want.
//
int TextureResource::GenerateTexture(glTexture *pglTexture, GLubyte *pImgData)
{
	int result = 0;

	int	components;
	// set the bytes per pixel
	if (pglTexture->Type == GL_RGBA)
	{
		components = 4;
	}
	else
	{
		components = 3;
	}

	// Typical Texture Generation Using Data From The Bitmap
	glBindTexture(GL_TEXTURE_2D, pglTexture->TextureID);				// Bind To The Texture ID

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);

	BOOL	Mipping;

	switch(m_TextureFilterType)
	{
		default:
		case txNoFilter:	glPixelStorei(GL_UNPACK_ALIGNMENT, 1 );
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
							glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
							Mipping = FALSE;
							break;

		case txBilinear:	if (m_fMipMapping == FALSE)
							{
								glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
								glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
								// set the build type flag
								Mipping = FALSE;
							}
							else
							{
								glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
								glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
								Mipping = TRUE;
							}
							break;

		case txTrilinear:	glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
							glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
							// always mip mapping for trilinear
							Mipping = TRUE;
							break;
	}


	// crank out the texture
	if (Mipping == FALSE)
	{
		glTexImage2D(GL_TEXTURE_2D,
					 0,
					 components,
					 pglTexture->Width,
					 pglTexture->Height,
					 0,
					 pglTexture->Type,
					 GL_UNSIGNED_BYTE,
					 pImgData);
	}
	else
	{
		// Build Mipmaps (builds different versions of the picture for distances - looks better)
		result = gluBuild2DMipmaps(GL_TEXTURE_2D,
								   components,
								   pglTexture->Width,
								   pglTexture->Height,
								   pglTexture->Type,
								   GL_UNSIGNED_BYTE,
								   pImgData);
	}

	return 0;
}


// extract the extension from the specified filename
//
void TextureResource::ExtensionFromFilename(char *szFileName, char *szExtension)
{
	int len = lstrlen(szFileName);

	int begin;
 	for (begin=len;begin>=0;begin--)
	{
		if (szFileName[begin] == '.')
		{
			begin++;
			break;
		}
	}

	if (begin <= 0)
	{
		szExtension[0] = '\0';
	}
	else
	{
		lstrcpy(szExtension, &szFileName[begin]);
	}
}
