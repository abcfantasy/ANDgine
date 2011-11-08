#include "SceneManager.h"
#include "SDL.h"

#include "InputManager.h"
#include "ResourceManager.h"

#include "Model.h"
#include "HeightMapModel.h"
#include "TextureResource.h"
#include "GameObject.h"

#include "SceneNode.h"
#include "GameObjectNode.h"
#include "PlayerNode.h"

SceneManager* SceneManager::instance() {
	static SceneManager sm;
	return &sm;
};

void SceneManager::initializeScene() {
	Model *pyramid = new Model();
	pyramid->setTexture( "Textures\\stone54.jpg" );

	// front face
	pyramid->addVertex( Vertex3f(		// bottom left (green)
		-0.5f, -0.5f, 0.5f,				// position
		0.0f, 1.0f, 0.0f,				// color
		0.0f, 0.0f, 1.0f,				// normal
		0.0f, 0.0f ) );					// texture
	pyramid->addVertex( Vertex3f( 
		0.5f, -0.5f, 0.5f, 
		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, 1.0f,
		1.0f, 0.0f ) );		// bottom right (blue)
	pyramid->addVertex( Vertex3f( 
		0.0f, 0.5f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, 1.0f,
		0.5f, 1.0f) );		// top (red)

	// right face
	pyramid->addVertex( Vertex3f( 
		0.5f, -0.5f, 0.5f, 
		0.0f, 0.0f, 1.0f, 
		1.0f, 0.0f, 0.0f,
		0.0f, 0.0f ) );		// bottom left (blue)
	pyramid->addVertex( Vertex3f(
		0.5f, -0.5f, -0.5f, 
		0.0f, 1.0f, 0.0f, 
		1.0f, 0.0f, 0.0f,
		1.0f, 0.0f ) );		// bottom right (green)
	pyramid->addVertex( Vertex3f( 
		0.0f, 0.5f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		1.0f, 1.0f, 0.0f,
		0.5f, 1.0f ) );		// top (red)

	// back face
	pyramid->addVertex( Vertex3f( 
		0.5f, -0.5f, -0.5f, 
		0.0f, 1.0f, 0.0f, 
		0.0f, 0.0f, -1.0f,
		0.0f, 0.0f ) );		// bottom left (green)
	pyramid->addVertex( Vertex3f( 
		-0.5f, -0.5f, -0.5f, 
		0.0f, 0.0f, 1.0f, 
		0.0f, 0.0f, -1.0f,
		1.0f, 0.0f) );	// bottom right (blue)
	pyramid->addVertex( Vertex3f( 
		0.0f, 0.5f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		0.0f, 1.0f, -1.0f,
		0.5f, 1.0f) );		// top (red)

	// left face
	pyramid->addVertex( Vertex3f( 
		-0.5f, -0.5f, -0.5f,
		0.0f, 0.0f, 1.0f, 
		-1.0f, 0.0f, 0.0f,
		0.0f, 0.0f ) );	// bottom left (blue)
	pyramid->addVertex( Vertex3f( 
		-0.5f, -0.5f, 0.5f, 
		0.0f, 1.0f, 0.0f, 
		-1.0f, 0.0f, 0.0f,
		1.0f, 0.0f) );		// bottom right (green)
	pyramid->addVertex( Vertex3f( 
		0.0f, 0.5f, 0.0f, 
		1.0f, 0.0f, 0.0f, 
		-1.0f, 1.0f, 0.0f,
		0.5f, 1.0f) );		// top (red)

	playerNode_ = new PlayerNode( GameObject( pyramid ) );

	this->sceneGraph_.addObject( playerNode_ );

	HeightMapModel *terrain = new HeightMapModel( "Heightmaps\\hildebrand.tga", 0.0f, 15.0f );
	terrain->setTexture( "Textures\\dirt.tga" );
	
	GameObjectNode *terrainNode = new GameObjectNode( GameObject( (Model*)terrain ) );
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