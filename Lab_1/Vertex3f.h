#ifndef _VERTEX3F_H_
#define _VERTEX3F_H_

class Vertex3f {
private:
	float x_;
	float y_;
	float z_;

	float normalX_;
	float normalY_;
	float normalZ_;

	// texture
	float u_;
	float v_;

	// color
	float r_;
	float g_;
	float b_;

public:
	Vertex3f();
	Vertex3f( float x, float y, float z );
	Vertex3f( float x, float y, float z, float u, float v );
	Vertex3f( float x, float y, float z, float u, float v, float normalX, float normalY, float normalZ );
	Vertex3f( float x, float y, float z, float r, float g, float b );
	Vertex3f( float x, float y, float z, float r, float g, float b, float normalX, float normalY, float normalZ, float u, float v );
	~Vertex3f();

	inline float getX() { return this->x_; };
	inline float getY() { return this->y_; };
	inline float getZ() { return this->z_; };

	inline void setX( float x ) { this->x_ = x; }
	inline void setY( float y ) { this->y_ = y; }
	inline void setZ( float z ) { this->z_ = z; }

	inline float getR() { return this->r_; };
	inline float getG() { return this->g_; };
	inline float getB() { return this->b_; };

	inline float getU() { return this->u_; };
	inline float getV() { return this->v_; };

	inline float getNormalX() { return this->normalX_; };
	inline float getNormalY() { return this->normalY_; };
	inline float getNormalZ() { return this->normalZ_; };

};

#endif