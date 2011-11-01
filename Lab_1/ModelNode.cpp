#include "ModelNode.h"

#include "SDL.h"
#include "SDL_opengl.h"

ModelNode::ModelNode() {
	this->Initialize();
	this->textureHandle = -1;
};

ModelNode::ModelNode( int texture ) {
	this->Initialize();
	this->textureHandle = texture;
}

ModelNode::~ModelNode() {
};

void ModelNode::Initialize() {
	this->setRotation( 0, 0, 0 );
	this->setPosition( 0, 0, 0 );
	this->setVelocity( 0, 0, 0 );
	this->setAngleVelocity( 0, 0, 0 );
	this->setDisplayListId( -1 );
};

void ModelNode::AddVertex( Vertex3f vertex ) {
	this->vertices_.push_back( vertex );
};

void ModelNode::compile() {
	if( this->getDisplayListId() != -1 ) {
		glDeleteLists( this->getDisplayListId(), 1 );
	}
	this->setDisplayListId( glGenLists( 1 ) );
	glNewList( this->getDisplayListId(), GL_COMPILE );
	
	float *position = this->getPosition();
	glTranslatef( position[0], position[1], position[2] );

	float *rotation = this->getRotation();
	glRotatef( rotation[0], 1.0, 0.0, 0.0 );
	glRotatef( rotation[1], 0.0, 1.0, 0.0 );
	glRotatef( rotation[2], 0.0, 0.0, 1.0 );
	
	// if( textureId != -1 )
	// render with textures
	//float specReflection[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	//glMateriali(GL_FRONT, GL_SHININESS, 129 );

	glBegin( GL_TRIANGLES );

	// if model has texture
	if ( textureHandle >= 0 )
		glBindTexture( GL_TEXTURE_2D, textureHandle );

	for( std::vector<Vertex3f>::iterator i = this->vertices_.begin(); i != this->vertices_.end(); ++i ) {
		glNormal3f( i->getNormalX(), i->getNormalY(), i->getNormalZ() );
		glTexCoord2f( i->getU(), i->getV() );
		glColor3f( i->getR(), i->getG(), i->getB() );
		glVertex3f( i->getX(), i->getY(), i->getZ() );
	}
	glEnd();

	glEndList();
};

void ModelNode::render( float deltaT ) {
	this->translate( this->getVelocity(), deltaT );
	this->rotate( this->getAngleVelocity(), deltaT );

	if( this->getDisplayListId() == -1 ) {
		this->compile();
	}

	glPushMatrix();
	glCallList( this->getDisplayListId() );
	glPopMatrix();
}