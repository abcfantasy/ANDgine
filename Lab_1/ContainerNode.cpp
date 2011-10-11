#include "ContainerNode.h"
#include "Matrix.h"

ContainerNode::ContainerNode() {
	Matrix::buildIdentity( this->getMatrix() );
	this->setVelocity( 0, 0, 0 );
	this->setAngleVelocity( 0, 0, 0 );
};

ContainerNode::~ContainerNode() {	
	for( std::vector< SceneNode* >::iterator i = this->objects_.begin(); i != this->objects_.end(); ++i ) {
		delete (*i);
	}
};

void ContainerNode::addObject( SceneNode *object ) {
	this->objects_.push_back( object );
};

void ContainerNode::render( float deltaT, FrameIndexBuffer *indices ) {
	// First we adjust the transformation matrix to account for the object's movement
	this->animate( deltaT );

	// Here the transformation matrix propagates to the child objects
	// This only happens if we have something to adjust; if the matrix is the identity matrix that means there's no adjustment
	if( !Matrix::isIdentity( this->getMatrix() ) ) {
		// We walk through each object and transform it
		for( std::vector< SceneNode* >::iterator i = this->objects_.begin(); i != this->objects_.end(); ++i )
			Matrix::multiply( this->getMatrix(), (*i)->getMatrix() );
		// After we're done we reset the matrix to the identity matrix
		Matrix::buildIdentity( this->getMatrix() );
	}

	// We render each of the contained objects
	for( std::vector< SceneNode* >::iterator i = this->objects_.begin(); i != this->objects_.end(); ++i )
		(*i)->render( deltaT, indices );

};