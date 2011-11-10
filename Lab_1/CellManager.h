#ifndef _CELLMANAGER_H_
#define _CELLMANAGER_H_

class CellManager
{

public:
	static CellManager* instance();

	int LoadCell( char* fileName, int cellX, int cellY );
};

#endif