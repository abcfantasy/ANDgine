#include "Camera.h"
#include "SDL_opengl.h"
#define _USE_MATH_DEFINES
#include <math.h>

void Camera::positionCamera( float pos_x, float pos_y, float pos_z, float view_x, float view_y, float view_z ) {
	this->position_[0] = pos_x;
	this->position_[1] = pos_y;
	this->position_[2] = pos_z;

	this->view_[0] = view_x;
	this->view_[1] = view_y;
	this->view_[2] = view_z;
};

void Camera::rotate( float x, float y, float deltaT ) {
	// Rotation around the X-axis
	this->view_[1] += x * deltaT / 1000.0f;
	
	// Limit the rotation so it doesn't go too high or too low
	if(this->view_[1] > 2.0f)	this->view_[1] = 2.0f;
	if(this->view_[1] < 0.4f)	this->view_[1] = 0.4f;
	
	// Rotation around the Y-axis
	y = float(M_PI) * ( -y * deltaT / 1000.0f ) / 180;

	float newViewX = this->position_[0] - this->view_[0];
	float newViewZ = this->position_[2] - this->view_[2];
	
	this->position_[0] = (float)( this->view_[0] + cosf( y ) * newViewX - sinf( y ) * newViewZ );
	this->position_[2] = (float)( this->view_[2] + sinf( y ) * newViewX + cosf( y ) * newViewZ );
};

void Camera::rotate( float rotation[2], float deltaT ) {
	this->rotate( rotation[0], rotation[1], deltaT );
};

void Camera::translate( float x, float y, float z, float deltaT ) {
	this->position_[0] += x * deltaT / 1000.0f;
	this->position_[1] += y * deltaT / 1000.0f;
	this->position_[2] += z * deltaT / 1000.0f;

	this->view_[0] += x * deltaT / 1000.0f;
	this->view_[1] += y * deltaT / 1000.0f;
	this->view_[2] += z * deltaT / 1000.0f;
};

void Camera::changeY( float deltaY ) {
	this->position_[1] += deltaY;
	this->view_[1] += deltaY;
}

void Camera::translate( float position[3], float deltaT ) {
	this->translate( position[0], position[1], position[2], deltaT );
};

void Camera::render() {
	gluLookAt( position_[0], position_[1], position_[2], view_[0], view_[1], view_[2], 0.0f, 1.0f, 0.0f );
};