#ifndef _SCENENODE_H_
#define _SCENENODE_H_

class SceneNode {
private:
	int displayListId_;
	float rotation_[3];
	float translation_[3];
	float velocity_[3];
	float angle_velocity_[3];

public:
	inline float* getRotation() { return this->rotation_; };
	inline float* getTranslation() { return this->translation_; };
	inline int getDisplayListId() { return this->displayListId_; };
	inline float* getVelocity() { return this->velocity_; };
	inline float* getAngleVelocity() { return this->angle_velocity_; };

	inline void setDisplayListId( int id ) { this->displayListId_ = id; };
	void setRotation( float x, float y, float z );
	void setRotation( float rotation[3] );
	void setTranslation( float x, float y, float z );
	void setTranslation( float translation[3] );
	void setVelocity( float x, float y, float z );
	void setVelocity( float velocity[3] );
	void setAngleVelocity( float x, float y, float z );
	void setAngleVelocity( float angle_velocity[3] );

	void rotate( float rotation[3], float deltaT );
	void translate( float translation[3], float deltaT );

	virtual void compile() = 0;
	virtual void render( float deltaT ) = 0;
};

#endif