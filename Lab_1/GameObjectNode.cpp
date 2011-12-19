#include "GameObjectNode.h"
#include "GameObject.h"
#include "Collision.h"

GameObjectNode::GameObjectNode( GameObject *gameObject ) {
	this->gameObject_ = gameObject;
	this->setPosition( 0.0f, 0.0f, 0.0f );
	this->setRotation( 0.0f, 0.0f, 0.0f );
	this->setVelocity( 0.0f, 0.0f, 0.0f );
	this->setAcceleration( 0.0f, 0.0f, 0.0f );
	this->setAngleVelocity( 0.0f, 0.0f, 0.0f );
	this->displayListId_ = SceneNode::INVALID_HANDLE;

	// create bounding box
	if( gameObject->getModel() != NULL ) {
		std::vector<Vertex3f> *modelVertices = gameObject->getModel()->getVertices();
		float minX = (*modelVertices)[0].getX();
		float maxX = (*modelVertices)[0].getX();
		float minY = (*modelVertices)[0].getY();
		float maxY = (*modelVertices)[0].getY();
		float minZ = (*modelVertices)[0].getZ();
		float maxZ = (*modelVertices)[0].getZ();
		for ( std::vector<Vertex3f>::iterator i = (modelVertices->begin() + 1); i != modelVertices->end(); ++i ) {
			// x
			if ( i->getX() < minX )			minX = i->getX();
			else if ( i->getX() > maxX )	maxX = i->getX();
			// y
			if ( i->getY() < minY )			minY = i->getY();
			else if ( i->getY() > maxY )	maxY = i->getY();
			// z
			if ( i->getZ() < minZ )			minZ = i->getZ();
			else if ( i->getZ() > maxZ )	maxZ = i->getZ();
		}
		this->boundingBox_.push_back( Vertex3f( minX, minY, minZ ) );
		this->boundingBox_.push_back( Vertex3f( maxX, minY, minZ ) );
		this->boundingBox_.push_back( Vertex3f( maxX, maxY, minZ ) );
		this->boundingBox_.push_back( Vertex3f( minX, maxY, minZ ) );

		this->boundingBox_.push_back( Vertex3f( maxX, minY, maxZ ) );
		this->boundingBox_.push_back( Vertex3f( maxX, maxY, maxZ ) );
		this->boundingBox_.push_back( Vertex3f( minX, maxY, maxZ ) );
		this->boundingBox_.push_back( Vertex3f( minX, minY, maxZ ) );
	}
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

// Checking the collision with the bounding box
// And then with the model itself in case it has some special collision detection
bool GameObjectNode::checkCollision( SceneNode *node ) {
	if( this->boundingBox_.size() == 8 ) {
		if( Collision::GJKCollide( &(this->boundingBox_), node->getBoundingBox() ) ) {
			if( this->gameObject_->getModel() != NULL ) {
				float pos[4];
				this->worldToModel( (*node->getBoundingBox())[0].getPosition(), pos );
				return this->gameObject_->getModel()->checkSpecialCollision( pos );
			}
		}
	}
	return false;
};