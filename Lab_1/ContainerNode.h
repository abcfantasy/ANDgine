#ifndef _CONTAINERNODE_H_
#define _CONTAINERNODE_H_

#include <vector>
#include "SceneNode.h"

// Class that contains other SceneNode objects
class ContainerNode : public SceneNode {
private:
	// The list of objects it contains
	std::vector<SceneNode*> objects_;
public:
	// Constructor/destructor; just some initializations
	ContainerNode();
	~ContainerNode();

	// Get a pointer to the array of objects
	inline std::vector<SceneNode*>* getObjects() { return &objects_; };
	// Get an object based on its position in the array
	inline SceneNode* getObjectAt( int pos ) { return this->objects_[pos]; };
	inline SceneNode* operator []( int pos ) { return this->objects_[pos]; };

	// Add an object
	void addObject( SceneNode *object );
	// Compiles the position/rotation into a display list
	void compile();
	// Render the contained objects recursively
	// deltaT is the parameter that controls how much the objects should move (if they can move)
	void render( float deltaT );
};

#endif