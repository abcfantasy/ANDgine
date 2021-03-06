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
	static const int DIRECTION_NORTH = 0;
	static const int DIRECTION_SOUTH = 1;
	static const int DIRECTION_EAST  = 2;
	static const int DIRECTION_WEST  = 3;

	static int NEXT_DIRECTION;
	static float *PREVIOUS_MAP;

	// Builds a heightmap from a TGA image and scaling parameters
	// For example, give it a scale between 0.0f and 10.0f all the heights will be interpolated between 0.0f and 10.0f
	HeightMapModel( const unsigned int handle, char *fileName, float minScale = 0.0f, float maxScale = 1.0f ) :
		Model( handle, fileName ),
		minScale_( minScale ), maxScale_( maxScale )
		{};
	// Basic destructor
	~HeightMapModel() {};

	// Overrides the method from Model, because we're rendering with a triangle strip and in a different order
	void compile();

	// Easy access to a particular vertex
	// heightmap( i, j ) = the vertex at position i,j
	Vertex3f* operator()( int i, int j );

	// Gets the interpolated height at the x, z coordinates
	float getHeight( float x, float z );

	// Special collision method - we're not using a bounding box for this kind of model
	bool checkSpecialCollision( float *position );

	// Loads the model from the image file
	bool load();
	bool loadFromSeed();
	bool loadFromImage();

	// Rescales the model between min and max
	void rescale( float minScale = 0.0f, float maxScale = 1.0f );
};

#endif