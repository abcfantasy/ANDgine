#include "LightingManager.h"
#include <math.h>

/*
 * <summary>Gets the singleton instane</summary>
 * <returns>The instance of LightingManager</returns>
 */
LightingManager* LightingManager::instance()
{
	static LightingManager lm;
	return &lm;
}

/*
 * <summary>Constructor</summary>
 */
LightingManager::LightingManager(void)
{
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
 * <summary>Moves a light to a new position.</summary>
 * <param name="light">The light to move.</param>
 * <param name="newPosition">The new position of the light.</param>
 */
void LightingManager::moveLight( GLenum light, GLfloat newPosition[4] )
{
	glLightfv(light, GL_POSITION, newPosition );
}

void LightingManager::normalize(Vertex4f *v)
{
    // calculate the length of the vector
	float len = sqrtf((v->getX() * v->getX()) + (v->getY() * v->getY()) + (v->getZ() * v->getZ()));

    // avoid division by 0
    if (len == 0.0f)
        len = 1.0f;

    // reduce to unit size
    v->setX( v->getX() / len );
    v->setY( v->getY() / len );
    v->setZ( v->getZ() / len );
}

void LightingManager::calculateNormal(Vertex4f v[3], Vertex4f *normal ) {
	LightingManager::calculateNormal( &v[0], &v[1], &v[2], normal );
};

void LightingManager::calculateNormal(Vertex4f *v1, Vertex4f *v2, Vertex4f *v3, Vertex4f *normal )
{
	Vertex4f a, b;

    // calculate the vectors A and B
    // note that v[3] is defined with counterclockwise winding in mind
    // a
    a.setX( v1->getX() - v2->getX() );
    a.setY( v1->getY() - v2->getY() );
    a.setZ( v1->getZ() - v2->getZ() );
    // b
    b.setX( v2->getX() - v3->getX() );
    b.setY( v2->getY() - v3->getY() );
    b.setZ( v2->getZ() - v3->getZ() );

    // calculate the cross product and place the resulting vector
    // into the address specified by vertex_t *normal
	normal->setX( (a.getY() * b.getZ()) - (a.getZ() * b.getY()) );
    normal->setY( (a.getZ() * b.getX()) - (a.getX() * b.getZ()) );
    normal->setZ( (a.getX() * b.getY()) - (a.getY() * b.getX()) );

    // normalize
    LightingManager::normalize(normal);
}