#ifndef _GAMEOBJECTNODE_H_
#define _GAMEOBJECTNODE_H_

#include "SceneNode.h"
#include "GameObject.h"

// Class that holds information related to a particular instance of a GameObject
// Implements the FlyWeight design pattern (this will be a nice thing to mention in a presentation)
class GameObjectNode : public SceneNode {
protected:
	// The contained game object
	GameObject gameObject_;
	// TODO: Add other related attributes as they are decided

public:
	// Basic constructor/destructor
	GameObjectNode( GameObject gameObject );
	~GameObjectNode() {};

	// Getter
	inline GameObject* getGameObject() { return &this->gameObject_; };
	
	// Compiles the position/rotation into a display list
	void compile();
	// Render the game object's model
	// deltaT is the parameter that controls how much the object should move (if it can move)
	void render( float deltaT );
};

#endif