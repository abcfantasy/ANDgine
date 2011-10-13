#include "SDL.h"
#include "SDL_opengl.h"
#include "Vertex3f.h"
#include "LightingManager.h"

LightingManager* LightingManager::instance_ = NULL;

/*
 * <summary>Gets the singleton instane</summary>
 * <returns>The instance of LightingManager</returns>
 */
LightingManager* LightingManager::Instance()
{
	if ( instance_ == NULL )
		instance_ = new LightingManager();

	return instance_;
}

/*
 * <summary>Constructor</summary>
 */
LightingManager::LightingManager(void)
{
}

/*
 * <summary>Initializes the lighting such that glColor determines ambient and diffuse
 * of materials.</summary>
 */
void LightingManager::initializeColorMaterialLighting(void)
{
	/* Enable lighting */
	glEnable(GL_LIGHTING);
	// enable color tracking
	glEnable(GL_COLOR_MATERIAL);
	// set material properties which will be assigned by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);
}

/*
 * <summary>Sets the global ambient light of the scene.</summary>
 * <param name="color">An RGBA array representing the color of the light.</param>
 */
void LightingManager::setGlobalAmbient( GLfloat color[4] )
{
	glLightModelfv( GL_LIGHT_MODEL_AMBIENT, color );
}

/*
 * <summary>Sets a light using the Phong model.</summary>
 * <param name="light">The light to set (such as GL_LIGHT0). OpenGL supports 8 lights.</param>
 * <param name="ambientLight">An RGBA array for the ambient light.</param>
 * <param name="diffuseLight">An RGBA array for the diffuse light.</param>
 * <param name="specularLight">An RGBA arrary fo the specular light.</param>
 * <param name="position">A vector representing the position of the light.</param>
 */
void LightingManager::setPhongLight( GLenum light, GLfloat ambientLight[4], GLfloat diffuseLight[4], GLfloat specularLight[4], GLfloat position[4] )
{
	glEnable(light);
	
	// Assign created components to given light
	glLightfv(light, GL_AMBIENT, ambientLight);
	glLightfv(light, GL_DIFFUSE, diffuseLight);
	glLightfv(light, GL_SPECULAR, specularLight);
	glLightfv(light, GL_POSITION, position);
}

/*
 * <summary>Sets the specular reflection of a material.</summary>
 * <param name="specular">An RGBA array for the specular reflection.</param>
 * <remarks>Call this function before calls to glVertex</remarks>
 */
void LightingManager::setSpecularReflection( GLfloat specular[4] )
{
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);
}

/*
 * <summary>Moves a light to a new position.</summary>
 * <param name="light">The light to move.</param>
 * <param name="newPosition">The new position of the light.</param>
 */
void LightingManager::moveLight( GLenum light, GLfloat newPosition[4] )
{
	glLightfv(light, GL_POSITION, newPosition );
}

