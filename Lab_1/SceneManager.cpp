#include "SceneManager.h"
#include "SDL.h"

#include "InputManager.h"
#include "ResourceManager.h"

#include "Model.h"
#include "HeightMapModel.h"
#include "GameObject.h"

#include "GameObjectNode.h"
#include "PlayerNode.h"

#include "CollisionManager.h"
#include "Math.h"

SceneManager* SceneManager::instance() {
	static SceneManager sm;
	return &sm;
};

GameObjectNode *terrainNode = NULL;

void SceneManager::initializeScene() {
	Model *playerModel = ResourceManager::instance()->get<Model>( "Models\\smiley.obj" );
	playerNode_ = new PlayerNode( new GameObject( playerModel ) );
	playerNode_->translate( 25.0f, 0.0f, 25.0f );
	this->sceneGraph_.addObject( playerNode_ );
	
	Model *planet = ResourceManager::instance()->get<Model>( "Models\\planet3f.obj" );
	GameObjectNode *planetNode = new GameObjectNode( new GameObject( planet ) );
	planetNode->setAngleVelocity(0.0f, 20.0f, 0.0f );
	this->sceneGraph_.addObject( planetNode );
	
	HeightMapModel *terrain = ResourceManager::instance()->get<HeightMapModel>( "Heightmaps\\hildebrand.tga" );
	terrain->setTexture( "Textures\\dirt.tga" );
	terrain->rescale( 0.0f, 15.0f );
	terrainNode = new GameObjectNode( new GameObject( terrain ) );
	//terrainNode->translate( 25.0f, -15.0f, 25.0f );

	this->sceneGraph_.addObject( terrainNode );
	//this->sceneGraph_.translate( 0.0f, 0.0f, -3.0f );

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
	//Math::subtract( playerNode_->getPosition(), terrainNode->getPosition(), pos );
	//playerNode_->translate( playerNode_->getPosition()[0], (*(HeightMapModel*)terrainNode->getGameObject()->getModel())(pos[0], pos[2])->getY() - playerNode_->getPosition()[1], playerNode_->getPosition()[2], deltaT );
	//playerNode_->translate( playerNode_->getPosition()[0], (*(HeightMapModel*)terrainNode->getGameObject()->getModel())(playerNode_->getPosition()[0], playerNode_->getPosition()[2])->getY() - playerNode_->getPosition()[1], playerNode_->getPosition()[2], deltaT );
	if( terrainNode == NULL ) return;
//	terrainNode->worldToModel( playerNode_->getPosition(), pos );
//	playerNode_->setY( (*((HeightMapModel*)terrainNode->getGameObject()->getModel()))( pos[2], pos[0] )->getY() );
	playerNode_->setY( terrainNode->getPosition()[1] + (*(HeightMapModel*)terrainNode->getGameObject()->getModel())(playerNode_->getPosition()[0], playerNode_->getPosition()[2])->getY() + 0.5f );
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
	if ( key == SDLK_UP )	{ SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, -3.0f ); }
	if ( key == SDLK_DOWN )	{ SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, 3.0f ); }
	if ( key == SDLK_RIGHT ){ SceneManager::instance()->getPlayerNode()->addVelocity( 3.0f, 0.0f, 0.0f ); }
	if ( key == SDLK_LEFT )	{ SceneManager::instance()->getPlayerNode()->addVelocity( -3.0f, 0.0f, 0.0f ); }
}

void SceneManager::keyUp( SDLKey key, SDLMod mod )
{
	if ( key == SDLK_UP )	{ SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, 3.0f ); }
	if ( key == SDLK_DOWN )	{ SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, -3.0f ); }
	if ( key == SDLK_RIGHT ){ SceneManager::instance()->getPlayerNode()->addVelocity( -3.0f, 0.0f, 0.0f ); }
	if ( key == SDLK_LEFT )	{ SceneManager::instance()->getPlayerNode()->addVelocity( 3.0f, 0.0f, 0.0f ); }
}