#include <vector>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SceneNode.h"
#include "Vertex3f.h"
#include "ModelNode.h"

ModelNode::ModelNode() {
	this->setRotation( 0, 0, 0 );
	this->setTranslation( 0, 0, 0 );
	this->setVelocity( 0, 0, 0 );
	this->setAngleVelocity( 0, 0, 0 );
	this->setDisplayListId( -1 );
};

ModelNode::~ModelNode() {
};

void ModelNode::AddVertex( Vertex3f vertex ) {
	this->vertices_.push_back( vertex );
};

void ModelNode::compile() {
	if( this->getDisplayListId() != -1 ) {
		glDeleteLists( this->getDisplayListId(), 1 );
	}
	this->setDisplayListId( glGenLists( 1 ) );
	glNewList( this->getDisplayListId(), GL_COMPILE );
	
	float *translation = this->getTranslation();
	glTranslatef( translation[0], translation[1], translation[2] );

	float *rotation = this->getRotation();
	glRotatef( rotation[0], 1.0, 0.0, 0.0 );
	glRotatef( rotation[1], 0.0, 1.0, 0.0 );
	glRotatef( rotation[2], 0.0, 0.0, 1.0 );
	
	glBegin( GL_TRIANGLES );
	for( std::vector<Vertex3f>::iterator i = this->vertices_.begin(); i != this->vertices_.end(); ++i ) {
		glColor3f( i->getR(), i->getG(), i->getB() );
		glVertex3f( i->getX(), i->getY(), i->getZ() );
	}
	glEnd();

	glEndList();
};

void ModelNode::render( float deltaT ) {
	this->translate( this->getVelocity(), deltaT );
	this->rotate( this->getAngleVelocity(), deltaT );

	if( this->getDisplayListId() == -1 ) {
		this->compile();
	}

	glPushMatrix();
	glCallList( this->getDisplayListId() );
	glPopMatrix();
}