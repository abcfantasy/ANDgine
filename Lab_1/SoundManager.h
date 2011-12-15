#ifndef _SOUNDMANAGER_H_
#define _SOUNDMANAGER_H_

#include "SDL_mixer.h"

class SoundManager
{

private:
	Mix_Music *mus1;

public:
	// singleton
	static SoundManager* instance();

	// initialize
	void InitializeSound();

	void LoadMusic( char* filePath );
	void PlayMusic();
	void FreeResources();
};

#endif