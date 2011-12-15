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
	// The bounding box for collision
	std::vector<Vertex3f> boundingBox_;
	// TODO: Add other related attributes as they are decided

public:
	// Basic constructor/destructor
	GameObjectNode( GameObject *gameObject );
	~GameObjectNode();

	// Getter
	inline GameObject* getGameObject() { return this->gameObject_; };
	inline std::vector<Vertex3f>* getBoundingBox() { return &this->boundingBox_; };

	// Overriding the SceneNode rotate and translate methods
	// So they translate both the object and the bounding box at the same time
	void translate( float x, float y, float z, float deltaT = 1000.0f );
	void translate( float position[3], float deltaT = 1000.0f );

	// Compiles the position/rotation into a display list
	void compile();
	// Render the game object's model
	// deltaT is the parameter that controls how much the object should move (if it can move)
	void render( float deltaT );
};

#endif