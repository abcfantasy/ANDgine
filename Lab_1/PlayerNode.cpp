#include "PlayerNode.h"
#include "InputManager.h"

PlayerNode::PlayerNode( GameObject gameObject ) : GameObjectNode( gameObject ) {
	this->camera_.positionCamera(
		this->position_[0],	this->position_[1] + 0.5f,	this->position_[2] + 3.0f,
		this->position_[0],	this->position_[1],			this->position_[2]
	);
};

void PlayerNode::render( float deltaT ) {
	float angle_x, angle_y;

	InputManager::instance()->getMouseAngle( &angle_x, &angle_y );
	this->rotate( angle_x, angle_y, 0.0f );

	this->translate( this->getVelocity(), deltaT );
	this->rotate( this->getAngleVelocity(), deltaT );
	this->camera_.render();

	if( this->displayListId_ == SceneNode::INVALID_HANDLE ) this->compile();

	glPushMatrix();
	glCallList( this->getDisplayListId() );

	Model* gameObjectModel = this->gameObject_.getModel();
	if( gameObjectModel != NULL ) gameObjectModel->render();

	glPopMatrix();
};


void PlayerNode::rotate( float x, float y, float z, float deltaT ) {
	this->camera_.rotate( x, y, deltaT );
	SceneNode::rotate( x, y, z, deltaT );
};

void PlayerNode::rotate( float rotation[3], float deltaT ) {
	PlayerNode::rotate( rotation[0], rotation[1], rotation[2], deltaT );
};

void PlayerNode::translate( float x, float y, float z, float deltaT ) {
	// update bounding box
	for ( int i = 0; i < this->boundingBox_.size(); i++ ) {
		boundingBox_[i].setX( boundingBox_[i].getX() + ( x * deltaT / 1000.0f ) );
		boundingBox_[i].setY( boundingBox_[i].getY() + ( y * deltaT / 1000.0f ) );
		boundingBox_[i].setZ( boundingBox_[i].getZ() + ( z * deltaT / 1000.0f ) );
	}
	this->camera_.translate( x, y, z, deltaT );
	SceneNode::translate( x, y, z, deltaT );
};

void PlayerNode::setY( float newY ) {
	float deltaY = newY - this->position_[1];
	this->position_[1] = newY;
	this->camera_.changeY( deltaY );
}


void PlayerNode::translate( float position[3], float deltaT ) {
	PlayerNode::translate( position[0], position[1], position[2], deltaT );
};
