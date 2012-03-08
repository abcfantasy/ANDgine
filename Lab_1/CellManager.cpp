#include "CellManager.h"
#include "Model.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "HeightMapModel.h"
#include "HeightMapGen.h"

using namespace rapidxml;

CellManager* CellManager::instance() {
	static CellManager cm;
	return &cm;
}

CellManager::~CellManager() {
	delete xmlFile_;
}

void CellManager::Initialize( char *input_xml ) {
	// parse XML data
	this->xml_file = input_xml;
	xmlFile_ = new rapidxml::file<>( input_xml );
	xmlDoc.parse<0>(xmlFile_->data());
}

PlayerNode* CellManager::LoadPlayer() {
	xml_node<>* subNode;
	xml_node<>* cur_node = xmlDoc.first_node()->first_node("player")->first_node();
	// load player model
	Model *playerModel = ResourceManager::instance()->get<Model>( cur_node->first_node("location")->value() );
	PlayerNode *player = new PlayerNode( new GameObject( playerModel ) );
	// load player texture if available
	subNode = cur_node->first_node("texture");
	if( subNode ) player->getGameObject()->getModel()->setTexture( subNode->value() );

	float x, y, z;
	subNode = cur_node->first_node("translation");
	if( subNode ) {
		x = (float)atof( subNode->first_node("x")->value() );
		y = (float)atof( subNode->first_node("y")->value() );
		z = (float)atof( subNode->first_node("z")->value() );
		player->translate(x,y,z);
	}

	return player;
}

void CellManager::LoadCell( int cellX, int cellZ, ContainerNode *sceneGraph ) {
	ContainerNode *cellNode = new ContainerNode();
	SceneNode *newNode;
	char cellName[25];

	this->cellX = cellX;
	this->cellZ = cellZ;
	sceneGraph->addObject( cellNode );

	sprintf_s( cellName, 25, "cell%dx%d", cellX, cellZ );
	xml_node<>* cur_node = xmlDoc.first_node()->first_node(cellName);
	
	if( cur_node == NULL ) {
		HeightMapModel *terrain = ResourceManager::instance()->get<HeightMapModel>( "" );
		terrain->rescale( 0.0f, 3.0f );
		terrain->setTexture( "Textures\\road.jpg" );
		newNode = this->activeTerrainNode = new GameObjectNode( new GameObject( terrain ) );
		newNode->setPosition( cellX * 256.0f, 0.0f, cellZ * 256.0f );
		cellNode->addObject( newNode );
		this->cells.insert( std::pair< std::pair<int, int>, GameObjectNode*>( std::pair<int, int>( cellX, cellZ ), this->activeTerrainNode ) );	// keep track of all added terrains
		return;
	}

	cur_node = cur_node->first_node();
	while( cur_node ) {
		processNode( cur_node, &newNode );
		cellNode->addObject( newNode );
		cur_node = cur_node->next_sibling();
	}
}

void CellManager::processNode( xml_node<> *cur_node, SceneNode **newNode ) {
	float x, y, z;
	xml_node<>* subNode;
	
	if( cur_node && strcmp( cur_node->name(), "heightmap") == 0 ) {
		char *modelPath;

		subNode = cur_node->first_node( "location" );
		if( subNode ) modelPath = subNode->value();
		else modelPath = "";
				
		HeightMapModel *terrain = ResourceManager::instance()->get<HeightMapModel>( modelPath );
		(*newNode) = this->activeTerrainNode = new GameObjectNode( new GameObject( terrain ) );
			
		subNode = cur_node->first_node("scale");
		if( cur_node->first_node("scale") ) {
			float min = (float)atof( subNode->first_attribute("min")->value() );
			float max = (float)atof( subNode->first_attribute("max")->value() );
			terrain->rescale( min, max );
		}

		this->cells.insert( std::pair< std::pair<int, int>, GameObjectNode*>( std::pair<int, int>( cellX, cellZ ), this->activeTerrainNode ) );	// keep track of all added terrains

	} else if( cur_node && strcmp( cur_node->name(), "model") == 0 ) {
		Model *newModel = ResourceManager::instance()->get<Model>( cur_node->first_node("location")->value() );
		(*newNode) = new GameObjectNode( new GameObject( newModel ) );			
	}
		
	subNode = cur_node->first_node("texture");
	if( subNode ) ((GameObjectNode*)(*newNode))->getGameObject()->getModel()->setTexture( subNode->value() );

	subNode = cur_node->first_node( "velocity" );
	if( subNode ) {
		x = (float)atof( subNode->first_node("x")->value() );
		y = (float)atof( subNode->first_node("y")->value() );
		z = (float)atof( subNode->first_node("z")->value() );
		(*newNode)->setVelocity(x,y,z);
	}
	subNode = cur_node->first_node( "angleVelocity" );
	if( subNode ) {
		x = (float)atof( subNode->first_node("x")->value() );
		y = (float)atof( subNode->first_node("y")->value() );
		z = (float)atof( subNode->first_node("z")->value() );
		(*newNode)->setAngleVelocity(x,y,z);
	}
	subNode = cur_node->first_node("translation");
	if( subNode ) {
		x = (float)atof( subNode->first_node("x")->value() );
		y = (float)atof( subNode->first_node("y")->value() );
		z = (float)atof( subNode->first_node("z")->value() );
		(*newNode)->translate(x,y,z);
	}
	subNode = cur_node->first_node("rotation");
	if( subNode ) {
		x = (float)atof( subNode->first_node("x")->value() );
		y = (float)atof( subNode->first_node("y")->value() );
		z = (float)atof( subNode->first_node("z")->value() );
		(*newNode)->rotate(x,y,z);
	}
}

void CellManager::CheckPlayerLocation( PlayerNode *player, ContainerNode *sceneGraph ) {
	float playerX = player->getPosition()[0];
	float playerZ = player->getPosition()[2];

	int cellX = (int)( floor( ( playerX ) / 256.0f ) );
	int cellZ = (int)( floor( ( playerZ ) / 256.0f ) );

	if( cellX == this->cellX && cellZ == this->cellZ ) return;
	/*if( cellX == this->cellX + 1 )	HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_WEST;
	else if( cellX == this->cellX - 1 )	HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_EAST;
	if( cellZ == this->cellZ + 1 )	HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_SOUTH;
	else if( cellZ == this->cellZ - 1 )	HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_NORTH;*/

	if( cellX == this->cellX + 1 )	HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_NORTH;
	else if( cellX == this->cellX - 1 )	HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_SOUTH;
	if( cellZ == this->cellZ + 1 )	HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_WEST;
	else if( cellZ == this->cellZ - 1 )	HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_EAST;

	HeightMapModel::PREVIOUS_MAP = new float[ this->activeTerrainNode->getGameObject()->getModel()->getVertices()->size() ];
	for( unsigned int i = 0; i < this->activeTerrainNode->getGameObject()->getModel()->getVertices()->size(); ++i ) {
		HeightMapModel::PREVIOUS_MAP[i] = (*this->activeTerrainNode->getGameObject()->getModel()->getVertices())[i].getY();
	}

	if( this->cells.find( std::pair<int, int>( cellX, cellZ ) ) == this->cells.end() ) {
		this->LoadCell( cellX, cellZ, sceneGraph );
	} else {
		this->activeTerrainNode = this->cells[ std::pair<int, int>( cellX, cellZ ) ];
	}
}