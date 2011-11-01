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

void SceneNode::setAngleVelocity( float x, float y, float z ) {
	this->angle_velocity_[0] = x;
	this->angle_velocity_[1] = y;
	this->angle_velocity_[2] = z;
};

void SceneNode::setAngleVelocity( float angle_velocity[3] ) {
	this->setAngleVelocity( angle_velocity[0], angle_velocity[1], angle_velocity[2] );
};

void SceneNode::rotate( float rotation[3], float deltaT ) {
	this->rotation_[0] += rotation[0] * deltaT / 1000.0f;
	this->rotation_[1] += rotation[1] * deltaT / 1000.0f;
	this->rotation_[2] += rotation[2] * deltaT / 1000.0f;

	this->compile();
};

void SceneNode::translate( float position[3], float deltaT ) {
	this->position_[0] += position[0] * deltaT / 1000.0f;
	this->position_[1] += position[1] * deltaT / 1000.0f;
	this->position_[2] += position[2] * deltaT / 1000.0f;

	this->compile();
};