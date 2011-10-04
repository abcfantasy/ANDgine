#ifndef _VERTEXMANAGER_H_
#define _VERTEXMANAGER_H_

// Singleton class that contains all the vertices that will be used
// It also contains the Vertex Array code
// It's basically a Stack Allocator for vertices
// TODO: free memory

class VertexManager {
private:
	// The stack of vertices
	Vertex4f vertices_[2048];
	// Top of the stack
	GLuint top_;
	// Ids for the vertexbuffer and colorbuffer
	GLuint vertexBuffer_;
	GLuint colorBuffer_;

	// Constructor/destructor - the object is a singleton so they can't be explicitly called
	VertexManager();
	~VertexManager();

public:
	// Get the instance of the Singleton
	static VertexManager* instance();
	// Constant that indicates that a vertex could not be added (e.g. if the stack is full)
	static const GLuint INVALID_INDEX = 0xFFFFFFFF;

	// Getters for the Ids
	inline GLuint getVertexBuffer() { return this->vertexBuffer_; };
	inline GLuint getColorBuffer() { return this->colorBuffer_; };

	// Adds a vertex to the stack and returns its index
	GLuint addVertex( Vertex4f vertex );
	// Applies a transformation matrix to a vertex
	// The vertex is given as an index
	void transformVertex( const GLuint index, const float matrix[16]  );
	// Renders an array of indexes using indexed Vertex Arrays
	void render( FrameIndexBuffer *indexes );
};

#endif