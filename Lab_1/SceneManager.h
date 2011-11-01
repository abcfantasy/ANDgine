#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

#include "ContainerNode.h"

// Class that contains information related to the scenegraph and to rendering
class SceneManager {
private:
	// The scenegraph (its root is just a ContainerNode)
	ContainerNode sceneGraph_;
	// Timestamp of when the last frame was rendered
	int timeLastFrame_;

	// Basic constructor and destructor
	SceneManager() : timeLastFrame_( 0 ) {};
	~SceneManager() {};

public:
	// It's a singleton object
	static SceneManager* instance();

	// Getter for the scene graph
	inline ContainerNode* getSceneGraph() { return &this->sceneGraph_; };

	// This method is temporarily here; it has all the test code that we wrote to render a basic scene
	void initializeScene();
	// Renders a frame (it tracks deltaT by itself)
	void renderScene();
};

#endif