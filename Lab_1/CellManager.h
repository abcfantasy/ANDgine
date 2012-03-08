#ifndef _CELLMANAGER_H_
#define _CELLMANAGER_H_

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
#include "PlayerNode.h"
#include "ContainerNode.h"
#include "GameObjectNode.h"
#include <map>

using namespace rapidxml;

// Singleton class that handles cell loading from the XML file
// The XML file is read once and kept in memory for the whole session
class CellManager
{
private:
	char *xml_file;
	file<> *xmlFile_;
	xml_document<> xmlDoc;
	std::map<std::pair<int, int>, GameObjectNode*> cells;
	GameObjectNode *activeTerrainNode;
	int cellX, cellZ;

	void processNode( xml_node<> *cur_node, SceneNode **newNode );

public:
	// It's a singleton object
	static CellManager* instance();
	// Releasing the XML file
	~CellManager();
	
	// Reading the XML file and launching the first cell
	void Initialize( char *input_xml );
	// Loading the player (his data is separate from the cells)
	PlayerNode* LoadPlayer();
	// Loads cell (x,y) and places it in the scenegraph
	void LoadCell( int cellX, int cellY, ContainerNode *sceneGraph );
	// Checks to see if the player has moved to a different cell
	void CheckPlayerLocation( PlayerNode *player, ContainerNode *sceneGraph );

	// Which heightmap are we currently on?
	inline GameObjectNode* ActiveTerrainNode() { return this->activeTerrainNode; };
};

#endif