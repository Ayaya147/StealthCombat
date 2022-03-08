#pragma once
#include <Windows.h>
#include <xaudio2.h>
#include <array>
#include <string>

class AudioSystem
{
public:
	AudioSystem();
	~AudioSystem();

	int LoadSound(const std::string& filename);
	void SetVolume(int id, float vol);
	void PlaySoundEx(int id, int loopCount);
	void StopSound(int index);
	void StopSoundAll();

private:
	HRESULT CheckChunk(HANDLE file, DWORD format, DWORD* chunkSize, DWORD* chunkDataPosition);
	HRESULT ReadChunkData(HANDLE file, void* buffer, DWORD buffersize, DWORD bufferoffset);

	IXAudio2* mXAudio2;
	IXAudio2MasteringVoice* mMasteringVoice;

	static constexpr unsigned int mSoundNum = 100;
	std::array<IXAudio2SourceVoice*, mSoundNum> mSourceVoice;
	std::array<BYTE*, mSoundNum> mDataAudio;
	std::array<DWORD, mSoundNum> mSizeAudio;
	std::array<std::string, mSoundNum> mSoundName;
	int mSoundIndex;
};
