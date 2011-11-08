#pragma once

#include "Vertex3f.h"
#include <vector>

class CollisionManager
{

private:
	float* GetFarthestPointInDirection( std::vector<float*> shape, float* direction );
	float* support( std::vector<float*> shape1Vertices, std::vector<float*> shape2Vertices, float* d );
	float* tripleProduct( float* v1, float* v2, float* v3 );
	bool containsOrigin( std::vector<float*> *s, float* d );

public:
	// singleton
	static CollisionManager* instance();

	bool CollisionManager::GJKCollide( std::vector<Vertex3f> shape1, std::vector<Vertex3f> shape2 );
	bool CollisionManager::GJKCollide( std::vector<float*> shape1, std::vector<Vertex3f> shape2 );
};

