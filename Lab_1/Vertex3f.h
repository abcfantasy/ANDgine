#ifndef _VERTEX3F_H_
#define _VERTEX3F_H_

class Vertex3f {
private:
	float x_;
	float y_;
	float z_;

	float r_;
	float g_;
	float b_;

public:
	Vertex3f();
	Vertex3f( float x, float y, float z );
	Vertex3f( float x, float y, float z, float r, float g, float b );
	~Vertex3f();

	inline float getX() { return this->x_; };
	inline float getY() { return this->y_; };
	inline float getZ() { return this->z_; };

	inline float getR() { return this->r_; };
	inline float getG() { return this->g_; };
	inline float getB() { return this->b_; };
};

#endif