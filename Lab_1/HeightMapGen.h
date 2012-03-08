#ifndef _HEIGHTMAPGEN_H_
#define _HEIGHTMAPGEN_H_

// Static class that generates a random heightmap
class HeightMapGen
{
private:
	// private functions that aid in the generation method
	static float randnum (float min, float max);
	static float avgDiamondVals (int i, int j, int stride, int size, int subSize, float *map);
	static float avgSquareVals (int i, int j, int stride, int size, float *map);
	// generates a random array for use as heightmap
	static void fill2DFractArray (float *map, int size, int seedValue, float heightScale, float h);

public:
	// heightmap generation methods
	static void generateHeightMap( float *map, int size, int seedValue, float heightScale, float h );
	static void generateHeightMapNorth( float *map, int size, int seedValue, float heightScale, float h, float *previousMap );
	static void generateHeightMapSouth( float *map, int size, int seedValue, float heightScale, float h, float *previousMap );
	static void generateHeightMapWest( float *map, int size, int seedValue, float heightScale, float h, float *previousMap );
	static void generateHeightMapEast( float *map, int size, int seedValue, float heightScale, float h, float *previousMap );
};

#endif