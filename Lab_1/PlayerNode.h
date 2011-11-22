#ifndef _PLAYERNODE_H_
#define _PLAYERNODE_H_

#include "Camera.h"
#include "GameObjectNode.h"
#include "GameObject.h"

// The PlayerNode contains the player's object (as a GameObject) and camera information (as a CameraNode)
class PlayerNode : public GameObjectNode {
private:
	// The camera
	Camera camera_;

public:
	// Constructor is the same as a GameObjectNode
	PlayerNode( GameObject gameObject );
	// Basic destructor
	~PlayerNode() {};

	// Getter for the camera
	inline Camera* getCamera() { return &this->camera_; };

	// Overriding the SceneNode rotate and translate methods
	// So they rotate both the object and the camera at the same time
	void rotate( float x, float y, float z, float deltaT = 1000.0f );
	void rotate( float rotation[3], float deltaT = 1000.0f );

	void translate( float x, float y, float z, float deltaT = 1000.0f );
	void translate( float position[3], float deltaT = 1000.0f );

	void PlayerNode::setY( float newY );

	// Overriding the render function to properly adjust for mouse movement and render the camera as well
	void render( float deltaT );
};

#endif