#ifndef _MATRIX_H_
#define _MATRIX_H_

// Static class that provides functions for 4x4 matrix operations

class Matrix {
private:
	// Private constructor/destructor - should never be called
	Matrix();
	~Matrix();
public:
	// Sets the parameter to the I4 identity matrix 
	static void buildIdentity( float m[16] );

	// Checks whether the parameter is the I4 identity matrix
	static bool isIdentity( const float m[16] );
	
	// A and B are 4x4 matrices
	// Performs the operation A * B and puts the result in B
	static void multiply( const float a[16], float b[16] );

	// A and B are 4x4 matrices
	// Performs the operation A * B and puts the result in C
	static void multiply( const float a[16], const float b[16], float c[16] );

	// A is a 4x4 matrix and v is a 4-element vector
	// Performs the operation A * v and puts the result in v
	static void multiplyVector( const float m[16], float v[4] );
};

#endif