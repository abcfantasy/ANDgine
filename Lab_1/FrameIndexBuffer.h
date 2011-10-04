#ifndef _FRAMEINDEXBUFFER_H_
#define _FRAMEINDEXBUFFER_H_

// TODO: Figure out why it doesn't work if this include isn't here
#include "GL/glew.h"

// Class that collects the indices of the vertices that will be rendered in each frame
// It's basically a Stack Allocator for indices
// It should persist for the lifetime of the application and its memory is never deallocated - it's just overwritten

class FrameIndexBuffer {
private:
	// The stack of indices
	GLuint *indices_;
	// Top of the stack
	GLuint top_;
	// Maximum size of the stack; Default: 2048
	GLuint size_;
	
public:
	// Constructor that creates a stack of a given size; Default: 2048
	FrameIndexBuffer( int size = 2048 );
	// Destructor; deallocates the memory
	~FrameIndexBuffer();

	// Get the number of indices in the stack
	inline GLuint getCount() { return this->top_; };
	// Get a pointer to the stack
	inline GLuint* getIndices() { return this->indices_; };

	// Add an index to the stack
	void addIndex( GLuint index );
	// Reset the stack to zero
	void reset();
};

#endif