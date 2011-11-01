#ifndef _MODEL_H_
#define _MODEL_H_

#include <vector>
#include "SDL_opengl.h"
#include "Vertex3f.h"

class Model {
private:
	// The vertices that make up the model
	std::vector<Vertex3f> vertices_;
	// The texture ID as it's registered with openGL
	GLuint textureId_;
	// The display list ID as it's registered with openGL
	GLuint displayListId_;
	// The rendering method; should be an OpenGL constant; the default is GL_TRIANGLES
	GLuint renderMethod_;

public:
	static const GLuint INVALID_HANDLE = 0xFFFFFFFF;
	
	// Basic constructor; no texture Id
	Model() : textureId_( Model::INVALID_HANDLE ), displayListId_( Model::INVALID_HANDLE ), renderMethod_( GL_TRIANGLES ) {};
	// Constructs a model and gives it a texture Id as well
	Model( GLuint textureId ) : textureId_( textureId ), displayListId_( Model::INVALID_HANDLE ), renderMethod_( GL_TRIANGLES ) {};
	// Basic destructor
	~Model() {};

	// Getters and setters
	inline GLuint getRenderMethod() { return this->renderMethod_; };
	inline void setRenderMethod( GLuint renderMethod ) { this->renderMethod_ = renderMethod; };

	inline GLuint gettextureId() { return this->textureId_; };
	inline void settextureId( GLuint textureId ) { this->textureId_ = textureId; };
	
	// Adds a vertex to the model
	void addVertex( Vertex3f vertex );
	// Compiles the model into a display list
	void compile();
	// Calls the display list to render the model
	void render();
};

#endif