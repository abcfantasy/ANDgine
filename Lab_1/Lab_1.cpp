/*
 * This is template created for Game Engines 2011 based on the SDL examples from http://nehe.gamedev.net/
 */

#include <stdio.h>
#include <stdlib.h>
#include "SDL.h"
#include "SDL_opengl.h"

#include <vector>
#include "Vertex3f.h"
#include "SceneNode.h"
#include "ModelNode.h"
#include "ContainerNode.h"

/* screen width, height, and bit depth */
#define SCREEN_WIDTH  640
#define SCREEN_HEIGHT 480
#define SCREEN_BPP     16

/* Define our booleans */
#define TRUE  1
#define FALSE 0

/* This is our SDL surface */
SDL_Surface *surface;

/* function to release/destroy our resources and restoring the old desktop */
void Quit( int returnCode )
{
	/* clean up the window */
	SDL_Quit( );

	/* and exit appropriately */
	exit( returnCode );
}

/* function to reset our viewport after a window resize */
int resizeWindow( int width, int height )
{

	return( TRUE );
}

/* function to handle key press events */
void handleKeyPress( SDL_keysym *keysym )
{

	return;
}

/* general OpenGL initialization function */
int initGL( GLvoid )
{

	/* Enable smooth shading */
	glShadeModel( GL_SMOOTH );

	/* Set the background Color*/
	glClearColor( 0.0f, 0.0f, 0.0f, 0.0f );

	/* Depth buffer setup */
	glClearDepth( 1.0f );

	/* Enables Depth Testing */
	glEnable( GL_DEPTH_TEST );

	/* The Type Of Depth Test To Do */
	glDepthFunc( GL_LEQUAL );

	/* Really Nice Perspective Calculations */
	glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

	return( TRUE );
}

float rpyramid = 0.0f;
ContainerNode sceneGraph;

void initializeScene() {
	ModelNode *pyramid = new ModelNode();
	pyramid->AddVertex( Vertex3f( 0.0f, 0.5f, 0.0f, 1.0, 0.0, 0.0 ) );
	pyramid->AddVertex( Vertex3f( -0.5f, -0.5f, 0.5f, 0.0, 1.0, 0.0 ) );
	pyramid->AddVertex( Vertex3f( 0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0 ) );

	pyramid->AddVertex( Vertex3f( 0.0f, 0.5f, 0.0f, 1.0, 0.0, 0.0 ) );
	pyramid->AddVertex( Vertex3f( 0.5f, -0.5f, 0.5f, 0.0, 0.0, 1.0 ) );
	pyramid->AddVertex( Vertex3f( 0.5f, -0.5f, -0.5f, 0.0, 1.0, 0.0 ) );

	pyramid->AddVertex( Vertex3f( 0.0f, 0.5f, 0.0f, 1.0, 0.0, 0.0 ) );
	pyramid->AddVertex( Vertex3f( 0.5f, -0.5f, -0.5f, 0.0, 1.0, 0.0 ) );
	pyramid->AddVertex( Vertex3f( -0.5f,-0.5f, -0.5f, 0.0, 0.0, 1.0 ) );
	
	pyramid->AddVertex( Vertex3f( 0.0f, 0.5f, 0.0f, 1.0, 0.0, 0.0 ) );
	pyramid->AddVertex( Vertex3f( -0.5f, -0.5f,-0.5f, 0.0, 0.0, 1.0 ) );
	pyramid->AddVertex( Vertex3f( -0.5f, -0.5f, 0.5f, 0.0, 1.0, 0.0 ) );

	pyramid->AddVertex( Vertex3f( -0.5f, -0.5f,-0.5f, 0.0f, 0.0f, 1.0f ) );
	pyramid->AddVertex( Vertex3f( -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f ) );
	pyramid->AddVertex( Vertex3f( 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f ) );

	pyramid->AddVertex( Vertex3f( 0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f ) );
	pyramid->AddVertex( Vertex3f( -0.5f, -0.5f, 0.5f, 0.0f, 1.0f, 0.0f ) );
	pyramid->AddVertex( Vertex3f( 0.5f, -0.5f, -0.5f, 0.0f, 1.0f, 0.0f ) );
	
	pyramid->setVelocity( 0.0f, 0.0f, 0.0f );
	pyramid->setAngleVelocity( 0.0f, 90.0f, 0.0f );

	sceneGraph.AddObject( pyramid );
};

/* Here goes our drawing code */
int drawGLScene( float deltaT )
{
	/* Clear The Screen And The Depth Buffer */
	glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );

	/*HERE you should put your code in order to do render something on the screen, use lighting, modify the camera position etc... */
	glLoadIdentity();
	sceneGraph.render( deltaT );

	/* Draw it to the screen */
	SDL_GL_SwapBuffers( );

	return( TRUE );
}

int main( int argc, char **argv )
{
	/* Flags to pass to SDL_SetVideoMode */
	int videoFlags;
	/* main loop variable */
	int done = FALSE;
	/* used to collect events */
	SDL_Event event;
	/* this holds some info about our display */
	const SDL_VideoInfo *videoInfo;

	int timeLastFrame = 0;
	int timeThisFrame;


	/* initialize SDL */
	if ( SDL_Init( SDL_INIT_VIDEO ) < 0 )
	{
		fprintf( stderr, "Video initialization failed: %s\n",
			 SDL_GetError( ) );
		Quit( 1 );
	}

	/* Fetch the video info */
	videoInfo = SDL_GetVideoInfo( );

	if ( !videoInfo )
	{
		fprintf( stderr, "Video query failed: %s\n",
			 SDL_GetError( ) );
		Quit( 1 );
	}

	/* the flags to pass to SDL_SetVideoMode */
	videoFlags  = SDL_OPENGL;          /* Enable OpenGL in SDL */
	videoFlags |= SDL_GL_DOUBLEBUFFER; /* Enable double buffering */
	videoFlags |= SDL_HWPALETTE;       /* Store the palette in hardware */
	videoFlags |= SDL_RESIZABLE;       /* Enable window resizing */

	/* This checks to see if surfaces can be stored in memory */
	if ( videoInfo->hw_available )
	videoFlags |= SDL_HWSURFACE;
	else
	videoFlags |= SDL_SWSURFACE;

	/* This checks if hardware blits can be done */
	if ( videoInfo->blit_hw )
	videoFlags |= SDL_HWACCEL;

	/* Sets up OpenGL double buffering */
	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

	/* get a SDL surface */
	surface = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP,
				videoFlags );

	/* Verify there is a surface */
	if ( !surface )
	{
		fprintf( stderr,  "Video mode set failed: %s\n", SDL_GetError( ) );
		Quit( 1 );
	}

	/* initialize OpenGL */
	initGL( );

	/* resize the initial window */
	resizeWindow( SCREEN_WIDTH, SCREEN_HEIGHT );

	/* initializing scene */
	initializeScene();

	/* wait for events */ 
	while ( !done )
	{
		/* handle the events in the queue */

		while ( SDL_PollEvent( &event ) )
		{
			switch( event.type )
			{
			  
			case SDL_VIDEORESIZE:
				/* handle resize event */
				surface = SDL_SetVideoMode( event.resize.w,
							event.resize.h,
							16, videoFlags );
				if ( !surface )
				{
					fprintf( stderr, "Could not get a surface after resize: %s\n", SDL_GetError( ) );
					Quit( 1 );
				}
				resizeWindow( event.resize.w, event.resize.h );
				break;

			case SDL_QUIT:
				/* handle quit requests */
				done = TRUE;
				break;
			default:
				break;
			}
		}

		timeThisFrame = SDL_GetTicks();
		/* draw the scene */
		drawGLScene( float(timeThisFrame - timeLastFrame) );
		timeLastFrame = timeThisFrame;
	}

	/* clean ourselves up and exit */
	Quit( 0 );

	/* Should never get here */
	return( 0 );
}
