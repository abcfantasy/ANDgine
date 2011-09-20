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

void SceneNode::setTranslation( float x, float y, float z ) {
	this->translation_[0] = x;
	this->translation_[1] = y;
	this->translation_[2] = z;

	this->compile();
};

void SceneNode::setTranslation( float translation[3] ) {
	this->setTranslation( translation[0], translation[1], translation[2] );
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
	this->rotation_[0] += rotation[0] * deltaT / 1000;
	this->rotation_[1] += rotation[1] * deltaT / 1000;
	this->rotation_[2] += rotation[2] * deltaT / 1000;

	this->compile();
};

void SceneNode::translate( float translation[3], float deltaT ) {
	this->translation_[0] += translation[0] * deltaT / 1000;
	this->translation_[1] += translation[1] * deltaT / 1000;
	this->translation_[2] += translation[2] * deltaT / 1000;

	this->compile();
};