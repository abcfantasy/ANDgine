#include "SceneManager.h"
#include "SDL.h"
#include "SceneNode.h"
#include "GameObject.h"
#include "GameObjectNode.h"
#include "Model.h"
#include "ResourceManager.h"
#include "TextureResource.h"

SceneManager* SceneManager::instance() {
	static SceneManager sm;
	return &sm;
};

void SceneManager::initializeScene() {
	// TODO: Make this part a bit more syntax-friendly
	unsigned int handle = ResourceManager::instance()->addResource<TextureResource>( "stone54.jpg" );
	TextureResource *texture = (TextureResource*)((*ResourceManager::instance())[handle]);
	Model *pyramid = new Model( texture->getTexture()->TextureID );

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
	
	GameObjectNode *pyramidNode = new GameObjectNode( GameObject( pyramid ) );
	pyramidNode->setVelocity( 0.0f, 0.0f, 0.0f );
	pyramidNode->setAngleVelocity( 0.0f, 40.0f, 0.0f );

	this->sceneGraph_.addObject( pyramidNode );
	float pos[3] = { 0.0f, 0.0f, -3.0f };
	this->sceneGraph_.translate( pos );
};

void SceneManager::renderScene() {
	int timeThisFrame = SDL_GetTicks();
	float deltaT = float(timeThisFrame - timeLastFrame_);
	this->timeLastFrame_ = timeThisFrame;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	glLoadIdentity();

	this->sceneGraph_.render( deltaT );

	SDL_GL_SwapBuffers( );
};