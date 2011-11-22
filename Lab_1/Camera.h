#ifndef _CAMERA_H_
#define _CAMERA_H_

class Camera {
protected:
	// Position and view vectors for the Camera
	float position_[3];
	float view_[3];

public:
	// Basic constructor/destructor
	Camera() {};
	~Camera() {};

	// Initialize the vectors
	void positionCamera( float pos_x, float pos_y, float pos_z, float view_x, float view_y, float view_z );

	// Rotating the camera; there is no rotation around the Z axis
	void rotate( float x, float y, float deltaT = 1000.0f );
	void rotate( float rotation[2], float deltaT = 1000.0f );

	// Translating the camera; 
	void translate( float x, float y, float z, float deltaT = 1000.0f );
	void translate( float position[3], float deltaT = 1000.0f );

	void Camera::changeY( float deltaY );

	// Calls gluLookAt with the proper parameters
	void render();
};

#endif