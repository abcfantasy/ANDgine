#include "GameObjectNode.h"
#include "GameObject.h"

GameObjectNode::GameObjectNode( GameObject *gameObject ) {
	this->gameObject_ = gameObject;
	this->setPosition( 0.0f, 0.0f, 0.0f );
	this->setRotation( 0.0f, 0.0f, 0.0f );
	this->setVelocity( 0.0f, 0.0f, 0.0f );
	this->setAngleVelocity( 0.0f, 0.0f, 0.0f );
	this->displayListId_ = SceneNode::INVALID_HANDLE;

	// create bounding box
	std::vector<Vertex3f> modelVertices = gameObject->getModel()->getVertices();
	float minX = modelVertices[0].getX();
	float maxX = modelVertices[0].getX();
	float minY = modelVertices[0].getY();
	float maxY = modelVertices[0].getY();
	float minZ = modelVertices[0].getZ();
	float maxZ = modelVertices[0].getZ();
	for ( unsigned int i = 1; i < modelVertices.size(); i++ ) {
		// x
		if ( modelVertices[i].getX() < minX )
			minX = modelVertices[i].getX();
		else if ( modelVertices[i].getX() > maxX )
			maxX = modelVertices[i].getX();
		// y
		if ( modelVertices[i].getY() < minY )
			minY = modelVertices[i].getY();
		else if ( modelVertices[i].getY() > maxY )
			maxY = modelVertices[i].getY();
		// z
		if ( modelVertices[i].getZ() < minZ )
			minZ = modelVertices[i].getZ();
		else if ( modelVertices[i].getZ() > maxZ )
			maxZ = modelVertices[i].getZ();
	}
	this->boundingBox_.push_back( Vertex3f( minX, minY, minZ ) );
	this->boundingBox_.push_back( Vertex3f( maxX, minY, minZ ) );
	this->boundingBox_.push_back( Vertex3f( maxX, maxY, minZ ) );
	this->boundingBox_.push_back( Vertex3f( minX, maxY, minZ ) );

	this->boundingBox_.push_back( Vertex3f( maxX, minY, maxZ ) );
	this->boundingBox_.push_back( Vertex3f( maxX, maxY, maxZ ) );
	this->boundingBox_.push_back( Vertex3f( minX, maxY, maxZ ) );
	this->boundingBox_.push_back( Vertex3f( minX, minY, maxZ ) );
};

void GameObjectNode::translate( float x, float y, float z, float deltaT ) {
	// update bounding box
	for ( unsigned int i = 0; i < this->boundingBox_.size(); i++ ) {
		boundingBox_[i].setX( boundingBox_[i].getX() + ( x * deltaT / 1000.0f ) );
		boundingBox_[i].setY( boundingBox_[i].getY() + ( y * deltaT / 1000.0f ) );
		boundingBox_[i].setZ( boundingBox_[i].getZ() + ( z * deltaT / 1000.0f ) );
	}
	SceneNode::translate( x, y, z, deltaT );
};

void GameObjectNode::translate( float position[3], float deltaT ) {
	GameObjectNode::translate( position[0], position[1], position[2], deltaT );
};

GameObjectNode::~GameObjectNode() {
	delete gameObject_;
}

void GameObjectNode::render( float deltaT ) {
	this->applyVelocity( deltaT );
	if( this->displayListId_ == SceneNode::INVALID_HANDLE ) this->compile();
	glPushMatrix();
	glCallList( this->getDisplayListId() );
	
	this->gameObject_->render();
	
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