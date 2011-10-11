#ifndef _MODELNODE_H_
#define _MODELNODE_H_

#include <vector>
#include "GL/glew.h"
#include "Vertex4f.h"
#include "SceneNode.h"
#include "FrameIndexBuffer.h"

// Class that holds information related to the actual model of an object
// Contains a list of vertex indices

class ModelNode : public SceneNode {
private:
	// The indices of the vertices that compose the model
	std::vector<GLuint> vertices_index_;

	void replaceNormal( GLuint *index, Vertex4f *vertex, Vertex4f *normal );

public:
	// Constructor/destructor; just some initializations
	ModelNode();
	~ModelNode();

	// Add an index to the model
	void addIndex( GLuint index );
	// Normalize the face described by the last 3 indices
	void normalizeFace();
	// Normalize the face described by the specified vertex indices
	void normalizeFace( GLuint *a, GLuint *b, GLuint *c );

	// Render the model;
	// deltaT is the parameter that controls how much the model should move (if it can move)
	// indices points to the FrameIndexBuffer that will receive the indices to render
	void render( float deltaT, FrameIndexBuffer *indices );
};

#endif