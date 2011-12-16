#include "CollisionManager.h"
#include "Math.h"

/*
 * <summary>Gets the singleton instane</summary>
 * <returns>The instance of CollisionManager</returns>
 */
CollisionManager* CollisionManager::instance()
{
	static CollisionManager cm;
	return &cm;
}

float* CollisionManager::GetFarthestPointInDirection( std::vector<float*> shape, float* direction )
{
	float* farthestPoint = shape[0];
	float highest = Math::dotProduct( shape[0], direction );
	float current = 0;

	for ( unsigned int i = 1; i < shape.size(); i++) {
		current = Math::dotProduct( shape[i], direction );
		if ( current >= highest ) {
			highest = current;
			farthestPoint = shape[i];
		}
	}

	return farthestPoint;
}

void CollisionManager::support( std::vector<float*> shape1Vertices, std::vector<float*> shape2Vertices, float* d, float* result )
{
	// d is a vector direction (not necessarily normalized)
	// get points on the edge of the shapes in opposite directions
	float* p1 = GetFarthestPointInDirection( shape1Vertices, d );
	float not_d[3];
	Math::negate( d, not_d );
	float* p2 = GetFarthestPointInDirection( shape2Vertices, not_d );
	// perform minkowski difference
	Math::subtract( p1, p2, result );
	// result is now a point in minkowski space on the edge of the minkowski difference
}
bool CollisionManager::containsOrigin( std::vector<float*> *s, float* d )
{
	// get the last point added to the simplex
	float* a = s->back();
	// compute AO (same thing as -A)
	float ao[3];
	Math::negate( a, ao );
	// if simplex is line
	if ( s->size() == 2 ) {
		// get b
		float* b = (*s)[0];
		// compute ab
		float ab[3];
		Math::subtract( b, a, ab );
		if ( Math::dotProduct( ab, ao ) > 0 )
		{
			float dir[3];
			Math::crossProduct( ab, ao, ab, dir );
			d[0] = dir[0];
			d[1] = dir[1];
			d[2] = dir[2];
		}
		else
		{
			d[0] = ao[0];
			d[1] = ao[1];
			d[2] = ao[2];
		}
	}
	// if simplex is triangle
	else if ( s->size() == 3 ) {
		// get b and c
		float* b = (*s)[1];
		float* c = (*s)[0];
		// compute the edges
		float ab[3];
		Math::subtract( b, a, ab );
		float ac[3];
		Math::subtract( c, a, ac );
		float abc[3];
		Math::crossProduct( ab, ac, abc );
		// START MOD
		float check[3];
		Math::crossProduct( abc, ac, check );
		if ( Math::dotProduct( check, ao ) > 0 )
		{
			if ( Math::dotProduct( ac, ao ) > 0 )
			{
				// remove point b
				(*s)[1] = (*s)[2];
				s->pop_back();
				// set new direction
				float dir[3];
				Math::crossProduct( ac, ao, ac, dir );
				d[0] = dir[0];
				d[1] = dir[1];
				d[2] = dir[2];
			}
			else if ( Math::dotProduct( ab, ao ) > 0 )
			{
				// remove point c
				(*s)[0] = (*s)[1];
				(*s)[1] = (*s)[2];
				s->pop_back();
				// set new direction
				float dir[3];
				Math::crossProduct( ab, ao, ab, dir );
				d[0] = dir[0];
				d[1] = dir[1];
				d[2] = dir[2];
			}
			else {
				// remove points a and b
				(*s)[0] = (*s)[2];
				s->pop_back();
				s->pop_back();
				// set new direction
				d[0] = ao[0];
				d[1] = ao[1];
				d[2] = ao[2];
			}
		}
		else
		{
			float check2[3];
			Math::crossProduct( ab, abc, check2 );
			if ( Math::dotProduct( check2, ao ) > 0 )
			{
				if ( Math::dotProduct( ab, ao ) > 0 )
				{
					// remove point c
					(*s)[0] = (*s)[1];
					(*s)[1] = (*s)[2];
					s->pop_back();
					// set new direction
					float dir[3];
					Math::crossProduct( ab, ao, ab, dir );
					d[0] = dir[0];
					d[1] = dir[1];
					d[2] = dir[2];
				}
				else
				{
					// remove points a and b
					(*s)[0] = (*s)[2];
					s->pop_back();
					s->pop_back();
					// set new direction
					d[0] = ao[0];
					d[1] = ao[1];
					d[2] = ao[2];
				}
			}
			else
			{
				if ( Math::dotProduct( abc, ao ) > 0 )
				{
					// the simplex stays a, b, c
					d[0] = abc[0];
					d[1] = abc[1];
					d[2] = abc[2];
				}
				else
				{
					float *temp = s->front();
					(*s)[0] = (*s)[1];
					(*s)[1] = (*s)[2];
					(*s)[2] = temp;

					d[0] = -abc[0];
					d[1] = -abc[1];
					d[2] = -abc[2];
				}
			}
		}
	}
	// else if simplex is tetrahedon
	else {
		// a is the last point added to the simplex
		float *a = (*s)[3];
		float *b = (*s)[2];
		float *c = (*s)[1];
		float *d = (*s)[0];

		
		float *ab = new float[3];
		float *ac = new float[3];
		float *ad = new float[3];
		float *abc = new float[3];
		float *acd = new float[3];
		float *adb = new float[3];
		
		Math::subtract( b, a, ab );
		Math::subtract( c, a, ac );
		Math::subtract( d, a, ad );
		Math::crossProduct( ab, ac, abc );
		Math::crossProduct( ac, ad, acd );
		Math::crossProduct( ad, ab, adb );

		// the side (positive or negative) of B, C and D relative to the planes of
		// ACD, ADB and ABC respectively
		float v = Math::dotProduct( acd, ab );
		int BsideOnACD = Math::sign( &v );
		v = Math::dotProduct( adb, ac );
		int CsideOnADB = Math::sign( &v );
		v = Math::dotProduct( abc, ad );
		int DsideOnABC = Math::sign( &v );

		// whether the origin is on the same side of ACD/ADB/ABC as
		// B, C and D respectively
		v = Math::dotProduct( acd, ao );
		bool ABsameAsOrigin = Math::sign( &v ) == BsideOnACD;
		v = Math::dotProduct( adb, ao );
		bool ACsameAsOrigin = Math::sign( &v ) == CsideOnADB;
		v = Math::dotProduct( adb, ao );
		bool ADsameAsOrigin = Math::sign( &v ) == DsideOnABC;

		// if the origin is on the same side as all B, C and D, the origin
		// is inside the tetrahedron and thus there is a collision
		if ( ABsameAsOrigin && ACsameAsOrigin && ADsameAsOrigin )
			return true;
		// if the origin is not on the side of B relative to ACD
		else if ( !ABsameAsOrigin )
		{
			// B is the farthest from the origin among all of the
			// tetrahedron's points, so remove it from the list and
			// go on with the triangle case
			s[2] = s[3];
			s->pop_back();
			// the new direction is on the other side of ACD, relateive
			// to B
			d[0] = acd[0] * -BsideOnACD;
			d[1] = acd[1] * -BsideOnACD;
			d[2] = acd[2] * -BsideOnACD;
		}
		// if the origin is not on the side of C relative to ADB
		else if ( !ACsameAsOrigin )
		{
			// C is farthest from the origin among all of the
			// tetrahedron's points,s o remove it from the list and
			// go on with the triangle case
			s[1] = s[2];
			s[2] = s[3];
			s->pop_back();
			// the new direction is on the other side of ADB
			// relative to C
			d[0] = adb[0] * -CsideOnADB;
			d[1] = adb[1] * -CsideOnADB;
			d[2] = adb[2] * -CsideOnADB;
		}
		// if the origin is not on the side of D relative to ABC
		else //if ( !ADsameAsOrigin )
		{
			// D is the farthest from the origin among all of the
			// tetrahedron's points, so remove it from the list and
			// go on with the triangle case
			s[0] = s[1];
			s[1] = s[2];
			s[2] = s[3];
			s->pop_back();
			// the new direction is on the other side of ABC.
			// relateive to D
			d[0] = abc[0] * -DsideOnABC;
			d[1] = abc[1] * -DsideOnABC;
			d[2] = abc[2] * -DsideOnABC;
		}
		// go on with the triangle case
		// TODO: maybe we should restrict the depth of recursion
		return containsOrigin( s, d );
	}
	
	return false;
}

bool CollisionManager::GJKCollide( std::vector<Vertex3f>* shape1, std::vector<Vertex3f>* shape2 )
{
	// change vector3f to float array
	std::vector<float*> shape1Vertices;
	std::vector<float*> shape2Vertices;
	for ( std::vector<Vertex3f>::iterator i = shape1->begin(); i != shape1->end(); ++i )
		shape1Vertices.push_back( i->getPosition() );
	for ( std::vector<Vertex3f>::iterator i = shape2->begin(); i != shape2->end(); ++i )
		shape2Vertices.push_back( i->getPosition() );

	// create simplex queue
	std::vector<float*> simplexList;

	// initial point
	float initialPoint[3];
	Math::subtract( shape1Vertices[0], shape2Vertices[0], initialPoint );
	// get the first minkowski different point
	float *s = new float[3];
	support( shape1Vertices, shape2Vertices, initialPoint, s );
	simplexList.push_back( s );
	// set direction
	float d[3];
	Math::negate( s, d );
	// start looping
 	while (true) 
	{
		// add a new point to the simplex because we haven't terminated yet
		float *newPoint = new float[3];
		support( shape1Vertices, shape2Vertices, d, newPoint );
		simplexList.push_back( newPoint );
		// make sure that the last point we added actually passed the origin
		if ( Math::dotProduct( simplexList.back(), d ) < 0 )
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