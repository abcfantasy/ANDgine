#include "SoundManager.h"

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

void SoundManager::LoadMusic( char* filePath ) {
	this->mus1 = Mix_LoadMUS( filePath );
}

void SoundManager::PlayMusic() {
	Mix_PlayMusic( this->mus1, 1 );
}

void SoundManager::FreeResources() {
	Mix_FreeMusic( mus1 );
}
