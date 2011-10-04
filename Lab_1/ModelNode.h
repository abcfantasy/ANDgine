#ifndef _MODELNODE_H_
#define _MODELNODE_H_

// TODO: Figure out why it doesn't work if this include isn't here
#include "Vertex4f.h"

// Class that holds information related to the actual model of an object
// Contains a list of vertex indices

class ModelNode : public SceneNode {
private:
	// The indices of the vertices that compose the model
	std::vector<GLuint> vertices_index_;

public:
	// Constructor/destructor; just some initializations
	ModelNode();
	~ModelNode();

	// Add an index to the model
	void addIndex( GLuint index );
	// Render the model;
	// deltaT is the parameter that controls how much the model should move (if it can move)
	// indices points to the FrameIndexBuffer that will receive the indices to render
	void render( float deltaT, FrameIndexBuffer *indices );
};

#endif