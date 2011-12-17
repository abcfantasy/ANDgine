#include "SceneManager.h"
#include "SDL.h"

#include "SoundManager.h"
#include "CollisionManager.h"
#include "InputManager.h"
#include "ResourceManager.h"

#include "Model.h"
#include "HeightMapModel.h"
#include "GameObject.h"

#include "GameObjectNode.h"
#include "PlayerNode.h"

#include "rapidxml.hpp"
#include "rapidxml_utils.hpp"
using namespace rapidxml;

SceneManager* SceneManager::instance() {
	static SceneManager sm;
	return &sm;
};

GameObjectNode *terrainNode = NULL;

void SceneManager::initializeScene(char *input_xml, char *cell) {
	xml_document<> xmlDoc;
	float x, y, z;
	
	file<> file(input_xml);
	xmlDoc.parse<0>(file.data());
	xml_node<>* cur_node = xmlDoc.first_node()->first_node(cell)->first_node();
	xml_node<>* subNode;
	GameObjectNode *newNode;

	while( cur_node ) {
		if( cur_node && strcmp( cur_node->name(), "player") == 0) {
			Model *playerModel = ResourceManager::instance()->get<Model>( cur_node->first_node("location")->value() );
			newNode = playerNode_ = new PlayerNode( new GameObject( playerModel ) );
		} else if( cur_node && strcmp( cur_node->name(), "heightmap") == 0 ) {
			char *modelPath;

			subNode = cur_node->first_node( "location" );
			if( subNode ) modelPath = subNode->value();
			else modelPath = "";
				
			HeightMapModel *terrain = ResourceManager::instance()->get<HeightMapModel>( modelPath );
			newNode = terrainNode = new GameObjectNode( new GameObject( terrain ) );
			
			subNode = cur_node->first_node("scale");
			if( cur_node->first_node("scale") ) {
				float min = (float)atof( subNode->first_attribute("min")->value() );
				float max = (float)atof( subNode->first_attribute("max")->value() );
				terrain->rescale( min, max );
			}

			this->sceneGraph_.addObject( terrainNode );
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
		this->sceneGraph_.addObject( newNode );

		cur_node = cur_node->next_sibling();
	}

	InputManager::instance()->addKeyDownEvent( &SceneManager::keyDown );
	InputManager::instance()->addKeyUpEvent( &SceneManager::keyUp );
};

void SceneManager::renderScene() {
	int timeThisFrame = SDL_GetTicks();
	float deltaT = float(timeThisFrame - timeLastFrame_);
	this->timeLastFrame_ = timeThisFrame;
	if( deltaT > 1000.0f/30.0f )
		deltaT = 1000.0f/30.0f;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	this->sceneGraph_.render( deltaT );

	// check heightmap
	float pos[4];
	if( terrainNode != NULL ) {
		terrainNode->worldToModel( playerNode_->getPosition(), pos );
		float playerY = playerNode_->getPosition()[1];
		float deltaY = ( terrainNode->getPosition()[1] + ((HeightMapModel*)terrainNode->getGameObject()->getModel())->getHeight( pos[0], pos[2] ) ) - playerY;
		if( deltaY != 0 ) playerNode_->translate( 0.0f, deltaY, 0.0f );
	}

	// change ambient sound
	SoundManager::instance()->ChangeAmbientEffect( playerNode_->getPosition(), playerNode_->getRotation()[1] );

	// TESTING COLLISION
	/*
	if ( playerNode_ != NULL && pyramid2Node != NULL )
	{
	if ( CollisionManager::instance()->GJKCollide( playerNode_->getBoundingBox(), pyramid2Node->getBoundingBox() ) )
	{
		// do something to show that two pyramids made BOOM
		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 1.0f, 0.0f );
	}
	}*/

	SDL_GL_SwapBuffers();
};

void SceneManager::keyDown( SDLKey key, SDLMod mod )
{
	// one option is to check the key here and handle result
	// another option is to poll the getKeys of InputManager in the game loop and handle accordingly
	switch( key ) {
	case SDLK_UP:		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, -3.0f );	break;
	case SDLK_DOWN:		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, 3.0f );		break;
	case SDLK_RIGHT:	SceneManager::instance()->getPlayerNode()->addVelocity( 3.0f, 0.0f, 0.0f );		break;
	case SDLK_LEFT:		SceneManager::instance()->getPlayerNode()->addVelocity( -3.0f, 0.0f, 0.0f );	break;
	case SDLK_SPACE:	SoundManager::instance()->PlaySound( "explosion" ); break;
	case SDLK_ESCAPE:	SDL_Quit(); exit( 1 ); break;
	}
}

void SceneManager::keyUp( SDLKey key, SDLMod mod )
{
	switch( key ) {
	case SDLK_UP:		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, 3.0f );		break;
	case SDLK_DOWN:		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, -3.0f );	break;
	case SDLK_RIGHT:	SceneManager::instance()->getPlayerNode()->addVelocity( -3.0f, 0.0f, 0.0f );	break;
	case SDLK_LEFT:		SceneManager::instance()->getPlayerNode()->addVelocity( 3.0f, 0.0f, 0.0f );		break;
	}
}