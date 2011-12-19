#include "SoundManager.h"
#define _USE_MATH_DEFINES
#include <math.h>

/*
 * <summary>Gets the singleton instane</summary>
 * <returns>The instance of SoundManager</returns>
 */
SoundManager* SoundManager::instance()
{
	static SoundManager sm;
	return &sm;
}

// initialize
void SoundManager::InitializeSound() {
	// Setup audio mode
	Mix_Init(MIX_INIT_MP3);
	Mix_OpenAudio(44100, AUDIO_S16SYS, 2, 2048);
}

// load a music file
void SoundManager::LoadMusic( char* mp3FilePath ) {
	this->music = Mix_LoadMUS( mp3FilePath );
}

// play background music
void SoundManager::PlayMusic() {
	Mix_VolumeMusic( 30 ); // slightly low volume; should be tweakable
	Mix_PlayMusic( this->music, -1 );  // loop music indefinitely
}

// check if background music is playing
// redundant since music loops indefinitely
bool SoundManager::IsMusicPlaying() {
	return Mix_PlayingMusic();
}

// load a sound file
void SoundManager::LoadSound( char* wavFilePath, char* name ) {
	this->sounds.insert( std::pair<char*, Mix_Chunk*>( name, Mix_LoadWAV( wavFilePath ) ) );
}

// loads a sound file with a given position in the 3D world
// NOTE: Currently only supports 1 ambient sound at a time, but can be easily extended
void SoundManager::LoadAmbientSound( char* wavFilePath, float* position ) {
	char* name = "ambient";
	this->sounds.insert( std::pair<char*, Mix_Chunk*>( name, Mix_LoadWAV( wavFilePath ) ) );
	this->soundPosition = position;
}

// plays a given sound
void SoundManager::PlaySound( char* name ) {
	Mix_PlayChannel( -1, this->sounds[name], 0 ); // play in any free channel
}

// plays the ambient sound
void SoundManager::PlayAmbientSound() {
	char* name = "ambient";
	Mix_SetPanning( 3, 0, 0 ); // start with no sound and let it be modified with ChangeAmbientEffect (otherwise causes a quick loud sound in the beginning)
	Mix_PlayChannel( 3, this->sounds[name], -1 );
}

// changes ambient sound settings depending on distance and facing direction of player
void SoundManager::ChangeAmbientEffect( float* earPosition, float earRotation ) {
	// get distance from player to sound source
	float distance = sqrt( pow( this->soundPosition[0] - earPosition[0], 2 ) + pow( this->soundPosition[2] - earPosition[2], 2 ) ) * 5;
	if ( distance <= 255 )
		Mix_SetDistance( 3, (Uint8)distance );

	float rotation;
	// make sure rotation is between 0 to 360
	if ( earRotation >= 0 )
		rotation = earRotation - ( 360 * floor(earRotation / 360) );
	else
		rotation = 360 - ((earRotation * -1) - ( 360 * floor((earRotation*-1) / 360) ));
	// calculate the right sound factor, according to the angle of the sound source to the player, and player's looking direction
	float v = sin( atan2( this->soundPosition[2] - earPosition[2], this->soundPosition[0] - earPosition[0] ) - (rotation * float(M_PI) / 180 ) );
	// map the value between 0 to 254 (currently from -1 to 1)
	v = (v * 127) + 127;

	// set the sound panning left and right
	Mix_SetPanning( 3, 254 - (Uint8)v, (Uint8)v );
}

// free all resources related to sounds
void SoundManager::FreeResources() {
	// free music
	Mix_FreeMusic( music );

	// free sounds
	for ( std::map<char*, Mix_Chunk*>::iterator it = this->sounds.begin(); it != this->sounds.end(); it++) {
		Mix_FreeChunk( it->second );
    }
	this->sounds.clear();
}
