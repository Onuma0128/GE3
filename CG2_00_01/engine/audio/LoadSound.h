#pragma once
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>

struct ChunkHeader {
	char id[4];
	int32_t size;
};

struct RiffHeader {
	ChunkHeader chunk;
	char type[4];
};

struct FormatChunk {
	ChunkHeader chunk;
	WAVEFORMATEX fmt;
};

struct SoundData {
	// 波形フォーマット
	WAVEFORMATEX wfex;
	// バッファの先頭アドレス
	BYTE* pBuffer;
	// バッファのサイズ
	unsigned int bufferSize;
};

// 音声データの読み込み
SoundData SoundLoadWave(const char* filename);
// 音声データの解放
void SoundUnload(SoundData* soundData);
// サウンドの再生
void SoundPlayWave(IXAudio2* xAudio2, const SoundData& soundData);