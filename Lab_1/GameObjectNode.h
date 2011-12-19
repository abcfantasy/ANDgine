#ifndef _GAMEOBJECTNODE_H_
#define _GAMEOBJECTNODE_H_

class GameObject;

#include <vector>
#include "Vertex3f.h"
#include "SceneNode.h"

// Class that holds information related to a particular instance of a GameObject
// Implements the FlyWeight design pattern (this will be a nice thing to mention in a presentation)
class GameObjectNode : public SceneNode {
protected:
	// The contained game object
	GameObject *gameObject_;
	// TODO: Add other related attributes as they are decided

public:
	// Basic constructor/destructor
	GameObjectNode( GameObject *gameObject );
	~GameObjectNode();

	// Getter
	inline GameObject* getGameObject() { return this->gameObject_; };
	inline std::vector<Vertex3f>* getBoundingBox() { return &this->boundingBox_; };

	// Compiles the position/rotation into a display list
	void compile();
	// Render the game object's model
	// deltaT is the parameter that controls how much the object should move (if it can move)
	void render( float deltaT );
	
	// Checking the collision with the bounding box
	// And then with the model itself in case it has some special collision detection
	bool checkCollision( SceneNode *node );
};

#endif