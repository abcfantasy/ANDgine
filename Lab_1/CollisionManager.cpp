#include "CollisionManager.h"
#include "Math.h"

/*
 * <summary>Gets the singleton instane</summary>
 * <returns>The instance of LightingManager</returns>
 */
CollisionManager* CollisionManager::instance()
{
	static CollisionManager cm;
	return &cm;
}

float* CollisionManager::GetFarthestPointInDirection( std::vector<float*> shape, float* direction )
{
	float* farthestPoint = shape[0];
	float highest = Math::dotProduct( direction, shape[0] );
	float current = 0;

	for (int i = 1; i < shape.size(); i++) {
		current = Math::dotProduct( direction, shape[i] );
		if ( current > highest ) {
			highest = current;
			farthestPoint = shape[i];
		}
	}

	return farthestPoint;
}

float* CollisionManager::support( std::vector<float*> shape1Vertices, std::vector<float*> shape2Vertices, float* d )
{
	// d is a vector direction (not necessarily normalized)
	// get points on the edge of the shapes in opposite directions
	float* p1 = GetFarthestPointInDirection( shape1Vertices, d );
	float not_d[3];
	Math::negate( d, not_d );
	float* p2 = GetFarthestPointInDirection( shape2Vertices, not_d );
	// perform minkowski difference
	float p3[3];
	Math::subtract( p1, p2, p3 );
	// p3 is now a point in minkowski space on the edge of the minkowski difference
	return p3;
}

float* CollisionManager::tripleProduct( float* v1, float* v2, float* v3 )
{
	float result[3];
	Math::crossProduct( v1, v2, v3, result );
	return result;
}

bool CollisionManager::containsOrigin( std::vector<float*> *s, float* d )
{
	// get the last point added to the simplex
	float* a = s->back();
	// compute AO (same thing as -A)
	float ao[3];
	Math::negate( a, ao );
	// if simplex is triangle
	if ( s->size() == 3 ) {
		// get b and c
		float* b = (*s)[1];
		float* c = (*s)[0];
		// compute the edges
		float ab[3];
		Math::subtract( b, a, ab );
		float ac[3];
		Math::subtract( c, a, ac );
		// compute the normals
		float* abNormal = tripleProduct( ac, ab, ab );
		float* acNormal = tripleProduct( ab, ac, ac );
		// is th origin in R4?
		if ( Math::dotProduct( abNormal, ao ) > 0 ) {
			// remove point c
			(*s)[0] = (*s)[1];
			(*s)[1] = (*s)[2];
			s->pop_back();
			// set the new direction to abNormal
			d[0] = abNormal[0];
			d[1] = abNormal[1];
			d[2] = abNormal[2];
		} 
		else {
			// is the origin in R3?
			if ( Math::dotProduct( acNormal, ao ) > 0 ) {
				// remove point b
				(*s)[1] = (*s)[2];
				s->pop_back();
				// set the new direction to acNormal
				d[0] = acNormal[0];
				d[1] = acNormal[1];
				d[2] = acNormal[2];
			} 
			else {
				// otherwise we know its in R5 so we can return true
				return true;
			}
		}
	}
	// else if simplex is line segment
	else {
		// get b
		float* b = (*s)[0];
		// compute ab
		float ab[3];
		Math::subtract( b, a, ab );
		// get the perpendicular/normal to AB in the direction of the origin
		float* abNormal = tripleProduct( ab, ao, ab );
		// set the direction to abNormal
		d[0] = abNormal[0];
		d[1] = abNormal[1];
		d[2] = abNormal[2];
	}
	
	return false;
}

bool CollisionManager::GJKCollide( std::vector<Vertex3f> shape1, std::vector<Vertex3f> shape2 )
{
	// change vector3f to float array
	std::vector<float*> shape1Vertices;
	std::vector<float*> shape2Vertices;
	for ( int i = 0; i < shape1.size(); i++ )
		shape1Vertices.push_back( shape1[i].getPosition() );
	for ( int i = 0; i < shape2.size(); i++ )
		shape2Vertices.push_back( shape2[i].getPosition() );

	// create simplex queue
	std::vector<float*> simplexList;

	// choose direction
	float d[3];
	d[0] = 1.0f;
	d[1] = 0.0f;
	d[2] = 0.0f;
	// get the first minkowski different point
	simplexList.push_back( support( shape1Vertices, shape2Vertices, d ) );
	// negate d for next point
	Math::negate( d, d );
	// start looping
	while (true) 
	{
		// add a new point to the simplex because we haven't terminated yet
		simplexList.push_back( support( shape1Vertices, shape2Vertices, d ) );
		// make sure that the last point we added actually passed the origin
		if ( Math::dotProduct( simplexList.back(), d ) <= 0 )
		{
			// if the point added last was not past te origin in the direction of d
			// then the minkowski sum cannot possibly contain the origin since
			// the last point added is on the edge of minkowski difference.
			return false;
		} 
		else 
		{
			// otherwise we need to determine if the origin is in
			// the current simplex
			if ( containsOrigin( &simplexList, d ) ) 
			{
				// if it does, then we know there is a collision
				return true;
			}
		}
	}
}