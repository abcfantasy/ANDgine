#include "SceneManager.h"
#include "SDL.h"
#include "SceneNode.h"
#include "ModelNode.h"
#include "VertexManager.h"

SceneManager::SceneManager() {
	this->frameIndexBuffer_ = new FrameIndexBuffer( 2048 );
	this->timeLastFrame_ = 0;
};

SceneManager::~SceneManager() {
	delete this->frameIndexBuffer_;
};

SceneManager* SceneManager::instance() {
	static SceneManager sm;
	return &sm;
};

void SceneManager::initializeScene() {
	/*GLuint pyramid_top = VertexManager::instance()->addVertex( Vertex4f(
		0.0f, 0.5f, 0.0f, 1.0f,
		0.0f, 0.5f, 0.0f,
		0.0, 1.0, 0.0 ) );
	GLuint pyramid_backleft = VertexManager::instance()->addVertex( Vertex4f(
		-0.5f,-0.5f, -0.5f, 1.0f,
		-0.5f,-0.5f, -0.5f,
		0.0, 0.0, 1.0 ) );
	GLuint pyramid_backright = VertexManager::instance()->addVertex( Vertex4f(
		0.5f, -0.5f, -0.5f, 1.0f,
		0.5f, -0.5f, -0.5f,
		0.0, 1.0, 0.0 ) );
	GLuint pyramid_frontleft = VertexManager::instance()->addVertex( Vertex4f(
		-0.5f, -0.5f, 0.5f, 1.0f,
		-0.5f, -0.5f, 0.5f,
		0.0, 1.0, 0.0 ) );
	GLuint pyramid_frontright = VertexManager::instance()->addVertex( Vertex4f(
		0.5f, -0.5f, 0.5f, 1.0f,
		0.5f, -0.5f, 0.5f,
		0.0, 0.0, 1.0 ) );*/

	GLuint pyramid_top = VertexManager::instance()->addVertex( Vertex4f(
		0.0f, 0.5f, 0.0f, 1.0f,
		1.0, 0.0, 0.0 ) );
	GLuint pyramid_backleft = VertexManager::instance()->addVertex( Vertex4f(
		-0.5f,-0.5f, -0.5f, 1.0f,
		0.0, 0.0, 1.0 ) );
	GLuint pyramid_backright = VertexManager::instance()->addVertex( Vertex4f(
		0.5f, -0.5f, -0.5f, 1.0f,
		0.0, 1.0, 0.0 ) );
	GLuint pyramid_frontleft = VertexManager::instance()->addVertex( Vertex4f(
		-0.5f, -0.5f, 0.5f, 1.0f,
		0.0, 1.0, 0.0 ) );
	GLuint pyramid_frontright = VertexManager::instance()->addVertex( Vertex4f(
		0.5f, -0.5f, 0.5f, 1.0f,
		0.0, 0.0, 1.0 ) );
	
	ModelNode *pyramid = new ModelNode();
	pyramid->addIndex( pyramid_top );
	pyramid->addIndex( pyramid_frontleft );
	pyramid->addIndex( pyramid_frontright );
	pyramid->normalizeFace();
	
	pyramid->addIndex( pyramid_top );
	pyramid->addIndex( pyramid_frontright );
	pyramid->addIndex( pyramid_backright );
	pyramid->normalizeFace();

	pyramid->addIndex( pyramid_top );
	pyramid->addIndex( pyramid_backright );
	pyramid->addIndex( pyramid_backleft );
	pyramid->normalizeFace();

	pyramid->addIndex( pyramid_top );
	pyramid->addIndex( pyramid_backleft );
	pyramid->addIndex( pyramid_frontleft );
	pyramid->normalizeFace();
	
	pyramid->addIndex( pyramid_frontleft );
	pyramid->addIndex( pyramid_backright );
	pyramid->addIndex( pyramid_backleft );
	pyramid->normalizeFace();

	pyramid->addIndex( pyramid_backright );
	pyramid->addIndex( pyramid_frontleft );
	pyramid->addIndex( pyramid_frontright );
	pyramid->normalizeFace();
	
	pyramid->setVelocity( 0.0f, 0.0f, 0.0f );
	pyramid->setAngleVelocity( 0.0f, -30.0f, 0.0f );

	GLuint square_topleft = VertexManager::instance()->addVertex( Vertex4f( 0.60f, -0.60f, 0.0f, 1.0f, 1.0, 0.0, 1.0 ) );
	GLuint square_topright = VertexManager::instance()->addVertex( Vertex4f( 0.80f,-0.60f, 0.0f, 1.0f, 1.0, 0.0, 1.0 ) );
	GLuint square_bottomleft = VertexManager::instance()->addVertex( Vertex4f( 0.60f, -0.80f, 0.0f, 1.0f, 1.0, 0.0, 1.0 ) );
	GLuint square_bottomright = VertexManager::instance()->addVertex( Vertex4f( 0.80f, -0.80f, 0.5f, 1.0f, 1.0, 0.0, 0.0 ) );

	ModelNode *square = new ModelNode();
	square->addIndex( square_topleft );
	square->addIndex( square_bottomleft );
	square->addIndex( square_topright );
	square->normalizeFace();
	
	square->addIndex( square_bottomleft );
	square->addIndex( square_bottomright );
	square->addIndex( square_topright );
	square->normalizeFace();

	ModelNode *cubeMoon = new ModelNode();

	GLuint cube_top_front_right = VertexManager::instance()->addVertex( Vertex4f( 0.1f, 0.1f, 0.1f, 1.0f, 1.0, 0.0, 0.0 ) );
	GLuint cube_top_front_left = VertexManager::instance()->addVertex( Vertex4f( -0.1f, 0.1f, 0.1f, 1.0f, 1.0, 0.0, 0.0 ) );
	GLuint cube_bottom_front_right = VertexManager::instance()->addVertex( Vertex4f( 0.1f, -0.1f, 0.1f, 1.0f, 1.0, 0.0, 0.0 ) );
	GLuint cube_bottom_front_left = VertexManager::instance()->addVertex( Vertex4f( -0.1f, -0.1f, 0.1f, 1.0f, 1.0, 1.0, 0.0 ) );

	GLuint cube_top_back_right = VertexManager::instance()->addVertex( Vertex4f( 0.1f, 0.1f, -0.1f, 1.0f, 1.0, 1.0, 0.0 ) );
	GLuint cube_top_back_left = VertexManager::instance()->addVertex( Vertex4f( -0.1f, 0.1f, -0.1f, 1.0f, 1.0, 1.0, 0.0 ) );
	GLuint cube_bottom_back_right = VertexManager::instance()->addVertex( Vertex4f( 0.1f, -0.1f, -0.1f, 1.0f, 1.0, 1.0, 0.0 ) );
	GLuint cube_bottom_back_left = VertexManager::instance()->addVertex( Vertex4f( -0.1f, -0.1f, -0.1f, 1.0f, 1.0, 1.0, 0.0 ) );

	//front
	cubeMoon->addIndex( cube_top_front_right );
	cubeMoon->addIndex( cube_top_front_left );
	cubeMoon->addIndex( cube_bottom_front_right );
	cubeMoon->normalizeFace();

	cubeMoon->addIndex( cube_bottom_front_right );
	cubeMoon->addIndex( cube_top_front_left );
	cubeMoon->addIndex( cube_bottom_front_left );
	cubeMoon->normalizeFace();

	//left
	cubeMoon->addIndex( cube_top_front_left );
	cubeMoon->addIndex( cube_top_back_left );
	cubeMoon->addIndex( cube_bottom_back_left );
	cubeMoon->normalizeFace();
	
	cubeMoon->addIndex( cube_bottom_back_left );
	cubeMoon->addIndex( cube_bottom_front_left );
	cubeMoon->addIndex( cube_top_front_left );
	cubeMoon->normalizeFace();

	//back
	cubeMoon->addIndex( cube_top_back_right );
	cubeMoon->addIndex( cube_top_back_left );
	cubeMoon->addIndex( cube_bottom_back_right );
	cubeMoon->normalizeFace();

	cubeMoon->addIndex( cube_bottom_back_right );
	cubeMoon->addIndex( cube_top_back_left );
	cubeMoon->addIndex( cube_bottom_back_left );
	cubeMoon->normalizeFace();
	
	//top
	cubeMoon->addIndex( cube_top_front_right );
	cubeMoon->addIndex( cube_top_back_right );
	cubeMoon->addIndex( cube_top_front_left );
	cubeMoon->normalizeFace();
	
	cubeMoon->addIndex( cube_top_front_left );
	cubeMoon->addIndex( cube_top_back_right );
	cubeMoon->addIndex( cube_top_back_left );
	cubeMoon->normalizeFace();

	//right
	cubeMoon->addIndex( cube_top_back_right );
	cubeMoon->addIndex( cube_top_front_right );
	cubeMoon->addIndex( cube_bottom_back_right );
	cubeMoon->normalizeFace();
	
	cubeMoon->addIndex( cube_bottom_back_right );
	cubeMoon->addIndex( cube_top_front_right );
	cubeMoon->addIndex( cube_bottom_front_right );
	cubeMoon->normalizeFace();
	
	//bottom
	cubeMoon->addIndex( cube_bottom_front_right );
	cubeMoon->addIndex( cube_bottom_back_right );
	cubeMoon->addIndex( cube_bottom_back_left );
	cubeMoon->normalizeFace();
	
	cubeMoon->addIndex( cube_bottom_back_left );
	cubeMoon->addIndex( cube_bottom_front_left );
	cubeMoon->addIndex( cube_bottom_front_right );
	cubeMoon->normalizeFace();

	cubeMoon->translate( 0.70f, 0.30f, 0.0f );
	cubeMoon->setAngleVelocity( 40.0f, 0.0f, 0.0f );
	
	ContainerNode *orbit = new ContainerNode();
	orbit->setVelocity( 0.0f, 0.0f, 0.0f );
	orbit->setAngleVelocity( 0.0f, 90.0f, 0.0f );

	//Connect shapes to Objects
	orbit->addObject( cubeMoon );

	this->sceneGraph_.addObject(orbit);
	this->sceneGraph_.addObject(square);
	this->sceneGraph_.addObject(pyramid);
};

void SceneManager::renderScene() {
	int timeThisFrame = SDL_GetTicks();
	float deltaT = float(timeThisFrame - timeLastFrame_);
	this->timeLastFrame_ = timeThisFrame;

	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	this->frameIndexBuffer_->reset();
	this->sceneGraph_.render( deltaT, this->frameIndexBuffer_ );
	VertexManager::instance()->render( this->frameIndexBuffer_ );

	SDL_GL_SwapBuffers( );
};