#ifndef _HEIGHTMAPMODEL_H_
#define _HEIGHTMAPMODEL_H_

#include "Model.h"

// Class for loading and displaying heightmaps; it subclasses model and overrides its compile method
class HeightMapModel : public Model {
private:
	// The width and length in units
	// Generally, 1 unit = 1 pixel in the heightmap image = 1.0f in coordinate space
	// This can be adjusted
	int width_;
	int length_;
	float minScale_;
	float maxScale_;

	// Calculates the normals for lighting; this method is magic :D
	// TODO: Still needs some optimization
	void computeNormals();

public:
	// Builds a heightmap from a TGA image and scaling parameters
	// For example, give it a scale between 0.0f and 10.0f all the heights will be interpolated between 0.0f and 10.0f
	HeightMapModel( char *fileName, float minScale = 0.0f, float maxScale = 1.0f );
	// Basic destructor
	~HeightMapModel() {};

	// Overrides the method from Model, because we're rendering with a triangle strip and in a different order
	void compile();

	// Easy access to a particular vertex
	// heightmap( i, j ) = the vertex at position i,j
	Vertex3f* operator()( int i, int j );
};

#endif