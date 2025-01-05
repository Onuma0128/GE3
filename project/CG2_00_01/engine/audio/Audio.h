#pragma once

#include <xaudio2.h>
#include <wrl.h>

#include <string>

using Microsoft::WRL::ComPtr;

class Audio
{
public:
	Audio();
	~Audio();

	void SoundPlayWave(const std::string& filePath, float volume);

private:
	ComPtr<IXAudio2> xAudio2_;
	IXAudio2MasteringVoice* masterVoice_ = nullptr;

};