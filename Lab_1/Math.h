#ifndef _MATH_H_
#define _MATH_H_

const float MATH_VECTOR	= 0.0f;
const float MATH_POINT	= 1.0f;

// Some mathematical functions we can use
class Math {
public:
	static void crossProduct( float *a, float *b, float *c, float *result );
	static void crossProduct( float *a, float *b, float *result );
	static float dotProduct( float *a, float *b );
	static void negate( float *a, float *result );
	static void addVector( float *a, float *b );
	static void subtract( float *a, float *b, float *result );
	static void normalize( float *v );
	static bool isNullVector( float *v );
	static void makeNullVector( float *v );
	static int sign( float *v );

	// Sets the parameter to the I4 identity matrix 
	static void identityMatrix( float m[16] );

	// Checks whether the parameter is the I4 identity matrix
	static bool isIdentity( const float m[16] );

	static void translationMatrix( float x, float y, float z, float m[16] );
	static void rotationMatrix( float x, float y, float z, float m[16] );

	// A and B are 4x4 matrices
	// Performs the operation A * B and puts the result in B
	static void multiply( const float a[16], float b[16] );

	// A and B are 4x4 matrices
	// Performs the operation A * B and puts the result in C
	static void multiply( const float a[16], const float b[16], float c[16] );

	// v is a 4-element vector and M is a 4x4 matrix and 
	// Performs the operation v * M and puts the result in v
	static void multiplyVector( float v[4], const float m[16] );
};

#endif