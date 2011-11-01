#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "SDL_opengl.h"

// Base class for the objects in the scene graph
// Derived classes are ContainerNode, which contains other objects, and GameObjectNode, which contains information about GameObject instances

class SceneNode {
protected:
	// The display list ID as it's registered with openGL
	int displayListId_;
	
	// Position of the object in World Space
	float position_[3];
	// Rotation of the object in World Space
	float rotation_[3];
	
	// Speed at which the object is translated; expressed as units/second
	float velocity_[3];
	// Speed at which the object rotates; expressed as degrees/second
	float angle_velocity_[3];

public:
	// Constant used to check if a value is invalid
	static const GLuint INVALID_HANDLE = 0xFFFFFFFF;

	// Many getters and setters
	inline float* getRotation() { return this->rotation_; };
	inline float* getPosition() { return this->position_; };
	inline int getDisplayListId() { return this->displayListId_; };
	inline float* getVelocity() { return this->velocity_; };
	inline float* getAngleVelocity() { return this->angle_velocity_; };

	inline void setDisplayListId( int id ) { this->displayListId_ = id; };
	void setRotation( float x, float y, float z );
	void setRotation( float rotation[3] );
	void setPosition( float x, float y, float z );
	void setPosition( float position[3] );
	void setVelocity( float x, float y, float z );
	void setVelocity( float velocity[3] );
	void setAngleVelocity( float x, float y, float z );
	void setAngleVelocity( float angle_velocity[3] );

	// Rotates the object
	// The rotation is given as as an array of 3 components representing the rotation on each axis
	// deltaT is used as a multiplier; Default = 1000.0f
	void rotate( float rotation[3], float deltaT = 1000.0f );
	
	// Translates the object
	// The translation is given as as an array of 3 components representing the rotation on each axis
	// deltaT is used as a multiplier; Default = 1000.0f
	void translate( float position[3], float deltaT = 1000.0f );

	// Virtual methods used for compiling the graphics data into a display list
	virtual void compile() = 0;
	// And for rendering
	virtual void render( float deltaT ) = 0;
};

#endif