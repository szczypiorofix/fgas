/*
 * For Gold and Sweetrolls
 * Copyright (C) 2020 Piotr Wróblewski <szczypiorofix@o2.pl>
 */

#ifndef _MUSIC_H_
#define _MUSIC_H_
#pragma once

#include <BASS/bass.h>

class Music {

public:

	// DEFAULT MUSIC VOLUME = 100%
	constexpr static float DEFAULT_MUSIC_VOLUME = 1.0f;


	Music(const std::string& musicFile, float volume, bool loop);
	~Music();

	bool playMusic(float _volume);
	bool playMusic();
	bool stopMusic();
	bool pauseMusic();

	HSTREAM getStream();

private:

	HSTREAM stream;

	std::string musicFileName;

	float volume;
	bool loop;

};

#endif
