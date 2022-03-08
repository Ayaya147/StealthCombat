#pragma once
#include <Windows.h>
#include <xaudio2.h>

#define MAX_SOUND_NUM (100)

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	int LoadSound(const char* filename);
	void SetVolume(int id, float vol);
	void PlaySoundEx(int id, int loopCount);
	void StopSound(int index);
	void StopSoundAll();

private:
	HRESULT CheckChunk(HANDLE file, DWORD format, DWORD* chunkSize, DWORD* chunkDataPosition);
	HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD buffersize, DWORD bufferoffset);

	IXAudio2 *mXAudio2 = nullptr;
	IXAudio2MasteringVoice *mMasteringVoice = nullptr;
	IXAudio2SourceVoice *mSourceVoice[MAX_SOUND_NUM] = {};
	BYTE* mDataAudio[MAX_SOUND_NUM] = {};
	DWORD mSizeAudio[MAX_SOUND_NUM] = {};
	char mSoundName[MAX_SOUND_NUM][256] = {};
	DWORD mSoundIndex = 0;
};
