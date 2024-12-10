#pragma once

#include <string>

class AudioManager
{
private:

	static AudioManager* instance_;

	AudioManager() = default;
	~AudioManager() = default;
	AudioManager(AudioManager&) = delete;
	AudioManager& operator=(AudioManager&) = delete;

public:

	static AudioManager* GetInstance();


	void LoadAudioFile();
};