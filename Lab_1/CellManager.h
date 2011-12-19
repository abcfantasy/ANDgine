#ifndef _CELLMANAGER_H_
#define _CELLMANAGER_H_

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "PlayerNode.h"
#include "ContainerNode.h"
#include "GameObjectNode.h"
#include <map>

using namespace rapidxml;

class CellManager
{

private:
	char *xml_file;
	xml_document<> xmlDoc;
	std::map<char*, GameObjectNode*> cells;
	GameObjectNode *activeTerrainNode;
	int cellX, cellY;

public:
	// It's a singleton object
	static CellManager* instance();

	void Initialize( char *input_xml );
	PlayerNode* LoadPlayer();
	void LoadCell( char *cell, ContainerNode *SceneGraph );
	void CheckPlayerLocation( PlayerNode *player, ContainerNode *SceneGraph );

	inline GameObjectNode* ActiveTerrainNode() { return this->activeTerrainNode; };
};

#endif