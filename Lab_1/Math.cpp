#include "Math.h"
#define _USE_MATH_DEFINES
#include <math.h>

void Math::crossProduct( float *a, float *b, float *c, float *result ) {
	/*float v1[3],v2[3];
	
	v1[0] = b[0] - a[0];
	v1[1] = b[1] - a[1];
	v1[2] = b[2] - a[2];

	v2[0] = c[0] - a[0];
	v2[1] = c[1] - a[1];
	v2[2] = c[2] - a[2];
	
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];*/
	float r[3];
	Math::crossProduct( a, b, r );
	Math::crossProduct( r, c, result );
}

void Math::crossProduct( float *a, float *b, float *result ) {
	result[0] = (a[1] * b[2]) - (a[2] * b[1]);
	result[1] = (a[2] * b[0]) - (a[0] * b[2]);
	result[2] = (a[0] * b[1]) - (a[1] * b[0]);
}

float Math::dotProduct( float *a, float *b ) {
	return (a[0] * b[0]) + (a[1] * b[1]) + (a[2] * b[2]);
}

void Math::negate( float *a, float *result ) {
	result[0] = -a[0];
	result[1] = -a[1];
	result[2] = -a[2];
}

void Math::normalize( float *v ) {
	float d;

	if( Math::isNullVector( v ) ) return;

	d = sqrtf(( v[0] * v[0] ) + ( v[1] * v[1] ) + ( v[2] * v[2] ));
	v[0] /= d;
	v[1] /= d;
	v[2] /= d;
}

void Math::addVector (float *a, float *b ) {
	a[0] += b[0];
	a[1] += b[1];
	a[2] += b[2];
}

void Math::subtract( float *a, float *b, float *result ) {
	result[0] = a[0] - b[0];
	result[1] = a[1] - b[1];
	result[2] = a[2] - b[2];
}

bool Math::isNullVector( float *v ) {
	if( v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f ) return true;
	return false;
}

void Math::makeNullVector( float *v ) {
	v[0] = v[1] = v[2] = 0.0f;
}

int Math::sign( float *v )
{
	if ( v < 0 )
		return -1;
	else if ( v > 0 )
		return 1;
	else
		return 0;
}

void Math::identityMatrix( float m[16] ) {
	for( unsigned char i = 0; i < 16; i++ )
		if( i % 5 == 0 ) m[i] = 1.0f;
		else m[i] = 0.0f;
};

void Math::translationMatrix( float x, float y, float z, float m[16] ) {
	// 1 0 0 0
	// 0 1 0 0
	// 0 0 1 0
	// x y z 1
	for( unsigned char i = 0; i < 12; i++ )
		if( i % 5 == 0 ) m[i] = 1.0f;
		else m[i] = 0.0f;
	m[12] = x;
	m[13] = y;
	m[14] = z;
	m[15] = 1;
};

void Math::rotationMatrix( float x, float y, float z, float m[16] ) {
	float rotation_matrix[16];
	
	// Here we are transforming the rotation parameters to radians
	// Taking into account that the parameter is given in degrees
	x *= float(M_PI) / 180.0f;
	y *= float(M_PI) / 180.0f;
	z *= float(M_PI) / 180.0f;
	
	// Are we rotating around the X-axis?
	if( x != 0.0f ) {
		// If we are, then we're building the rotation matrix
		// 1   0    0 0
		// 0 cos -sin 0
		// 0 sin  cos 0
		// 0   0    0 1
		Math::identityMatrix( rotation_matrix );
		rotation_matrix[5] = rotation_matrix[10] = cosf( x );
		rotation_matrix[6] = -sinf( x );
		rotation_matrix[9] = -rotation_matrix[6];
		
		// And applying it to the node's matrix
		Math::multiply( rotation_matrix, m );
	}

	// Are we rotating around the Y-axis?
	if( y != 0.0f ) {
		// If we are, then we're building the rotation matrix
		// cos 0  sin 0
		//   0 1    0 0
		//-sin 0  cos 0
		//   0 0    0 1
		Math::identityMatrix( rotation_matrix );
		rotation_matrix[0] = rotation_matrix[10] = cosf( y );
		rotation_matrix[2] = sinf( y );
		rotation_matrix[8] = -rotation_matrix[2];
		
		// And applying it to the node's matrix
		Math::multiply( rotation_matrix, m );
	}

	// Are we rotating around the Z-axis?
	if( z != 0.0f ) {
		// If we are, then we're building the rotation matrix
		// cos -sin 0 0
		// sin  cos 0 0
		//   0    0 1 0
		//   0    0 0 1
		Math::identityMatrix( rotation_matrix );
		rotation_matrix[0] = rotation_matrix[5] = cosf( z );
		rotation_matrix[1] = -sinf( z );
		rotation_matrix[4] = -rotation_matrix[1];
		
		// And applying it to the node's matrix
		Math::multiply( rotation_matrix, m );
	}
};

bool Math::isIdentity( const float m[16] ) {
	for( unsigned char i = 0; i < 16; i++ )
		if( i % 5 == 0 ) {
			if( m[i] != 1.0f ) return false;
		} else {
			if( m[i] != 0.0f ) return false;
		}

	return true;
};

void Math::multiply( const float a[16], float b[16] ) {
	float c[16];
	unsigned char i, j, k;
	
	for( i = 0; i < 4; i++ )
		for( j = 0; j < 4; j++ )
			c[i * 4 + j] = b[ i * 4 + j ];

	for( i = 0; i < 4; i++ ) {
		for( j = 0; j < 4; j++ ) {
			b[i * 4 + j] = 0.0f;
			for( k = 0; k < 4; k++ ) {
				b[i * 4 + j] += a[i * 4 + k] * c[k * 4 + j];
			}
		}
	}
};

void Math::multiply( const float a[16], const float b[16], float c[16] ) {
	unsigned char i, j, k;
	for( i = 0; i < 4; i++ ) {
		for( j = 0; j < 4; j++ ) {
			c[i * 4 + j] = 0.0f;
			for( k = 0; k < 4; k++ ) {
				c[i * 4 + j] += a[i * 4 + k] * b[k * 4 + j];
			}
		}
	}
};

void Math::multiplyVector( float v[4], const float m[16] ) {
	unsigned char i, j;
	float v_aux[4];

	for( i = 0; i < 4; i++ )
		v_aux[i] = v[i];

	for( i = 0; i < 4; i++ ) {
		v[i] = 0.0f;
		for( j = 0; j < 4; j++ ) {
			v[i] += m[i * 4 + j] * v_aux[j];
		}
	}
};