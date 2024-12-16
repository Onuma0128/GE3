#include "LoadSound.h"
//#include <cassert>
//
//
//void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData) {
//	HRESULT result{};
//
//	IXAudio2SourceVoice* pSourceVoice = nullptr;
//	result = xAudio2->CreateSourceVoice(&pSourceVoice, &soundData.wfex);
//	assert(SUCCEEDED(result));
//
//	XAUDIO2_BUFFER buf{};
//	buf.pAudioData = soundData.pBuffer;
//	buf.AudioBytes = soundData.bufferSize;
//	buf.Flags = XAUDIO2_END_OF_STREAM;
//
//	result = pSourceVoice->SubmitSourceBuffer(&buf);
//	result = pSourceVoice->Start();
//}
