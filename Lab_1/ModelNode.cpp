#include <vector>
#include <set>
#include "SDL.h"
#include "GL/glew.h"
#include "Matrix.h"
#include "SceneNode.h"
#include "Vertex4f.h"
#include "VertexManager.h"
#include "FrameIndexBuffer.h"
#include "ModelNode.h"

ModelNode::ModelNode() {
	Matrix::buildIdentity( this->getMatrix() );
	this->setVelocity( 0, 0, 0 );
	this->setAngleVelocity( 0, 0, 0 );
};

ModelNode::~ModelNode() {
};

void ModelNode::addIndex( GLuint index ) {
	// We only add the index if it's meaningful
	if( index != VertexManager::INVALID_INDEX )
		this->vertices_index_.push_back( index );
};

void ModelNode::render( float deltaT, FrameIndexBuffer *indices ) {
	// First we adjust the transformation matrix to account for the object's movement
	this->animate( deltaT );
	
	// Here we actually transform the vertices based on the matrix
	// This only happens if we have something to adjust; if the matrix is the identity matrix that means there's no adjustment
	if( !Matrix::isIdentity( this->getMatrix() ) ) {
		// We create a set to keep track of the unique vertices
		std::set<GLuint> current;
		for( std::vector<GLuint>::iterator i = this->vertices_index_.begin(); i != this->vertices_index_.end(); ++i )
			current.insert( *i );

		// We walk through each vertex and transform it
		for( std::set<GLuint>::iterator i = current.begin(); i != current.end(); ++i )
			VertexManager::instance()->transformVertex( (*i), this->getMatrix() );
		// After we're done we reset the matrix to the identity matrix
		Matrix::buildIdentity( this->getMatrix() );
	}

	// And we add the indices to the list of vertices that need rendering
	for( std::vector<GLuint>::iterator i = this->vertices_index_.begin(); i != this->vertices_index_.end(); ++i )
		indices->addIndex( *i );
}