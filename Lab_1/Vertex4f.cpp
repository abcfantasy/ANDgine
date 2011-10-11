#include "Vertex4f.h"

Vertex4f::Vertex4f() {
	this->x_ = 0.0f;
	this->y_ = 0.0f;
	this->z_ = 0.0f;
	this->w_ = 1.0f;

	this->nX_ = 0.0f;
	this->nY_ = 0.0f;
	this->nZ_ = 0.0f;

	this->r_ = 0.0f;
	this->g_ = 0.0f;
	this->b_ = 0.0f;

	this->hasNormal_ = false;
};

Vertex4f::Vertex4f( float x, float y, float z, float w = 1.0f ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;
	
	this->nX_ = 0.0f;
	this->nY_ = 0.0f;
	this->nZ_ = 0.0f;

	this->r_ = 0.0f;
	this->g_ = 0.0f;
	this->b_ = 0.0f;

	this->hasNormal_ = false;
};

Vertex4f::Vertex4f( float x, float y, float z, float w, float u, float v ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;
	
	this->nX_ = 0.0f;
	this->nY_ = 0.0f;
	this->nZ_ = 0.0f;

	this->u_ = u;
	this->v_ = v;
	this->b_ = 0.0f;

	this->hasNormal_ = false;
};

Vertex4f::Vertex4f( float x, float y, float z, float w, float u, float v, float nX, float nY, float nZ ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;

	this->nX_ = nX;
	this->nY_ = nY;
	this->nZ_ = nZ;

	this->u_ = u;
	this->v_ = v;
	this->b_ = 0.0f;

	this->hasNormal_ = true;
};

Vertex4f::Vertex4f( float x, float y, float z, float w, float r, float g, float b ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;
	
	this->nX_ = 0.0f;
	this->nY_ = 0.0f;
	this->nZ_ = 0.0f;

	this->r_ = r;
	this->g_ = g;
	this->b_ = b;

	this->hasNormal_ = false;
};

Vertex4f::Vertex4f( float x, float y, float z, float w, float r, float g, float b, float nX, float nY, float nZ ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;

	this->nX_ = nX;
	this->nY_ = nY;
	this->nZ_ = nZ;

	this->r_ = r;
	this->g_ = g;
	this->b_ = b;

	this->hasNormal_ = true;
};


Vertex4f::~Vertex4f() {
};

void Vertex4f::setPosition( float position[4] ) {
	this->setPosition( position[0], position[1], position[2], position[3] );
};

void Vertex4f::setPosition( float x, float y, float z, float w ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;
	this->w_ = w;
};

void Vertex4f::setNormal( float normal[3] ) {
	this->setNormal( normal[0], normal[1], normal[2] );
};

void Vertex4f::setNormal( float nX, float nY, float nZ ) {
	this->nX_ = nX;
	this->nY_ = nY;
	this->nZ_ = nZ;

	this->hasNormal_ = true;
};

void Vertex4f::setColor( float color[3] ) {
	this->setColor( color[0], color[1], color[2] );
};

void Vertex4f::setColor( float r, float g, float b ) {
	this->r_ = r;
	this->g_ = g;
	this->b_ = b;
};
