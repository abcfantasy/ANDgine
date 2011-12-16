#ifndef _COLLISIONMANAGER_H_
#define _COLLISIONMANAGER_H_

#include "Vertex3f.h"
#include <vector>

class CollisionManager
{

private:
	float* GetFarthestPointInDirection( std::vector<float*> shape, float* direction );
	void support( std::vector<float*> shape1Vertices, std::vector<float*> shape2Vertices, float* d, float* result );
	bool containsOrigin( std::vector<float*> *s, float* d );

public:
	// singleton
	static CollisionManager* instance();

	bool CollisionManager::GJKCollide( std::vector<Vertex3f>* shape1, std::vector<Vertex3f>* shape2 );
};

#endif