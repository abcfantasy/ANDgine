#include "SceneNode.h"

void SceneNode::setRotation( float x, float y, float z ) {
	this->rotation_[0] = x;
	this->rotation_[1] = y;
	this->rotation_[2] = z;

	this->compile();
};

void SceneNode::setRotation( float rotation[3] ) {
	this->setRotation( rotation[0], rotation[1], rotation[2] );
};

void SceneNode::setPosition( float x, float y, float z ) {
	this->position_[0] = x;
	this->position_[1] = y;
	this->position_[2] = z;

	this->compile();
};

void SceneNode::setPosition( float position[3] ) {
	this->setPosition( position[0], position[1], position[2] );
};

void SceneNode::setVelocity( float x, float y, float z ) {
	this->velocity_[0] = x;
	this->velocity_[1] = y;
	this->velocity_[2] = z;
};

void SceneNode::setVelocity( float velocity[3] ) {
	this->setVelocity( velocity[0], velocity[1], velocity[2] );
};

void SceneNode::addVelocity( float x, float y, float z ) {
	this->velocity_[0] += x;
	this->velocity_[1] += y;
	this->velocity_[2] += z;
};

void SceneNode::addVelocity( float velocity[3] ) {
	this->addVelocity( velocity[0], velocity[1], velocity[2] );
};

void SceneNode::setAngleVelocity( float x, float y, float z ) {
	this->angle_velocity_[0] = x;
	this->angle_velocity_[1] = y;
	this->angle_velocity_[2] = z;
};

void SceneNode::setAngleVelocity( float angle_velocity[3] ) {
	this->setAngleVelocity( angle_velocity[0], angle_velocity[1], angle_velocity[2] );
};

void SceneNode::addAngleVelocity( float x, float y, float z ) {
	this->angle_velocity_[0] += x;
	this->angle_velocity_[1] += y;
	this->angle_velocity_[2] += z;
};

void SceneNode::addAngleVelocity( float angle_velocity[3] ) {
	this->addAngleVelocity( angle_velocity[0], angle_velocity[1], angle_velocity[2] );
};

void SceneNode::rotate( float x, float y, float z, float deltaT ) {
	this->rotation_[0] += x * deltaT / 1000.0f;
	this->rotation_[1] += y * deltaT / 1000.0f;
	this->rotation_[2] += z * deltaT / 1000.0f;

	this->compile();
};

void SceneNode::rotate( float rotation[3], float deltaT ) {
	this->rotate( rotation[0], rotation[1], rotation[2], deltaT );
};

void SceneNode::translate( float x, float y, float z, float deltaT ) {
	this->position_[0] += x * deltaT / 1000.0f;
	this->position_[1] += y * deltaT / 1000.0f;
	this->position_[2] += z * deltaT / 1000.0f;

	this->compile();
};

void SceneNode::translate( float position[3], float deltaT ) {
	this->translate( position[0], position[1], position[2], deltaT );
};

void SceneNode::worldToModel( float x, float y, float z, float result[4], float type ) {
	float v[4];
	v[0] = x;
	v[1] = y;
	v[2] = z;
	v[3] = 0.0f;
	this->worldToModel( v, result, type );
};

void SceneNode::worldToModel( float coordinates[3], float result[4], float type ) {
	float rotation[16];
	float translation[16];

	result[0] = coordinates[0];
	result[1] = coordinates[1];
	result[2] = coordinates[2];
	result[3] = type;

	Math::identityMatrix( rotation );
	Math::rotationMatrix( this->rotation_[0], this->rotation_[1], this->rotation_[2], rotation );
	Math::multiplyVector( result, rotation );

	//Math::identityMatrix( translation );
	//Math::translationMatrix( this->position_[0], this->position_[1], this->position_[2], translation );
	//Math::multiplyVector( result, translation );
};

void SceneNode::applyVelocity( float deltaT ) {
	if( !Math::isNullVector( this->getVelocity() ) ) {
		float modelSpaceVelocity[4];
		this->worldToModel( this->getVelocity(), modelSpaceVelocity, MATH_VECTOR );
		this->translate( modelSpaceVelocity, deltaT );
	}
	if( !Math::isNullVector( this->getAngleVelocity() ) )
		this->rotate( this->getAngleVelocity(), deltaT );
};