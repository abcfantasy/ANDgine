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

void SoundManager::LoadMusic( char* mp3FilePath ) {
	this->music = Mix_LoadMUS( mp3FilePath );
}

void SoundManager::PlayMusic() {
	Mix_VolumeMusic( 30 );
	Mix_PlayMusic( this->music, -1 );
}

bool SoundManager::IsMusicPlaying() {
	return Mix_PlayingMusic();
}

void SoundManager::LoadSound( char* wavFilePath, char* name ) {
	this->sounds.insert( std::pair<char*, Mix_Chunk*>( name, Mix_LoadWAV( wavFilePath ) ) );
}

void SoundManager::LoadAmbientSound( char* wavFilePath, float* position ) {
	char* name = "ambient";
	this->sounds.insert( std::pair<char*, Mix_Chunk*>( name, Mix_LoadWAV( wavFilePath ) ) );
	this->soundPosition = position;
}

void SoundManager::PlaySound( char* name ) {
	Mix_PlayChannel( -1, this->sounds[name], 0 );
}

void SoundManager::PlayAmbientSound() {
	char* name = "ambient";
	Mix_SetPanning( 3, 0, 0 ); // start with no sound and let it be modified with ChangeAmbientEffect (otherwise causes a quick loud sound in the beginning)
	Mix_PlayChannel( 3, this->sounds[name], -1 );
}

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

void SoundManager::FreeResources() {
	// free music
	Mix_FreeMusic( music );

	// free sounds
	for ( std::map<char*, Mix_Chunk*>::iterator it = this->sounds.begin(); it != this->sounds.end(); it++) {
		Mix_FreeChunk( it->second );
    }
	this->sounds.clear();
}
