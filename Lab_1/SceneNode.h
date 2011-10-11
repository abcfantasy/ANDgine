#ifndef _SCENENODE_H_
#define _SCENENODE_H_

// Base class for the objects in the scene graph
// Derived classes are ContainerNode, which contains other objects, and ModelNode, which contains model information

class FrameIndexBuffer;

class SceneNode {
private:
	// Speed at which the object is translated; expressed as units/second
	float velocity_[3];
	// Speed at which the object rotates; expressed as degrees/second
	float angle_velocity_[3];
	// Transformation matrix that contains positional data
	float matrix_[16];

public:
	// Getters and setters for the attributes
	inline float* getVelocity() { return this->velocity_; };
	inline float* getAngleVelocity() { return this->angle_velocity_; };
	inline float* getMatrix() { return this->matrix_; };

	void setVelocity( float x, float y, float z );
	void setVelocity( float velocity[3] );
	void setAngleVelocity( float x, float y, float z );
	void setAngleVelocity( float angle_velocity[3] );

	// Rotates the object
	// Either we give the rotation as an array of 3 components
	// or as 3 parameters representing the rotation on each axis
	// deltaT is used as a multiplier; Default = 1000.0f
	void rotate( float rotation[3], float deltaT = 1000.0f );
	void rotate( float x, float y, float z, float deltaT = 1000.0f );
	
	// Translates the object
	// Either we give the translation as an array of 3 components
	// or as 3 parameters representing the translation on each axis
	// deltaT is used as a multiplier; Default = 1000.0f
	void translate( float translation[3], float deltaT = 1000.0f );
	void translate( float x, float y, float z, float deltaT = 1000.0f );

	// Applies the velocity and angle_velocity for the object
	// deltaT is used as a multiplier; Default = 1000.0f
	void animate( float deltaT = 1000.0f );

	// Virtual method used for rendering; implemented by the subclasses
	virtual void render( float deltaT, FrameIndexBuffer *indices ) = 0;
};

#endif