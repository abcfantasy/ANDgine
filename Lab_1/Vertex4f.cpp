#include "Vertex4f.h"

Vertex4f::Vertex4f() {
	this->x_ = 0.0f;
	this->y_ = 0.0f;
	this->z_ = 0.0f;
	this->w_ = 1.0f;

	this->r_ = 0.0f;
	this->g_ = 0.0f;
	this->b_ = 0.0f;
};

Vertex4f::Vertex4f( float x, float y, float z, float w = 1.0f ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;

	this->r_ = 0.0f;
	this->g_ = 0.0f;
	this->b_ = 0.0f;
};

Vertex4f::Vertex4f( float x, float y, float z, float w, float u, float v ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;

	this->u_ = u;
	this->v_ = v;
	this->b_ = 0.0f;
};

Vertex4f::Vertex4f( float x, float y, float z, float w, float r, float g, float b ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;

	this->r_ = r;
	this->g_ = g;
	this->b_ = b;
};

Vertex4f::~Vertex4f() {
};