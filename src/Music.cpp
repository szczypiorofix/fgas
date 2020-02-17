/*
 * Dungeon Engine
 * Copyright (C) 2020 szczypiorofix <szczypiorofix@o2.pl>
 */

#include <iostream>
#include "Music.h"
#include "Defines.h"





// ############################ BASS ############################

//BASS_Init(-1, 44100, 0, 0, NULL);
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;

// ############### FOR MOD & XM #####################
//HMUSIC music = BASS_MusicLoad(FALSE, "../res/music/ex-aws_cave.xm", 0, 0, 0, 0);
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;

//BASS_ChannelPlay(music, TRUE);

// ########### SAMPLE ############
//HSAMPLE sample = BASS_SampleLoad(FALSE, "../res/music/menu-music.ogg", 0, 0, 1, 0);
//HCHANNEL channel = BASS_SampleGetChannel(sample, TRUE);
//std::cout << "Sample: " << sample << std::endl;
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;
//BASS_Start();
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;
//
//BASS_ChannelPlay(channel, TRUE);
//std::cout << "ERROR CODE: " << BASS_ErrorGetCode() << std::endl;




Music::Music(const std::string musicFile, float volume, bool loop) {
	
	this->channel = 0;
	this->sample = 0;
	this->volume = volume;
	this->loop = loop;
	this->musicFileName = DIR_RES_MUSIC + musicFile;

	int flag = 0;
	if (loop) {
		flag = BASS_SAMPLE_LOOP;
	}

	this->stream = BASS_StreamCreateFile(FALSE, this->musicFileName.c_str(), 0, 0, flag);
	if (this->stream == 0) {
		printf("Failed to load stream! BASS_StreamCreateFile Error: %i\n", BASS_ErrorGetCode());
		exit(1);
	}

}


Music::~Music() {
#ifdef _DEBUG 
	printf("Releasing music ...\n");
#endif
	this->musicFileName = "";
	BASS_StreamFree(this->stream);
}


bool Music::playMusic() {
	if (!BASS_ChannelSetAttribute(this->stream, BASS_ATTRIB_VOL, this->volume)) {
		printf("Failed to change channel volume! BASS_ChannelSetAttribute Error: %i\n", BASS_ErrorGetCode());
	}
	printf("Playing %s ...\n", this->musicFileName.c_str());
	bool r = BASS_ChannelPlay(this->stream, TRUE);
	if (!r) {
		printf("Failed to play channel! BASS_ChannelPlay Error: %i\n", BASS_ErrorGetCode());
	}
	return r;
}


bool Music::playMusic(float _volume) {
	this->volume = _volume;
	if (!BASS_ChannelSetAttribute(this->stream, BASS_ATTRIB_VOL, this->volume)) {
		printf("Failed to change channel volume! BASS_ChannelSetAttribute Error: %i\n", BASS_ErrorGetCode());
	}
	printf("Playing %s ...\n", this->musicFileName.c_str());
	bool r = BASS_ChannelPlay(this->stream, TRUE);
	if (!r) {
		printf("Failed to play channel! BASS_ChannelPlay Error: %i\n", BASS_ErrorGetCode());
	}
	return r;
}


bool Music::stopMusic() {
	return BASS_ChannelStop(this->stream);
}


bool Music::pauseMusic() {
	return BASS_ChannelPause(this->stream);
}


HCHANNEL Music::getChannel() {
	return this->channel;
}


HSTREAM Music::getStream() {
	return this->stream;
}

