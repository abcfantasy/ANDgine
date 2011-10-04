#include "FrameIndexBuffer.h"

FrameIndexBuffer::FrameIndexBuffer( int size ) {
	this->top_ = 0;
	this->size_ = size;
	this->indices_ = new GLuint[ size ];
};

FrameIndexBuffer::~FrameIndexBuffer() {
	delete[] this->indices_;
};

void FrameIndexBuffer::addIndex( GLuint index ) {
	if( this->top_ == this->size_ )
		return;
	
	this->indices_[ top_ ] = index;
	++this->top_;
};

void FrameIndexBuffer::reset() {
	this->top_ = 0;
};