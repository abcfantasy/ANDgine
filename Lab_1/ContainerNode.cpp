#include "ContainerNode.h"

#include "SDL.h"
#include "SDL_opengl.h"

ContainerNode::ContainerNode() {
	this->setRotation( 0, 0, 0 );
	this->setPosition( 0, 0, 0 );
	this->setVelocity( 0, 0, 0 );
	this->setAngleVelocity( 0, 0, 0 );
	this->displayListId_ = SceneNode::INVALID_HANDLE;
};

ContainerNode::~ContainerNode() {
	for( std::vector< SceneNode* >::iterator i = this->objects_.begin(); i != this->objects_.end(); ++i ) {
		delete (*i);
	}
};

void ContainerNode::addObject( SceneNode *object ) {
	this->objects_.push_back( object );
	if( object->getDisplayListId() == SceneNode::INVALID_HANDLE )
		object->compile();
};

void ContainerNode::compile() {
	if( this->displayListId_ != SceneNode::INVALID_HANDLE ) {
		glDeleteLists( this->displayListId_, 1 );
	}
	this->displayListId_ = glGenLists( 1 );
	glNewList( this->getDisplayListId(), GL_COMPILE );
	
	glTranslatef( this->position_[0], this->position_[1], this->position_[2] );
	glRotatef( this->rotation_[0], 1.0, 0.0, 0.0 );
	glRotatef( this->rotation_[1], 0.0, 1.0, 0.0 );
	glRotatef( this->rotation_[2], 0.0, 0.0, 1.0 );

	glEndList();
};

void ContainerNode::render( float deltaT ) {
	this->applyVelocity( deltaT );
	if( this->displayListId_ == SceneNode::INVALID_HANDLE )	this->compile();
	glPushMatrix();
	glCallList( this->displayListId_ );

	for( std::vector< SceneNode* >::iterator i = this->objects_.begin(); i != this->objects_.end(); ++i ) {
		(*i)->render( deltaT );
	}

	glPopMatrix();
};