#include <vector>
#include "SDL.h"
#include "SDL_opengl.h"
#include "SceneNode.h"
#include "ModelNode.h"
#include "ContainerNode.h"

ContainerNode::ContainerNode() {
	this->setRotation( 0, 0, 0 );
	this->setTranslation( 0, 0, 0 );
	this->setVelocity( 0, 0, 0 );
	this->setAngleVelocity( 0, 0, 0 );
	this->setDisplayListId( -1 );
};

ContainerNode::~ContainerNode() {
	for( std::vector< SceneNode* >::iterator i = this->objects_.begin(); i != this->objects_.end(); ++i ) {
		delete (*i);
	}
};

void ContainerNode::AddObject( SceneNode *object ) {
	this->objects_.push_back( object );
	if( object->getDisplayListId() == -1 )
		object->compile();
};


void ContainerNode::compile() {
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


	glEndList();

	for( std::vector< SceneNode* >::iterator i = this->objects_.begin(); i != this->objects_.end(); ++i ) {
		if( (*i)->getDisplayListId() == -1 )
			(*i)->compile();
	}
};

void ContainerNode::render( float deltaT ) {
	this->translate( this->getVelocity(), deltaT );
	this->rotate( this->getAngleVelocity(), deltaT );

	if( this->getDisplayListId() == -1 ) {
		this->compile();
	}

	glPushMatrix();
	
	glCallList( this->getDisplayListId() );

	for( std::vector< SceneNode* >::iterator i = this->objects_.begin(); i != this->objects_.end(); ++i ) {
		(*i)->render( deltaT );
	}

	glPopMatrix();
};