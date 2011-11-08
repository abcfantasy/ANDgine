#include "SceneManager.h"
#include "SDL.h"

#include "InputManager.h"
#include "ResourceManager.h"

#include "Model.h"
#include "HeightMapModel.h"
#include "GameObject.h"

#include "GameObjectNode.h"
#include "PlayerNode.h"

SceneManager* SceneManager::instance() {
	static SceneManager sm;
	return &sm;
};

void SceneManager::initializeScene() {
	int playerModelHandle = ResourceManager::instance()->addResource<Model>( "Models\\smiley.obj" );
	Model *playerModel = (Model*)ResourceManager::instance()->getElement( playerModelHandle );
	playerModel->setTexture( "Textures\\smile.jpg" );

	playerNode_ = new PlayerNode( GameObject( playerModel ) );
	this->sceneGraph_.addObject( playerNode_ );
	
	int planetHandle = ResourceManager::instance()->addResource<Model>( "Models\\planet3f.obj" );
	Model *planet = (Model*)ResourceManager::instance()->getElement( planetHandle );
	planet->setTexture( "Textures\\world.jpg" );
	GameObjectNode *planetNode = new GameObjectNode( GameObject( planet ) );
	planetNode->setAngleVelocity(0.0f, 20.0f, 0.0f );
	this->sceneGraph_.addObject( planetNode );
	
	int terrainModelHandle = ResourceManager::instance()->addResource<HeightMapModel>( "Heightmaps\\hildebrand.tga" );
	HeightMapModel *terrainModel = (HeightMapModel*)ResourceManager::instance()->getElement( terrainModelHandle );
	terrainModel->setTexture( "Textures\\dirt.tga" );
	terrainModel->rescale( 0.0f, 15.0f );
	GameObjectNode *terrainNode = new GameObjectNode( GameObject( terrainModel ) );
	terrainNode->translate( 25.0f, -15.0f, 25.0f );

	this->sceneGraph_.addObject( terrainNode );
	this->sceneGraph_.translate( 0.0f, 0.0f, -3.0f );

	InputManager::instance()->addKeyDownEvent( &SceneManager::keyDown );
	InputManager::instance()->addKeyUpEvent( &SceneManager::keyUp );
};

void SceneManager::renderScene() {
	int timeThisFrame = SDL_GetTicks();
	float deltaT = float(timeThisFrame - timeLastFrame_);
	this->timeLastFrame_ = timeThisFrame;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	this->sceneGraph_.render( deltaT );

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