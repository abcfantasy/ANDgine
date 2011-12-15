#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "SDL_mixer.h"
#include <map>

class SoundManager
{

private:
	Mix_Music *music;
	std::map<char*, Mix_Chunk*> sounds;
	float* soundPosition;

public:
	// singleton
	static SoundManager* instance();

	// initialize
	void InitializeSound();

	// music (mp3)
	void LoadMusic( char* mp3FilePath );
	void PlayMusic();
	bool IsMusicPlaying();

	// sounds (wav)
	void LoadSound( char* wavFilePath, char* name );
	void LoadAmbientSound( char* wavFilePath, float* position );
	void PlaySound( char* name );
	void PlayAmbientSound();										
	void ChangeAmbientEffect( float* earPosition, float earRotation );	

	// free resources
	void FreeResources();
};

#endif