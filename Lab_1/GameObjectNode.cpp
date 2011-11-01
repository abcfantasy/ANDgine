#include "GameObjectNode.h"

GameObjectNode::GameObjectNode( GameObject gameObject ) {
	this->gameObject_ = gameObject;
	this->setPosition( 0.0f, 0.0f, 0.0f );
	this->setRotation( 0.0f, 0.0f, 0.0f );
	this->setVelocity( 0.0f, 0.0f, 0.0f );
	this->setAngleVelocity( 0.0f, 0.0f, 0.0f );
	this->displayListId_ = SceneNode::INVALID_HANDLE;
};

void GameObjectNode::render( float deltaT ) {
	this->translate( this->getVelocity(), deltaT );
	this->rotate( this->getAngleVelocity(), deltaT );

	if( this->displayListId_ == SceneNode::INVALID_HANDLE ) this->compile();

	glPushMatrix();
	glCallList( this->getDisplayListId() );

	Model* gameObjectModel = this->gameObject_.getModel();
	if( gameObjectModel != NULL ) gameObjectModel->render();

	glPopMatrix();
};

void GameObjectNode::compile() {
	if( this->getDisplayListId() != SceneNode::INVALID_HANDLE ) {
		glDeleteLists( this->getDisplayListId(), 1 );
	}
	this->displayListId_ = glGenLists( 1 );
	glNewList( this->getDisplayListId(), GL_COMPILE );
	
	glTranslatef( this->position_[0], this->position_[1], this->position_[2] );
	glRotatef( this->rotation_[0], 1.0, 0.0, 0.0 );
	glRotatef( this->rotation_[1], 0.0, 1.0, 0.0 );
	glRotatef( this->rotation_[2], 0.0, 0.0, 1.0 );

	glEndList();
};