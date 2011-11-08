#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include "SDL_opengl.h"
#include "Vertex3f.h"

class Model {
protected:
	// The vertices that make up the model
	std::vector<Vertex3f> vertices_;
	// The display list ID as it's registered with openGL
	GLuint displayListId_;
	// The rendering method; should be an OpenGL constant; the default is GL_TRIANGLES
	GLuint renderMethod_;
	// The texture handle from the Resource Manager
	int textureHandle_;

public:
	static const GLuint INVALID_HANDLE = 0xFFFFFFFF;
	
	// Basic constructor; no texture Handle
	Model() : textureHandle_( Model::INVALID_HANDLE ), displayListId_( Model::INVALID_HANDLE ), renderMethod_( GL_TRIANGLES ) {};
	// Constructs a model and gives it a texture Handle as well
	Model( int textureHandle ) : textureHandle_( textureHandle ), displayListId_( Model::INVALID_HANDLE ), renderMethod_( GL_TRIANGLES ) {};
	// Basic destructor; releases a texture if it has any
	~Model();

	// Getters and setters
	inline GLuint getRenderMethod() { return this->renderMethod_; };
	inline void setRenderMethod( GLuint renderMethod ) { this->renderMethod_ = renderMethod; };

	// Getter and setter for the texture handle
	inline GLuint getTextureHandle() { return this->textureHandle_; };
	inline void setTextureHandle( int textureHandle ) { this->textureHandle_ = textureHandle; };
	// Loads a texture through the Resource Manager
	void setTexture( char *fileName );
	
	// Operator to access a vertex based on index
	Vertex3f* operator[]( int i ) { return &this->vertices_[i]; };

	// Adds a vertex to the model
	void addVertex( Vertex3f vertex );
	// Compiles the model into a display list
	// This can be overriden by the subclasses
	virtual void compile();
	// Calls the display list to render the model
	void render();
};

#endif