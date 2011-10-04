#include "Matrix.h"

void Matrix::buildIdentity( float m[16] ) {
	for( unsigned char i = 0; i < 16; i++ )
		if( i % 5 == 0 ) m[i] = 1.0f;
		else m[i] = 0.0f;
};

bool Matrix::isIdentity( const float m[16] ) {
	for( unsigned char i = 0; i < 16; i++ )
		if( i % 5 == 0 ) {
			if( m[i] != 1.0f ) return false;
		} else {
			if( m[i] != 0.0f ) return false;
		}

	return true;
};

void Matrix::multiply( const float a[16], float b[16] ) {
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

void Matrix::multiply( const float a[16], const float b[16], float c[16] ) {
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

void Matrix::multiplyVector( const float m[16], float v[4] ) {
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