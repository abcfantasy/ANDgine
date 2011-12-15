#ifndef _SCENEMANAGER_H_
#define _SCENEMANAGER_H_

class PlayerNode;

#include "ContainerNode.h"
#include "SDL.h"

// Class that contains information related to the scenegraph and to rendering
class SceneManager {
private:
	// The scenegraph (its root is just a ContainerNode)
	ContainerNode sceneGraph_;
	// Pointer to the player node in the sceneGraph for easy callback manipulation
	// The memory will still be managed by the scenegraph
	PlayerNode *playerNode_;
	// Timestamp of when the last frame was rendered
	int timeLastFrame_;

	// Basic constructor and destructor
	SceneManager() : timeLastFrame_( 0 ) {};
	~SceneManager() {};

public:
	// It's a singleton object
	static SceneManager* instance();

	// Getters
	inline ContainerNode* getSceneGraph() { return &this->sceneGraph_; };
	inline PlayerNode* getPlayerNode() { return this->playerNode_; };

	// This method is temporarily here; it has all the test code that we wrote to render a basic scene
	void initializeScene();
	// Renders a frame (it tracks deltaT by itself)
	void renderScene();

	static void keyDown( SDLKey key, SDLMod mod );
	static void keyUp( SDLKey key, SDLMod mod );
};

#endif