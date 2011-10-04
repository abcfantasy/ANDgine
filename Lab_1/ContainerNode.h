#ifndef _CONTAINERNODE_H_
#define _CONTAINERNODE_H_

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
	SceneNode* getObjectAt( int pos ) { return this->objects_[pos]; };

	// Add an object
	void addObject( SceneNode *object );
	// Render the contained objects recursively
	// deltaT is the parameter that controls how much the model should move (if it can move)
	// indices points to the FrameIndexBuffer that will receive the indices to render
	void render( float deltaT, FrameIndexBuffer *indices );
};

#endif