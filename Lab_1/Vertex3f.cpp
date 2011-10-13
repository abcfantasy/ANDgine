#include "Vertex3f.h"

Vertex3f::Vertex3f() {
	this->x_ = 0.0;
	this->y_ = 0.0;
	this->z_ = 0.0;

	this->r_ = 1.0;
	this->g_ = 1.0;
	this->b_ = 1.0;
};

Vertex3f::Vertex3f( float x, float y, float z ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;

	this->r_ = 1.0;
	this->g_ = 1.0;
	this->b_ = 1.0;
};

Vertex3f::Vertex3f( float x, float y, float z, float u, float v ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;

	this->u_ = u;
	this->v_ = v;
	this->b_ = 0.0;
};

Vertex3f::Vertex3f( float x, float y, float z, float u, float v, float normalX, float normalY, float normalZ ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;

	this->u_ = u;
	this->v_ = v;
	this->b_ = 0.0;

	this->normalX_ = normalX;
	this->normalY_ = normalY;
	this->normalZ_ = normalZ;
}

Vertex3f::Vertex3f( float x, float y, float z, float r, float g, float b ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;

	this->r_ = r;
	this->g_ = g;
	this->b_ = b;
};

Vertex3f::Vertex3f( float x, float y, float z, float r, float g, float b, float normalX, float normalY, float normalZ, float u, float v ) {
	this->x_ = x;
	this->y_ = y;
	this->z_ = z;

	this->r_ = r;
	this->g_ = g;
	this->b_ = b;

	this->u_ = u;
	this->v_ = v;

	this->normalX_ = normalX;
	this->normalY_ = normalY;
	this->normalZ_ = normalZ;
}

Vertex3f::~Vertex3f() {
};