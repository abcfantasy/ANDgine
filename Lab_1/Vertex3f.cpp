#include "Vertex3f.h"

Vertex3f::Vertex3f() {
	this->position_[0] = 0.0;
	this->position_[1] = 0.0;
	this->position_[2] = 0.0;

	this->color_[0] = 1.0;
	this->color_[1] = 1.0;
	this->color_[2] = 1.0;
};

Vertex3f::Vertex3f( float x, float y, float z ) {
	this->position_[0] = x;
	this->position_[1] = y;
	this->position_[2] = z;

	this->color_[0] = 1.0;
	this->color_[0] = 1.0;
	this->color_[0] = 1.0;
};

Vertex3f::Vertex3f( float x, float y, float z, float u, float v ) {
	this->position_[0] = x;
	this->position_[0] = y;
	this->position_[0] = z;

	this->texture_[0] = u;
	this->texture_[1] = v;
};

Vertex3f::Vertex3f( float x, float y, float z, float u, float v, float normalX, float normalY, float normalZ ) {
	this->position_[0] = x;
	this->position_[1] = y;
	this->position_[2] = z;

	this->texture_[0] = u;
	this->texture_[1] = v;

	this->normal_[0] = normalX;
	this->normal_[1] = normalY;
	this->normal_[2] = normalZ;
}

Vertex3f::Vertex3f( float x, float y, float z, float r, float g, float b ) {
	this->position_[0] = x;
	this->position_[1] = y;
	this->position_[2] = z;

	this->color_[0] = r;
	this->color_[1] = g;
	this->color_[2] = b;
};

Vertex3f::Vertex3f( float x, float y, float z, float r, float g, float b, float normalX, float normalY, float normalZ, float u, float v ) {
	this->position_[0] = x;
	this->position_[1] = y;
	this->position_[2] = z;

	this->color_[0] = r;
	this->color_[1] = g;
	this->color_[2] = b;

	this->texture_[0] = u;
	this->texture_[1] = v;

	this->normal_[0] = normalX;
	this->normal_[1] = normalY;
	this->normal_[2] = normalZ;
}

Vertex3f::~Vertex3f() {
};

void Vertex3f::setPosition( float *position ) {
	this->position_[0] = position[0];
	this->position_[1] = position[1];
	this->position_[2] = position[2];
};

void Vertex3f::setColor( float *color ) {
	this->color_[0] = color[0];
	this->color_[1] = color[1];
	this->color_[2] = color[2];
};

void Vertex3f::setTexture( float *texture ) {
	this->texture_[0] = texture[0];
	this->texture_[1] = texture[1];
};

void Vertex3f::setNormal( float *normal ) {
	this->normal_[0] = normal[0];
	this->normal_[1] = normal[1];
	this->normal_[2] = normal[2];
};