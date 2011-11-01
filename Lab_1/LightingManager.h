#ifndef _LIGHTINGMANAGER_H_
#define _LIGHTINGMANAGER_H_

#include "SDL.h"
#include "SDL_opengl.h"

/*
 * Lighting Manager
 *
 * Singleton class that handles lights. 
 * 
 * Call initializeColorMaterialLighting() during the initialization of OpenGL to enable lighting.
 */
class LightingManager
{
protected:
	LightingManager(void);

public:
	//~LightingManager(void);

	// singleton
	static LightingManager* instance();

	/* use the below functions only within the initialization of OpenGL */
	void initializeColorMaterialLighting(void);

	/* use the below functions only after the initialization of OpenGL */
	void setGlobalAmbient( GLfloat color[4] );
	void setPhongLight( GLenum light, GLfloat ambientLight[4], GLfloat diffuseLight[4], GLfloat specularLight[4], GLfloat position[4] );
	void moveLight( GLenum light, GLfloat newPosition[4] );

	void LightingManager::setSpecularReflection( GLfloat specular[4] );
};

#endif