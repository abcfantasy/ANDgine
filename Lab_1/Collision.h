#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "Vertex3f.h"
#include <vector>

class Collision {
private:
	static float* GetFarthestPointInDirection( std::vector<float*> *shape, float* direction );
	static void support( std::vector<float*> *shape1Vertices, std::vector<float*> *shape2Vertices, float* d, float* result );
	static bool containsOrigin( std::vector<float*> *s, float* d );

public:
	static bool GJKCollide( std::vector<Vertex3f> *shape1, std::vector<Vertex3f> *shape2 );
};

#endif