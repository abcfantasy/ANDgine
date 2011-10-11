#include "VertexManager.h"
#include "Matrix.h"

VertexManager::VertexManager() {
	this->top_ = 0;
/*	glGenBuffers( 1, &(this->vertexBuffer_) );
	glBufferData( GL_ARRAY_BUFFER, 2048 * sizeof( Vertex4f ), 0, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, this->vertexBuffer_ );
	
	glGenBuffers( 1, &(this->colorBuffer_) );
	glBufferData( GL_ARRAY_BUFFER, 2048 * sizeof( Vertex4f ), 0, GL_DYNAMIC_DRAW );
	glBindBuffer( GL_ARRAY_BUFFER, this->colorBuffer_ );*/
};

VertexManager::~VertexManager() {
};

VertexManager* VertexManager::instance() {
	static VertexManager vm;
	return &vm;
};

GLuint VertexManager::addVertex( Vertex4f vertex ) {
	if( top_ == 2048 )
		return VertexManager::INVALID_INDEX;
	
	this->vertices_[ top_ ] = vertex;
	++top_;
	return top_ - 1;
};

Vertex4f* VertexManager::getVertex( GLuint index ) {
	return &this->vertices_[ index ];
};

void VertexManager::transformVertex( const GLuint index, const float matrix[16] ) {
	// We get the vertex from the specified index
	// And we put its coordinates in an array
	Vertex4f *v = (this->vertices_ + index);
	float coords[4];
	coords[0] = v->getX();
	coords[1] = v->getY();
	coords[2] = v->getZ();
	coords[3] = v->getW();
	
	// We multiply the coordinates array with the matrix
	Matrix::multiplyVector( matrix, coords );

	// And then we set the coordinates back into the class instance
	v->setPosition( coords );

	coords[0] = v->getNX();
	coords[1] = v->getNY();
	coords[2] = v->getNZ();
	coords[3] = 1.0f;

	Matrix::multiplyVector( matrix, coords );

	v->setNormal( coords );
};

void VertexManager::render( FrameIndexBuffer *indices ) {	
	glEnableClientState( GL_COLOR_ARRAY );
	glEnableClientState( GL_VERTEX_ARRAY );
	glEnableClientState( GL_NORMAL_ARRAY );
	
	// Using interleaved arrays
	// The vertices have 4 coordinates; their type is float; the structure is Vertex4f so we use that size as a stride; the pointer is the pointer to the Vertex4f array
	glVertexPointer( 4, GL_FLOAT, sizeof( Vertex4f ), this->vertices_ );

//	glNormalPointer( GL_FLOAT, sizeof( Vertex4f ), this->vertices_ );

	glNormalPointer( GL_FLOAT, sizeof( Vertex4f ), &(((float*)this->vertices_)[4]) );

	// The colors have 3 components; their type is float; the structure is Vertex4f so we use that size as a stride;
	// the pointer is modified to point to the start of the color data: 4 floats after the start of the structure
	glColorPointer( 3, GL_FLOAT, sizeof( Vertex4f ), &(((float*)this->vertices_)[7]) );
	
	// And finally we draw the vertices according to their indices
	glDrawElements( GL_TRIANGLES, indices->getCount(), GL_UNSIGNED_INT, indices->getIndices() );

	glDisableClientState( GL_NORMAL_ARRAY );
	glDisableClientState( GL_VERTEX_ARRAY );
	glDisableClientState( GL_COLOR_ARRAY );
};