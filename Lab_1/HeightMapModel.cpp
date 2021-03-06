#include "HeightMapModel.h"
#include "ResourceManager.h"
#include "TextureResource.h"
#include "TGAImage.h"
#include "Math.h"
#include "SDL.h"

#include "HeightMapGen.h"

int HeightMapModel::NEXT_DIRECTION = HeightMapModel::DIRECTION_NORTH;
float* HeightMapModel::PREVIOUS_MAP = NULL;

// Easy access to a particular vertex
// heightmap( i, j ) = the vertex at position i,j
Vertex3f* HeightMapModel::operator()( int i, int j ) {
	return &this->vertices_[ j * this->width_ + i ];
};

// Overrides the method from Model, because we're rendering with a triangle strip and in a different order
void HeightMapModel::compile(){
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
		int i, j;
		Vertex3f *point;
		glBindTexture( GL_TEXTURE_2D, this->texture_->getTexture()->TextureID );
		for ( i = 0 ; i < this->length_ - 1; ++i ) {
			glBegin(GL_TRIANGLE_STRIP);
			for ( j = 0; j < this->width_; ++j ) {
				point = (*this)( i + 1, j );
				glColor3f( point->getR(), point->getG(), point->getB() );
				glTexCoord2f( point->getU(), point->getV() );
				glNormal3f( point->getNormalX(), point->getNormalY(), point->getNormalZ() );
				glVertex3f(	point->getX(), point->getY(), point->getZ() );

				point = (*this)( i, j );
				glColor3f( point->getR(), point->getG(), point->getB() );
				glTexCoord2f( point->getU(), point->getV() );
				glNormal3f( point->getNormalX(), point->getNormalY(), point->getNormalZ() );
				glVertex3f(	point->getX(), point->getY(), point->getZ() );
			}
			glEnd();
		}	
	} else {
		int i, j;
		Vertex3f *point;
		for ( i = 0 ; i < this->length_ - 1; ++i ) {
			glBegin(GL_TRIANGLE_STRIP);
			for ( j = 0; j < this->width_; ++j ) {
				point = (*this)( i + 1, j );
				glColor3f( point->getR(), point->getG(), point->getB() );
				glNormal3f( point->getNormalX(), point->getNormalY(), point->getNormalZ() );
				glVertex3f(	point->getX(), point->getY(), point->getZ() );

				point = (*this)( i, j );
				glColor3f( point->getR(), point->getG(), point->getB() );
				glNormal3f( point->getNormalX(), point->getNormalY(), point->getNormalZ() );
				glVertex3f(	point->getX(), point->getY(), point->getZ() );
			}
			glEnd();
		}
	}

	glEndList();
};

// Calculates the normals for lighting; this method is magic :D
// TODO: Still needs some optimization
void HeightMapModel::computeNormals() {
	float norm1[3], norm2[3], norm3[3], norm4[3];
	int i, j;

	Math::makeNullVector( norm1 );
	Math::makeNullVector( norm2 );
	Math::makeNullVector( norm3 );
	Math::makeNullVector( norm4 );

	for(i = 0; i < this->length_; i++)
		for(j = 0; j < this->width_; j++) {
			// normals for the four corners
			if( i == 0 && j == 0 ) {
				Math::crossProduct( (*this)( 0, 0 )->getPosition(), (*this)( 0, 1 )->getPosition(), (*this)( 1, 0 )->getPosition(), norm1 );			
			} else if( j == this->width_ - 1 && i == this->length_ - 1 ) {
				Math::crossProduct( (*this)( i, j )->getPosition(), (*this)( j, i - 1 )->getPosition(), (*this)( j - 1, i )->getPosition(), norm1 );	
			} else if( j == 0 && i == this->length_ - 1 ) {
				Math::crossProduct( (*this)( i, j )->getPosition(), (*this)( j, i - 1 )->getPosition(), (*this)( j + 1, i )->getPosition(), norm1 );			
			} else if( j == this->width_ - 1 && i == 0 ) {
				Math::crossProduct( (*this)( i, j )->getPosition(), (*this)( j, i + 1 )->getPosition(), (*this)( j - 1, i )->getPosition(), norm1 );			
			}

			// normals for the borders
			else if( i == 0 ) {
				Math::crossProduct( (*this)( j, 0 )->getPosition(), (*this)( j - 1, 0 )->getPosition(), (*this)( j, 1 )->getPosition(), norm1 );
				Math::crossProduct( (*this)( j, 0 )->getPosition(), (*this)( j, 1 )->getPosition(), (*this)( j + 1, 0 )->getPosition(), norm2 );
			} else if( j == 0 ) {
				Math::crossProduct( (*this)( 0, i )->getPosition(), (*this)( 1, i )->getPosition(), (*this)( 0, i - 1 )->getPosition(), norm1 );
				Math::crossProduct( (*this)( 0, i )->getPosition(), (*this)( 0, i + 1 )->getPosition(), (*this)( 1, i )->getPosition(), norm2 );
			} else if( i == this->length_ ) {
				Math::crossProduct( (*this)( j, i )->getPosition(), (*this)( j, i - 1 )->getPosition(), (*this)( j + 1, i )->getPosition(), norm1 );
				Math::crossProduct( (*this)( j, i )->getPosition(), (*this)( j + 1, i )->getPosition(), (*this)( j, i - 1 )->getPosition(), norm2 );
			} else if( j == this->width_ ) {
				Math::crossProduct( (*this)( j, i )->getPosition(), (*this)( j, i - 1 )->getPosition(), (*this)( j - 1, i )->getPosition(), norm1 );
				Math::crossProduct( (*this)( j, i )->getPosition(), (*this)( j - 1, i )->getPosition(), (*this)( j, i + 1 )->getPosition(), norm2 );
			}

			// normals for the interior
			else {
				Math::crossProduct( (*this)( j, i )->getPosition(), (*this)( j - 1, i )->getPosition(), (*this)( j, i + 1 )->getPosition(), norm1 );
				Math::crossProduct( (*this)( j, i )->getPosition(), (*this)( j, i + 1 )->getPosition(), (*this)( j + 1, i )->getPosition(), norm2 );
				Math::crossProduct( (*this)( j, i )->getPosition(), (*this)( j + 1, i )->getPosition(), (*this)( j, i - 1 )->getPosition(), norm3 );
				Math::crossProduct( (*this)( j, i )->getPosition(), (*this)( j, i - 1 )->getPosition(), (*this)( j - 1, i )->getPosition(), norm4 );
			}
			Math::normalize( norm1 );
			Math::normalize( norm2 );
			Math::normalize( norm3 );
			Math::normalize( norm4 );
			Math::addVector( norm1, norm2 );
			Math::addVector( norm1, norm3 );
			Math::addVector( norm1, norm4 );
			Math::makeNullVector( norm2 );
			Math::makeNullVector( norm3 );
			Math::makeNullVector( norm4 );
			Math::normalize(norm1);
			norm1[2] = -norm1[2];
			(*this)( i, j )->setNormal( norm1 );
		}
};

bool HeightMapModel::load() {
	if( this->filename_[0] ) return loadFromImage();
	else return loadFromSeed();
};

bool HeightMapModel::loadFromSeed() {
	float map[257 * 257];
	// If you pass "size" to the function, the size of the map should be ( size + 1 )^2
	// SDL_GetTicks() is a pretty random seed
	// High h => smooth map
	// Low h => rough map

	if( PREVIOUS_MAP != NULL ) {
	switch( NEXT_DIRECTION ) {
		case DIRECTION_NORTH:
			HeightMapGen::generateHeightMapNorth( map, 256, SDL_GetTicks(), 1, 1.0f, PREVIOUS_MAP );
			break;
		case DIRECTION_SOUTH:
			HeightMapGen::generateHeightMapSouth( map, 256, SDL_GetTicks(), 1, 1.0f, PREVIOUS_MAP );
			break;
		case DIRECTION_EAST:
			HeightMapGen::generateHeightMapEast( map, 256, SDL_GetTicks(), 1, 1.0f, PREVIOUS_MAP );
			break;
		case DIRECTION_WEST:
			HeightMapGen::generateHeightMapWest( map, 256, SDL_GetTicks(), 1, 1.0f, PREVIOUS_MAP );
			break;
		}
		delete[] PREVIOUS_MAP;
		PREVIOUS_MAP = NULL;
	} else
		HeightMapGen::generateHeightMap( map, 256, SDL_GetTicks(), 1, 1.0f );

	//HeightMapGen::generateHeightMap( map, 256, SDL_GetTicks(), 1, 1.0f );

	this->width_ = this->length_ = 257;
	float textureIncrementX = 1.0f / this->width_;
	float textureIncrementZ = 1.0f / this->length_;
	
	float xOffset = 0.0f;
	float yOffset = 0.0f;

	// This calculates the amplitude of the scaling
	float amp = this->maxScale_ - this->minScale_;

	// We're applying all the modifications to a single vertex
	// The vertices in the array are copies anyway
	Vertex3f point;

	// We start calculating the vertices
	for (int i = 0 ; i < this->length_; ++i)
		for (int j = 0;j < this->width_; ++j ) {
			// The X and Z positions depend on the point we're at
			point.setX( float(j) + xOffset );
			point.setZ( float(i) + xOffset );
			// The Y position is interpolated using the amplitude
			point.setY( amp * (map[(this->width_ * j) + i]) + minScale_ );

			// The texture coordinates are also easy
			point.setU( j * textureIncrementX );
			point.setV( i * textureIncrementZ );

			//point.setR( 1.0f );
			//point.setG( 0.5f );
			//point.setB( 0.5f );

			this->addVertex( point );
		}
	
	// We need to calculate the normals for lighting
	this->computeNormals();
	// The rendering is done as a triangle strip
	this->renderMethod_ = GL_TRIANGLE_STRIP;

	return true;
};

bool HeightMapModel::loadFromImage() {
	TGAImage *tgaImage = ResourceManager::instance()->get<TGAImage>( this->filename_ );
	if( tgaImage == NULL )
		return false;

	// The width and length of the model (in units) are the same as the values in pixels
	this->width_ = tgaImage->width_;
	this->length_ = tgaImage->height_;
	
	// The texture coordinates will be at fixed increments which we compute here
	float textureIncrementX = 1.0f / this->width_;
	float textureIncrementZ = 1.0f / this->length_;
	// The model will be centered around the origin
	float xOffset = 0.0f;//-(this->width_ / 2.0f);
	float zOffset = 0.0f;//-(this->length_ / 2.0f);

	// This calculates the amplitude of the scaling
	float amp = this->maxScale_ - this->minScale_;
	// This keeps track of the byte we're at in the image
	int pixel = 0;
	// We're applying all the modifications to a single vertex
	// The vertices in the array are copies anyway
	Vertex3f point;
	
	// We start calculating the vertices
	for (int i = 0 ; i < this->length_; ++i)
		for (int j = 0;j < this->width_; ++j, pixel += tgaImage->bytesPerPx_ ) {
			// The X and Z positions depend on the pixel we're at
			point.setX( float(j) + xOffset );
			point.setZ( float(i) + zOffset );
			// The Y position scales with the last component of the pixel
			// And is interpolated using the amplitude
			point.setY( amp * ( tgaImage->image_[pixel + ( tgaImage->bytesPerPx_ - 1 ) ] / 256.0f ) );

			// The texture coordinates are also easy
			point.setU( j * textureIncrementX );
			point.setV( i * textureIncrementZ );

			// If mode = RGBA then we fill in the colors as well
			if ( tgaImage->bytesPerPx_ == 4 ) {
				point.setR( tgaImage->image_[pixel] / 256.0f );
				point.setG( tgaImage->image_[pixel + 1] / 256.0f );
				point.setB( tgaImage->image_[pixel + 2] / 256.0f );
			}
			this->addVertex( point );
		}
	// We're done with the image now
	ResourceManager::instance()->remove( tgaImage );
	
	// We need to calculate the normals for lighting
	this->computeNormals();
	// The rendering is done as a triangle strip
	this->renderMethod_ = GL_TRIANGLE_STRIP;

	return true;
};

// Gets the interpolated height at the coordinates given
float HeightMapModel::getHeight( float x, float z ) {
	int coordX, coordZ;
	float scaleX, scaleZ;
	float a, b, c, d;
	
	if( x < 0 || z < 0 || x >= width_ || z >= width_ )
		return 0;

	coordX = (int)x;
	coordZ = (int)z;

	scaleX = (float)x - coordX;
	scaleZ = (float)z - coordZ;

	a = (*this)( coordX, coordZ )->getY();
	b = (*this)( coordX + 1, coordZ )->getY();
	c = (*this)( coordX, coordZ + 1 )->getY();
	d = (*this)( coordX + 1, coordZ + 1 )->getY();

	return a * ( 1 - scaleX ) * ( 1 - scaleZ ) + b * scaleX * ( 1 - scaleZ ) + c * ( 1 - scaleX ) * scaleZ + d * scaleX * scaleZ;
};

// Rescales the model between min and max
void HeightMapModel::rescale( float minScale, float maxScale ) {
	float oldAmp = this->maxScale_ - this->minScale_;
	float newAmp = maxScale - minScale;

	for( std::vector<Vertex3f>::iterator i = this->vertices_.begin(); i != this->vertices_.end(); ++i )
		i->setY( i->getY() * newAmp / oldAmp );

	this->minScale_ = minScale;
	this->maxScale_ = maxScale;
};

// Special collision method - we're not using a bounding box for this kind of model
// We're checking the height at each point
bool HeightMapModel::checkSpecialCollision( float *position ) {
	float height = getHeight( position[0], position[2] );
	if( height > position[1] + 0.2 ) return true;
	return false;
};
