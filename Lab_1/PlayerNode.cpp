#include "PlayerNode.h"
#include "GameObject.h"
#include "InputManager.h"
#include "ResourceManager.h"

PlayerNode::PlayerNode( GameObject *gameObject ) : GameObjectNode( gameObject ) {
	this->camera_.positionCamera(
		this->position_[0],	this->position_[1] + 2.0f,	this->position_[2] + 4.0f,
		this->position_[0],	this->position_[1],			this->position_[2]
	);

	this->skyBox_ = new GameObjectNode( new GameObject( ResourceManager::instance()->get<Model>( "Models\\skybox.obj" ) ) );
};

PlayerNode::~PlayerNode() {
	delete skyBox_;
};

void PlayerNode::render( float deltaT ) {
	float angle_x, angle_y;
	bool *keys;

	InputManager::instance()->getMouseAngle( &angle_x, &angle_y );
	keys = InputManager::instance()->getKeys();
	if( keys[ SDLK_SPACE ] ) {
		this->camera_.rotate( angle_x, angle_y );
	} else {
		this->rotate( angle_x, angle_y, 0.0f );
	}
	this->applyVelocity( deltaT );
	this->camera_.render();
	if( this->displayListId_ == SceneNode::INVALID_HANDLE ) this->compile();
	glPushMatrix();
	glCallList( this->getDisplayListId() );

	this->gameObject_->render();

	glPopMatrix();
	this->skyBox_->render( deltaT );
};


void PlayerNode::rotate( float x, float y, float z, float deltaT ) {
	this->camera_.rotate( x, y, deltaT );
	SceneNode::rotate( 0.0f, y, z, deltaT );
};

void PlayerNode::rotate( float rotation[3], float deltaT ) {
	PlayerNode::rotate( rotation[0], rotation[1], rotation[2], deltaT );
};

void PlayerNode::translate( float x, float y, float z, float deltaT ) {
	// update bounding box
	for ( unsigned int i = 0; i < this->boundingBox_.size(); i++ ) {
		boundingBox_[i].setX( boundingBox_[i].getX() + ( x * deltaT / 1000.0f ) );
		boundingBox_[i].setY( boundingBox_[i].getY() + ( y * deltaT / 1000.0f ) );
		boundingBox_[i].setZ( boundingBox_[i].getZ() + ( z * deltaT / 1000.0f ) );
	}
	this->camera_.translate( x, y, z, deltaT );
	this->skyBox_->translate( x, y, z, deltaT );
	SceneNode::translate( x, y, z, deltaT );
};

void PlayerNode::translate( float position[3], float deltaT ) {
	PlayerNode::translate( position[0], position[1], position[2], deltaT );
};