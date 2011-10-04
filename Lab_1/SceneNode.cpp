#define _USE_MATH_DEFINES
#include <math.h>
#include "Matrix.h"
#include "FrameIndexBuffer.h"
#include "SceneNode.h"

void SceneNode::setVelocity( float x, float y, float z ) {
	this->velocity_[0] = x;
	this->velocity_[1] = y;
	this->velocity_[2] = z;
};

void SceneNode::setVelocity( float velocity[3] ) {
	this->setVelocity( velocity[0], velocity[1], velocity[2] );
};

void SceneNode::setAngleVelocity( float x, float y, float z ) {
	this->angle_velocity_[0] = x;
	this->angle_velocity_[1] = y;
	this->angle_velocity_[2] = z;
};

void SceneNode::setAngleVelocity( float angle_velocity[3] ) {
	this->setAngleVelocity( angle_velocity[0], angle_velocity[1], angle_velocity[2] );
};

void SceneNode::rotate( float rotation[3], float deltaT ) {
	this->rotate( rotation[0], rotation[1], rotation[2], deltaT );
};

void SceneNode::rotate( float x, float y, float z, float deltaT ) {
	float rotation_matrix[16];
	
	// Here we are transforming the rotation parameters to radians
	// Taking into account that the parameter is given in degrees
	// And that the deltaT time will influence the rotation
	x = ( x * deltaT / 1000.0f ) * float(M_PI) / 180.0f;
	y = ( y * deltaT / 1000.0f ) * float(M_PI) / 180.0f;
	z = ( z * deltaT / 1000.0f ) * float(M_PI) / 180.0f;
	
	// Are we rotating around the X-axis?
	if( x != 0.0f ) {
		// If we are, then we're building the rotation matrix
		// 1   0    0 0
		// 0 cos -sin 0
		// 0 sin  cos 0
		// 0   0    0 1
		Matrix::buildIdentity( rotation_matrix );
		rotation_matrix[5] = rotation_matrix[10] = cosf( x );
		rotation_matrix[6] = -sinf( x );
		rotation_matrix[9] = -rotation_matrix[6];
		
		// And applying it to the node's matrix
		Matrix::multiply( rotation_matrix, this->matrix_ );
	}

	// Are we rotating around the Y-axis?
	if( y != 0.0f ) {
		// If we are, then we're building the rotation matrix
		// cos 0  sin 0
		//   0 1    0 0
		//-sin 0  cos 0
		//   0 0    0 1
		Matrix::buildIdentity( rotation_matrix );
		rotation_matrix[0] = rotation_matrix[10] = cosf( y );
		rotation_matrix[2] = sinf( y );
		rotation_matrix[8] = -rotation_matrix[2];
		
		// And applying it to the node's matrix
		Matrix::multiply( rotation_matrix, this->matrix_ );
	}

	// Are we rotating around the Z-axis?
	if( z != 0.0f ) {
		// If we are, then we're building the rotation matrix
		// cos -sin 0 0
		// sin  cos 0 0
		//   0    0 1 0
		//   0    0 0 1
		Matrix::buildIdentity( rotation_matrix );
		rotation_matrix[0] = rotation_matrix[5] = cosf( z );
		rotation_matrix[1] = -sinf( z );
		rotation_matrix[4] = -rotation_matrix[1];
		
		// And applying it to the node's matrix
		Matrix::multiply( rotation_matrix, this->matrix_ );
	}
};

void SceneNode::translate( float translation[3], float deltaT ) {
	this->translate( translation[0], translation[1], translation[2], deltaT );
};

void SceneNode::translate( float x, float y, float z, float deltaT ) {
	float translation_matrix[16];

	// Building the translation matrix
	// 1 0 0 x
	// 0 1 0 y
	// 0 0 1 z
	// 0 0 0 1
	// We have to take into account the deltaT when calculating the actual translation

	Matrix::buildIdentity( translation_matrix );
	translation_matrix[3] = x * deltaT / 1000.0f;
	translation_matrix[7] = y * deltaT / 1000.0f;
	translation_matrix[11] = z * deltaT / 1000.0f;
	
	// And applying it to the node's matrix
	Matrix::multiply( translation_matrix, this->matrix_ );
};

void SceneNode::animate( float deltaT ) {
	// If we have velocity, we translate the object
	if( this->velocity_[0] != 0.0f || this->velocity_[1] != 0.0f || this->velocity_[2] != 0.0f )
		this->translate( this->velocity_, deltaT );
	
	// If we have angle velocity, we rotate the object
	if( this->angle_velocity_[0] != 0.0f || this->angle_velocity_[1] != 0.0f || this->angle_velocity_[2] != 0.0f )
		this->rotate( this->angle_velocity_, deltaT );
};