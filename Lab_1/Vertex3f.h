#ifndef _VERTEX3F_H_
#define _VERTEX3F_H_

class Vertex3f {
private:
	float x_;
	float y_;
	float z_;

	union{
		float r_;
		float u_;
	};
	union{
		float g_;
		float v_;
	};
	float b_;

public:
	Vertex3f();
	Vertex3f( float x, float y, float z );
	Vertex3f( float x, float y, float z, float u, float v );
	Vertex3f( float x, float y, float z, float r, float g, float b );
	~Vertex3f();

	inline float getX() { return this->x_; };
	inline float getY() { return this->y_; };
	inline float getZ() { return this->z_; };

	inline float getR() { return this->r_; };
	inline float getG() { return this->g_; };
	inline float getB() { return this->b_; };

	inline float getU() { return this->u_; };
	inline float getV() { return this->v_; };

	inline void setX( float x ) { this->x_ = x; };
	inline void setY( float y ) { this->y_ = y; };
	inline void setZ( float z ) { this->z_ = z; };
	
	inline void setR( float r ) { this->r_ = r; };
	inline void setG( float g ) { this->g_ = g; };
	inline void setB( float b ) { this->b_ = b; };
	
	inline void setU( float u ) { this->u_ = u; };
	inline void setV( float v ) { this->v_ = v; };
};

#endif