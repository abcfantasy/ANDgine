#include "Model.h"
#include "ResourceManager.h"

// Basic destructor; releases a texture if it has any
Model::~Model() {
	if( this->texture_ != NULL ) {
		ResourceManager::instance()->remove( this->texture_ );	
	}
};

// Loads a texture through the Resource Manager
void Model::setTexture( char *fileName ) {
	// If we already had a texture, we release it
	if( this->texture_ != NULL ) ResourceManager::instance()->remove( this->texture_ );	
	this->texture_ = ResourceManager::instance()->get<TextureResource>( fileName );
};

// Adds a vertex to the model
void Model::addVertex( Vertex3f vertex ) {
	this->vertices_.push_back( vertex );
};

// Compiles the model into a display list
void Model::compile() {
	// We need to free up the old display list if we were using one
	if( this->displayListId_ != Model::INVALID_HANDLE ) {
		glDeleteLists( this->displayListId_, 1 );
	}
	this->displayListId_ = glGenLists( 1 );
	glNewList( this->displayListId_, GL_COMPILE );
	
	// TODO: figure out how to work materials into this

	//float specReflection[] = { 0.2f, 0.2f, 0.2f, 1.0f };
	//glMaterialfv(GL_FRONT, GL_SPECULAR, specReflection);
	//glMateriali(GL_FRONT, GL_SHININESS, 129 );

	// If we have a texture, we're using it, but if we don't we're not using it
	// The code here may be duplicated but it works faster than it would be if we're making a check for every vertex
	if ( this->texture_ != NULL ) {
		glBindTexture( GL_TEXTURE_2D, this->texture_->getTexture()->TextureID );
		glBegin( this->renderMethod_ );
		for( std::vector<Vertex3f>::iterator i = this->vertices_.begin(); i != this->vertices_.end(); ++i ) {
			glNormal3f( i->getNormalX(), i->getNormalY(), i->getNormalZ() );
			glTexCoord2f( i->getU(), i->getV() );
			glColor3f( i->getR(), i->getG(), i->getB() );
			glVertex3f( i->getX(), i->getY(), i->getZ() );
		}
		glEnd();	
	} else {
		glBegin( this->renderMethod_ );
		for( std::vector<Vertex3f>::iterator i = this->vertices_.begin(); i != this->vertices_.end(); ++i ) {
			glNormal3f( i->getNormalX(), i->getNormalY(), i->getNormalZ() );
			glColor3f( i->getR(), i->getG(), i->getB() );
			glVertex3f( i->getX(), i->getY(), i->getZ() );
		}
		glEnd();
	}

	glEndList();
};

// Calls the display list to render the model
void Model::render() {
	// If for some reason it didn't compile, we do it now
	if( this->displayListId_ == Model::INVALID_HANDLE ) this->compile();

	glCallList( this->displayListId_ );
};

// Loads the model from its OBJ file
bool Model::load() {
	FILE *fp;
	char line[100];
   
	std::vector<Vertex3f> vertices;
	std::vector<Vertex3f> normals;
	std::vector<Vertex3f> textures;

	float x, y, z;
	float u, v;
	float normalX, normalY, normalZ;
	int v1, v2, v3;
	int vt1, vt2, vt3;
	int vn1, vn2, vn3;

	char textureFile[100];

	bool texture = false;
	bool normal = false;

	Vertex3f vertex;

    fopen_s( &fp, this->filename_ ,"r");
	
	if (fp == NULL) return false;

    while (fgets(line, 100, fp))
    {
            if (line[0] == 'v' && line[1] == ' ')
            {
                sscanf_s(line, "%*c %f %f %f", &x, &y, &z);
				vertices.push_back( Vertex3f( x, y, z ) );
            } 
			else if (line[0] == 'v' && line[1] == 't')
			{
				sscanf_s(line, "%*[vt] %f %f", &u, &v);
				textures.push_back( Vertex3f(0,0,0, u, v) );
				texture = true;
			}
			else if (line[0] == 'v' && line[1] == 'n')
			{
				sscanf_s(line, "%*[vn] %f %f %f", &normalX, &normalY, &normalZ);
				normals.push_back( Vertex3f(0,0,0,0,0, normalX, normalY, normalZ) );
				normal = true;
			}
			else if (line[0] == 'f' && texture == false && normal == false) 
            {
				sscanf_s(line, "%*c %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &v2,&v3);
				vertex.setPosition( vertices[ v1 - 1 ].getPosition() );
				this->addVertex( vertex );
				vertex.setPosition( vertices[ v2 - 1 ].getPosition() );
				this->addVertex( vertex );
				vertex.setPosition( vertices[ v3 - 1 ].getPosition() );
				this->addVertex( vertex );
            }
			else if (line[0] == 'f' && texture == true && normal == false) 
            {
				sscanf_s(line, "%*c %d/%d %d/%d %d/%d", &v1, &vt1, &v2, &vt2, &v3, &vt3 );
				vertex.setPosition( vertices[ v1 - 1 ].getPosition() );
				vertex.setTexture( textures[ vt1 - 1].getTexture() );
				  
				this->addVertex( vertex );
				  
				vertex.setPosition( vertices[ v2 - 1 ].getPosition() );
				vertex.setTexture( textures[ vt2 - 1].getTexture() );
				  
				this->addVertex( vertex );
				  
				vertex.setPosition( vertices[ v3 - 1 ].getPosition() );
				vertex.setTexture( textures[ vt3 - 1].getTexture() );
				  
				this->addVertex( vertex );
            }
            else if (line[0] == 'f' && texture == true && normal == true) 
            {
				sscanf_s(line, "%*c %d/%d/%d %d/%d/%d %d/%d/%d", &v1, &vt1, &vn1, &v2, &vt2, &vn2, &v3, &vt3, &vn3 );
				vertex.setPosition( vertices[ v1 - 1 ].getPosition() );
				vertex.setTexture( textures[ vt1 - 1].getTexture() );
				vertex.setNormal( normals[ vn1 - 1 ].getNormal() );
				  
				this->addVertex( vertex );
				  
				vertex.setPosition( vertices[ v2 - 1 ].getPosition() );
				vertex.setTexture( textures[ vt2 - 1].getTexture() );
				vertex.setNormal( normals[ vn2 - 1 ].getNormal() );
				  
				this->addVertex( vertex );
				  
				vertex.setPosition( vertices[ v3 - 1 ].getPosition() );
				vertex.setTexture( textures[ vt3 - 1].getTexture() );
				vertex.setNormal( normals[ vn3 - 1 ].getNormal() );
				  
				this->addVertex( vertex );
            }
			else if (line[0] == 't')
			{
				sscanf_s(line, "%*c %s", textureFile);
				setTexture( textureFile );
			}
    }   
    fclose(fp);
	return true;
};

// Special collision conditions
// This *should* be overriden if needed
// Returns true by default
bool Model::checkSpecialCollision( float *position ) {
	return true;
}