#include "Model.h"

// Adds a vertex to the model
void Model::addVertex( Vertex3f vertex ) {
	this->vertices_.push_back( vertex );
};

// Compiles the model into a display list
void Model::compile() {
	// We need to free up the old display list if we were using one
	if( this->displayListId_ != Model::INVALID_HANDLE ) {
		glDeleteLists( this->displayListId_, 1 );
	}
	this->displayListId_ = glGenLists( 1 );
	glNewList( this->displayListId_, GL_COMPILE );
	
	// TODO: figure out how to work materials into this

	//float specReflection[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	//glMateriali(GL_FRONT, GL_SHININESS, 129 );

	// If we have a texture, we're using it, but if we don't we're not using it
	// The code here may be duplicated but it works faster than it would be if we're making an check for every vertex
	if ( this->textureId_ != Model::INVALID_HANDLE ) {
		glBindTexture( GL_TEXTURE_2D, this->textureId_ );
		glBegin( this->renderMethod_ );
		for( std::vector<Vertex3f>::iterator i = this->vertices_.begin(); i != this->vertices_.end(); ++i ) {
			glNormal3f( i->getNormalX(), i->getNormalY(), i->getNormalZ() );
			glTexCoord2f( i->getU(), i->getV() );
			glColor3f( i->getR(), i->getG(), i->getB() );
			glVertex3f( i->getX(), i->getY(), i->getZ() );
		}
		glEnd();	
	} else {
		glBegin( this->renderMethod_ );
		for( std::vector<Vertex3f>::iterator i = this->vertices_.begin(); i != this->vertices_.end(); ++i ) {
			glNormal3f( i->getNormalX(), i->getNormalY(), i->getNormalZ() );
			glColor3f( i->getR(), i->getG(), i->getB() );
			glVertex3f( i->getX(), i->getY(), i->getZ() );
		}
		glEnd();
	}

	glEndList();
};

// Calls the display list to render the model
void Model::render() {
	// If for some reason it didn't compile, we do it now
	if( this->displayListId_ == Model::INVALID_HANDLE ) this->compile();

	glCallList( this->displayListId_ );
};