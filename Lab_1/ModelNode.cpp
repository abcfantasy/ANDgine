#include "ModelNode.h"
#include <set>
#include "Matrix.h"
#include "LightingManager.h"
#include "VertexManager.h"

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

void ModelNode::replaceNormal( GLuint *index, Vertex4f *vertex, Vertex4f *normal ) {
	if( vertex->hasNormal() ) {
		Vertex4f tempVertex = *vertex;
		tempVertex.setNormal( normal->getX(), normal->getY(), normal->getZ() );
		*index = VertexManager::instance()->addVertex( tempVertex );
	} else {
		vertex->setNormal( normal->getX(), normal->getY(), normal->getZ() );
	};
};


void ModelNode::normalizeFace() {
	int vertexCount = this->vertices_index_.size();
	this->normalizeFace( &this->vertices_index_[vertexCount-3], &this->vertices_index_[vertexCount-2], &this->vertices_index_[vertexCount-1] );
};

void ModelNode::normalizeFace( GLuint *a, GLuint *b, GLuint *c ) {
	Vertex4f normal;
	Vertex4f *v1 = VertexManager::instance()->getVertex( *a );
	Vertex4f *v2 = VertexManager::instance()->getVertex( *b );
	Vertex4f *v3 = VertexManager::instance()->getVertex( *c );
	
	LightingManager::calculateNormal( v1, v2, v3, &normal );

	this->replaceNormal( a, v1, &normal );
	this->replaceNormal( b, v2, &normal );
	this->replaceNormal( c, v3, &normal );
};