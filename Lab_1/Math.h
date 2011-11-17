#ifndef _MATH_H_
#define _MATH_H_

// Some mathematical functions we can use
class Math {
public:
	static void crossProduct( float *a, float *b, float *c, float *result );
	static void Math::crossProduct( float *a, float *b, float *result );
	static float dotProduct( float *a, float *b );
	static void negate( float *a, float *result );
	static void addVector( float *a, float *b );
	static void subtract( float *a, float *b, float *result );
	static void normalize( float *v );
	static bool isNullVector( float *v );
	static void makeNullVector( float *v );
	static int Math::sign( float *v );
};

#endif