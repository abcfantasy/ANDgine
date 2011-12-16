#ifndef _HEIGHTMAPGEN_H_
#define _HEIGHTMAPGEN_H_

class HeightMapGen
{
private:
	float randnum (float min, float max);
	float avgDiamondVals (int i, int j, int stride, int size, int subSize, float *map);
	float avgSquareVals (int i, int j, int stride, int size, float *map);

public:
	// singleton
	static HeightMapGen* instance();

	void fill2DFractArray (float *map, int size, int seedValue, float heightScale, float h);

};

#endif