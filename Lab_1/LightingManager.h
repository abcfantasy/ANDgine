#ifndef _LIGHTINGMANAGER_H_
#define _LIGHTINGMANAGER_H_

#include "GL/glew.h"
#include "Vertex4f.h"

class LightingManager
{
private:
	static void normalize (Vertex4f *v);

protected:
	LightingManager(void);

public:
	//~LightingManager(void);

	// singleton
	static LightingManager* instance();

	/* use the below functions only after the initialization of OpenGL */
	void setGlobalAmbient( GLfloat color[4] );
	void setPhongLight( GLenum light, GLfloat ambientLight[4], GLfloat diffuseLight[4], GLfloat specularLight[4], GLfloat position[4] );
	void moveLight( GLenum light, GLfloat newPosition[4] );

	// could go somewhere else
	static void calculateNormal(Vertex4f vertices[3], Vertex4f *normal );
	static void calculateNormal(Vertex4f *v1, Vertex4f *v2, Vertex4f *v3, Vertex4f *normal );
};

#endif