#include "Math.h"
#include <math.h>

void Math::crossProduct( float *a, float *b, float *c, float *result ) {
	float v1[3],v2[3];
	
	v1[0] = b[0] - a[0];
	v1[1] = b[1] - a[1];
	v1[2] = b[2] - a[2];

	v2[0] = c[0] - a[0];
	v2[1] = c[1] - a[1];
	v2[2] = c[2] - a[2];
	
	result[2] = v1[0] * v2[1] - v1[1] * v2[0];
	result[0] = v1[1] * v2[2] - v1[2] * v2[1];
	result[1] = v1[2] * v2[0] - v1[0] * v2[2];
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

bool Math::isNullVector( float *v ) {
	if( v[0] == 0.0f && v[1] == 0.0f && v[2] == 0.0f ) return true;
	return false;
}

void Math::makeNullVector( float *v ) {
	v[0] = v[1] = v[2] = 0.0f;
}