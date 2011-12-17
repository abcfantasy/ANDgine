#ifndef _HEIGHTMAPGEN_H_
#define _HEIGHTMAPGEN_H_

class HeightMapGen
{
private:
	static float randnum (float min, float max);
	static float avgDiamondVals (int i, int j, int stride, int size, int subSize, float *map);
	static float avgSquareVals (int i, int j, int stride, int size, float *map);

public:
	static void fill2DFractArray (float *map, int size, int seedValue, float heightScale, float h);

};

#endif