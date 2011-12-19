#include "CellManager.h"
#include "Model.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "HeightMapModel.h"

using namespace rapidxml;

CellManager* CellManager::instance() {
	static CellManager cm;
	return &cm;
}

void CellManager::Initialize( char *input_xml ) {
	// parse XML data

	this->xml_file = input_xml;
	cellX = cellY = 1;
}

PlayerNode* CellManager::LoadPlayer() {
	
	file<> file(xml_file);
	xmlDoc.parse<0>(file.data());
	xml_node<>* subNode;
	xml_node<>* cur_node = xmlDoc.first_node()->first_node("player")->first_node();
	// load player model
	Model *playerModel = ResourceManager::instance()->get<Model>( cur_node->first_node("location")->value() );
	PlayerNode *player = new PlayerNode( new GameObject( playerModel ) );
	// load player texture if available
	subNode = cur_node->first_node("texture");
	if( subNode ) player->getGameObject()->getModel()->setTexture( subNode->value() );

	return player;
}

void CellManager::LoadCell( char *cell, ContainerNode *SceneGraph ) {
	
	file<> file(xml_file);
	xmlDoc.parse<0>(file.data());

	float x, y, z;
	xml_node<>* subNode;
	GameObjectNode *newNode;

	xml_node<>* cur_node = xmlDoc.first_node()->first_node(cell)->first_node();
	while( cur_node ) {
		if( cur_node && strcmp( cur_node->name(), "heightmap") == 0 ) {
			char *modelPath;

			subNode = cur_node->first_node( "location" );
			if( subNode ) modelPath = subNode->value();
			else modelPath = "";
				
			HeightMapModel *terrain = ResourceManager::instance()->get<HeightMapModel>( modelPath );
			newNode = this->activeTerrainNode = new GameObjectNode( new GameObject( terrain ) );
			
			subNode = cur_node->first_node("scale");
			if( cur_node->first_node("scale") ) {
				float min = (float)atof( subNode->first_attribute("min")->value() );
				float max = (float)atof( subNode->first_attribute("max")->value() );
				terrain->rescale( min, max );
			}

			SceneGraph->addObject( this->activeTerrainNode );
			this->cells.insert( std::pair<char*, GameObjectNode*>( cell, this->activeTerrainNode ) );	// keep track of all added terrains

		} else if( cur_node && strcmp( cur_node->name(), "model") == 0 ) {
			Model *newModel = ResourceManager::instance()->get<Model>( cur_node->first_node("location")->value() );
			newNode = new GameObjectNode( new GameObject( newModel ) );			
		}
		
		subNode = cur_node->first_node("texture");
		if( subNode ) newNode->getGameObject()->getModel()->setTexture( subNode->value() );

		subNode = cur_node->first_node( "velocity" );
		if( subNode ) {
			x = (float)atof( subNode->first_node("x")->value() );
			y = (float)atof( subNode->first_node("y")->value() );
			z = (float)atof( subNode->first_node("z")->value() );
			newNode->setVelocity(x,y,z);
		}
		subNode = cur_node->first_node( "angleVelocity" );
		if( subNode ) {
			x = (float)atof( subNode->first_node("x")->value() );
			y = (float)atof( subNode->first_node("y")->value() );
			z = (float)atof( subNode->first_node("z")->value() );
			newNode->setAngleVelocity(x,y,z);
		}
		subNode = cur_node->first_node("translation");
		if( subNode ) {
			x = (float)atof( subNode->first_node("x")->value() );
			y = (float)atof( subNode->first_node("y")->value() );
			z = (float)atof( subNode->first_node("z")->value() );
			newNode->translate(x,y,z);
		}
		subNode = cur_node->first_node("rotation");
		if( subNode ) {
			x = (float)atof( subNode->first_node("x")->value() );
			y = (float)atof( subNode->first_node("y")->value() );
			z = (float)atof( subNode->first_node("z")->value() );
			newNode->rotate(x,y,z);
		}
		SceneGraph->addObject( newNode );

		cur_node = cur_node->next_sibling();
	}
}

void CellManager::CheckPlayerLocation( PlayerNode *player, ContainerNode *SceneGraph )
{
	float playerX = player->getPosition()[0];
	float playerZ = player->getPosition()[3];

	if ( playerX < 128 ) {
		if ( this->cells.find("cell1x1") != this->cells.end() )
			this->activeTerrainNode = this->cells["cell1x1"];
		cellX = 1;
		cellY = 1;
	}
	else if( playerX > 128.0 && playerX < 384 ){
		if ( this->cells.find("cell2x1") != this->cells.end() )
			this->activeTerrainNode = this->cells["cell2x1"];
		else
			this->LoadCell( "cell2x1", SceneGraph );
		cellX = 2;
		cellY = 1;
	}
	else if( playerX > 384.0 && playerX < 640 ){
		if ( this->cells.find("cell3x1") != this->cells.end() )
			this->activeTerrainNode = this->cells["cell3x1"];
		else
			this->LoadCell( "cell3x1", SceneGraph );
		cellX = 3;
		cellY = 1;
	}
	else if( playerX > 640.0 ){
		if ( this->cells.find("cell4x1") != this->cells.end() )
			this->activeTerrainNode = this->cells["cell4x1"];
		else
			this->LoadCell( "cell4x1", SceneGraph );
		cellX = 4;
		cellY = 1;
	}
}