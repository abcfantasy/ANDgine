#ifndef _VERTEX3F_H_
#define _VERTEX3F_H_

class Vertex3f {
private:
	float position_[3];
	float normal_[3];
	float texture_[2];
	float color_[3];

public:
	Vertex3f();
	Vertex3f( float x, float y, float z );
	Vertex3f( float x, float y, float z, float u, float v );
	Vertex3f( float x, float y, float z, float u, float v, float normalX, float normalY, float normalZ );
	Vertex3f( float x, float y, float z, float r, float g, float b );
	Vertex3f( float x, float y, float z, float r, float g, float b, float normalX, float normalY, float normalZ, float u, float v );
	~Vertex3f();

	inline float* getPosition() { return this->position_; };
	inline float getX() { return this->position_[0]; };
	inline float getY() { return this->position_[1]; };
	inline float getZ() { return this->position_[2]; };

	void setPosition( float *position );
	inline void setX( float x ) { this->position_[0] = x; };
	inline void setY( float y ) { this->position_[1] = y; };
	inline void setZ( float z ) { this->position_[2] = z; };

	inline float* getColor() { return this->color_; };
	inline float getR() { return this->color_[0]; };
	inline float getG() { return this->color_[1]; };
	inline float getB() { return this->color_[2]; };

	void setColor( float *color );
	inline void setR( float r ) { this->color_[0] = r; };
	inline void setG( float g ) { this->color_[1] = g; };
	inline void setB( float b ) { this->color_[2] = b; };

	inline float* getTexture() { return this->texture_; };
	inline float getU() { return this->texture_[0]; };
	inline float getV() { return this->texture_[1]; };

	void setTexture( float *texture );
	inline void setU( float u ) { this->texture_[0] = u; };
	inline void setV( float v ) { this->texture_[1] = v; };

	inline float* getNormal() { return this->normal_; };
	inline float getNormalX() { return this->normal_[0]; };
	inline float getNormalY() { return this->normal_[1]; };
	inline float getNormalZ() { return this->normal_[2]; };

	void setNormal( float *normal );
	inline void setNormalX( float nx ) { this->normal_[0] = nx; };
	inline void setNormalY( float ny ) { this->normal_[1] = ny; };
	inline void setNormalZ( float nz ) { this->normal_[2] = nz; };

};

#endif