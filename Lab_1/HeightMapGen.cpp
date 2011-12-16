#include "HeightMapGen.h"
#include <stdlib.h>
#include <math.h>

/*
 * randNum - Return a random floating point number such that
 *      (min <= return-value <= max)
 * 32,767 values are possible for any given range.
 */
float HeightMapGen::randnum (float min, float max)
{
	int r;
    float	x;
    
	r = rand();
    x = (float)(r & 0x7fff) /
		(float)0x7fff;
    return (x * (max - min) + min);
} 


/*
 * avgDiamondVals - Given the i,j location as the center of a diamond,
 * average the data values at the four corners of the diamond and
 * return it. "Stride" represents the distance from the diamond center
 * to a diamond corner.
 *
 * Called by fill2DFractArray.
 */
float HeightMapGen::avgDiamondVals (int i, int j, int stride,
			     int size, int subSize, float *map)
{
    /* In this diagram, our input stride is 1, the i,j location is
       indicated by "X", and the four value we want to average are
       "*"s:
           .   *   .

           *   X   *

           .   *   .
       */

    /* In order to support tiled surfaces which meet seamless at the
       edges (that is, they "wrap"), We need to be careful how we
       calculate averages when the i,j diamond center lies on an edge
       of the array. The first four 'if' clauses handle these
       cases. The final 'else' clause handles the general case (in
       which i,j is not on an edge).
     */
    if (i == 0)
	return ((float) (map[(i*size) + j-stride] +
			 map[(i*size) + j+stride] +
			 map[((subSize-stride)*size) + j] +
			 map[((i+stride)*size) + j]) * 0.25f);
    else if (i == size-1)
	return ((float) (map[(i*size) + j-stride] +
			 map[(i*size) + j+stride] +
			 map[((i-stride)*size) + j] +
			 map[((0+stride)*size) + j]) * 0.25f);
    else if (j == 0)
	return ((float) (map[((i-stride)*size) + j] +
			 map[((i+stride)*size) + j] +
			 map[(i*size) + j+stride] +
			 map[(i*size) + subSize-stride]) * 0.25f);
    else if (j == size-1)
	return ((float) (map[((i-stride)*size) + j] +
			 map[((i+stride)*size) + j] +
			 map[(i*size) + j-stride] +
			 map[(i*size) + 0+stride]) * 0.25f);
    else
	return ((float) (map[((i-stride)*size) + j] +
			 map[((i+stride)*size) + j] +
			 map[(i*size) + j-stride] +
			 map[(i*size) + j+stride]) * 0.25f);
}

/*
 * avgSquareVals - Given the i,j location as the center of a square,
 * average the data values at the four corners of the square and return
 * it. "Stride" represents half the length of one side of the square.
 *
 * Called by fill2DFractArray.
 */
float HeightMapGen::avgSquareVals (int i, int j, int stride, int size, float *map)
{
    /* In this diagram, our input stride is 1, the i,j location is
       indicated by "*", and the four value we want to average are
       "X"s:
           X   .   X

           .   *   .

           X   .   X
       */
    return ((float) (map[((i-stride)*size) + j-stride] +
		     map[((i-stride)*size) + j+stride] +
		     map[((i+stride)*size) + j-stride] +
		     map[((i+stride)*size) + j+stride]) * 0.25f);
}

// generate a 2d array for heightmap
// WARNING: size of map must be power of 2
// High h => smooth map
// Low h => rough map
void HeightMapGen::fill2DFractArray (float *map, int size, int seedValue, float heightScale, float h)
{
	int stride;
	int subSize;
	float ratio, scale;

	/* subSize is the dimension of the array in terms of connected line
       segments, while size is the dimension in terms of number of
       vertices. */
    subSize = size;
    size++;

	/* initialize random number generator */
    srand(seedValue);

	/* Set up our roughness constants.
	   Random numbers are always generated in the range 0.0 to 1.0.
	   'scale' is multiplied by the randum number.
	   'ratio' is multiplied by 'scale' after each iteration
	   to effectively reduce the randum number range.
	   */
	ratio = pow(2.0f,-h);
	scale = heightScale * ratio;

	/* Seed the first four values. For example, in a 4x4 array, we
       would initialize the data points indicated by '*':

           *   .   .   .   *

           .   .   .   .   .

           .   .   .   .   .

           .   .   .   .   .

           *   .   .   .   *

       In terms of the "diamond-square" algorithm, this gives us
       "squares".

       We want the four corners of the array to have the same
       point. This will allow us to tile the arrays next to each other
       such that they join seemlessly. */
    stride = subSize / 2;
    map[0] =
      map[(subSize*size)] =
        map[(subSize*size)+subSize] =
          map[subSize] = 0.0f;

	/* Now we add ever-increasing detail based on the "diamond" seeded
       values. We loop over stride, which gets cut in half at the
       bottom of the loop. Since it's an int, eventually division by 2
       will produce a zero result, terminating the loop. */
    while (stride) {
		/* Take the existing "square" data and produce "diamond"
		   data. On the first pass through with a 4x4 matrix, the
		   existing data is shown as "X"s, and we need to generate the
	       "*" now:

               X   .   .   .   X

               .   .   .   .   .

               .   .   *   .   .

               .   .   .   .   .

               X   .   .   .   X

	      It doesn't look like diamonds. What it actually is, for the
	      first pass, is the corners of four diamonds meeting at the
	      center of the array. */
		for (int i = stride; i < subSize; i += stride) {
			for (int j = stride; j < subSize; j += stride) {
				map[(i * size) + j] =
					scale * HeightMapGen::randnum(-0.5f, 0.5f) +
					HeightMapGen::avgSquareVals(i, j, stride, size, map);
				j += stride;
			}
			i += stride;
		}

		/* Take the existing "diamond" data and make it into
	       "squares". Back to our 4X4 example: The first time we
	       encounter this code, the existing values are represented by
	       "X"s, and the values we want to generate here are "*"s:

               X   .   *   .   X

               .   .   .   .   .

               *   .   X   .   *

               .   .   .   .   .

               X   .   *   .   X

	       i and j represent our (x,y) position in the array. The
	       first value we want to generate is at (i=2,j=0), and we use
	       "oddline" and "stride" to increment j to the desired value.
	       */
		int oddline = 0;
		for ( int i = 0; i < subSize; i += stride ) {
		    oddline = (oddline == 0);
			for ( int j = 0; j < subSize; j += stride ) {
				if ( (oddline) && !j ) j += stride;

				/* i and j are setup. Call avgDiamondVals with the
				   current position. It will return the average of the
				   surrounding diamond data points. */
				map[(i * size) + j] =
					scale * HeightMapGen::randnum(-0.5f, 0.5f) +
					HeightMapGen::avgDiamondVals(i, j, stride, size, subSize, map);

				/* To wrap edges seamlessly, copy edge values around
				   to other side of array */
				if (i==0)
					map[(subSize*size) + j] =
						map[(i * size) + j];
				if (j==0)
					map[(i*size) + subSize] =
						map[(i * size) + j];

				j+=stride;
			}
		}

		/* reduce random number range. */
		scale *= ratio;
		stride >>= 1;
	}
}