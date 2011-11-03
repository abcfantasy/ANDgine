#include "SceneManager.h"
#include "SDL.h"
#include "SceneNode.h"
#include "GameObject.h"
#include "GameObjectNode.h"
#include "Model.h"
#include "HeightMapModel.h"
#include "ResourceManager.h"
#include "TextureResource.h"

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
	
	GameObjectNode *pyramidNode = new GameObjectNode( GameObject( pyramid ) );
	pyramidNode->setVelocity( 0.0f, 0.0f, 0.0f );
	pyramidNode->setAngleVelocity( 0.0f, 40.0f, 0.0f );
	
	HeightMapModel *terrain = new HeightMapModel( "Heightmaps\\heightmap_smooth.tga", 0.0f, 15.0f );
	terrain->setTexture( "Textures\\dirt.tga" );
	
	GameObjectNode *terrainNode = new GameObjectNode( GameObject( (Model*)terrain ) );
	float pos_[3] = { 25.0f, -15.0f, 25.0f };
	terrainNode->translate( pos_ );
	terrainNode->setAngleVelocity( 0.0f, 10.0f, 0.0f );

	this->sceneGraph_.addObject( pyramidNode );
	this->sceneGraph_.addObject( terrainNode );
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

	SDL_GL_SwapBuffers();
};