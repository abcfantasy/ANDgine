#include "SceneManager.h"
#include "SDL.h"

#include "SoundManager.h"
#include "InputManager.h"
#include "ResourceManager.h"

#include "Model.h"
#include "HeightMapModel.h"
#include "GameObject.h"

#include "GameObjectNode.h"
#include "PlayerNode.h"

#include "CellManager.h"

SceneManager* SceneManager::instance() {
	static SceneManager sm;
	return &sm;
};

void SceneManager::initializeScene(char *input_xml, char *cell) {
	CellManager::instance()->Initialize( input_xml );
	playerNode_ = CellManager::instance()->LoadPlayer();
	sceneGraph_.addObject( playerNode_ );
	CellManager::instance()->LoadCell( 0, 0, &sceneGraph_ );

	InputManager::instance()->addKeyDownEvent( &SceneManager::keyDown );
	InputManager::instance()->addKeyUpEvent( &SceneManager::keyUp );
};

void SceneManager::renderScene() {
	int timeThisFrame = SDL_GetTicks();
	float deltaT = float(timeThisFrame - timeLastFrame_) / 1000.0f;
	this->timeLastFrame_ = timeThisFrame;
	if( deltaT > 1.0f/30.0f )
		deltaT = 1.0f/30.0f;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	this->sceneGraph_.render( deltaT );

	CellManager::instance()->CheckPlayerLocation( playerNode_, &sceneGraph_ );
	
	// check heightmap
	if( CellManager::instance()->ActiveTerrainNode() != NULL ) {
		float pos[4];
		GameObjectNode *terrainNode = CellManager::instance()->ActiveTerrainNode();
		terrainNode->worldToModel( playerNode_->getPosition(), pos );
		float playerY = playerNode_->getPosition()[1];
		float deltaY = ( terrainNode->getPosition()[1] + ((HeightMapModel*)terrainNode->getGameObject()->getModel())->getHeight( pos[0], pos[2] ) ) - playerY;
		if( deltaY != 0 ) playerNode_->translate( 0.0f, deltaY, 0.0f );
	}

	// change ambient sound
	SoundManager::instance()->ChangeAmbientEffect( playerNode_->getPosition(), playerNode_->getRotation()[1] );

	SDL_GL_SwapBuffers();
};

void SceneManager::keyDown( SDLKey key, SDLMod mod )
{
	// one option is to check the key here and handle result
	// another option is to poll the getKeys of InputManager in the game loop and handle accordingly
	switch( key ) {
	case SDLK_UP:		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, -15.0f );	break;
	case SDLK_DOWN:		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, 15.0f );	break;
	case SDLK_RIGHT:	SceneManager::instance()->getPlayerNode()->addVelocity( 15.0f, 0.0f, 0.0f );	break;
	case SDLK_LEFT:		SceneManager::instance()->getPlayerNode()->addVelocity( -15.0f, 0.0f, 0.0f );	break;
	case SDLK_SPACE:	SoundManager::instance()->PlaySound( "explosion" ); break;
	case SDLK_ESCAPE:	SDL_Quit(); exit( 1 ); break;
	}
}

void SceneManager::keyUp( SDLKey key, SDLMod mod )
{
	switch( key ) {
	case SDLK_UP:		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, 15.0f );	break;
	case SDLK_DOWN:		SceneManager::instance()->getPlayerNode()->addVelocity( 0.0f, 0.0f, -15.0f );	break;
	case SDLK_RIGHT:	SceneManager::instance()->getPlayerNode()->addVelocity( -15.0f, 0.0f, 0.0f );	break;
	case SDLK_LEFT:		SceneManager::instance()->getPlayerNode()->addVelocity( 15.0f, 0.0f, 0.0f );	break;
	}
}