#include "AudioSystem.h"

AudioSystem::AudioSystem()
{
	CoInitializeEx(nullptr, COINIT_MULTITHREADED);
	XAudio2Create(&mXAudio2, 0);
	mXAudio2->CreateMasteringVoice(&mMasteringVoice);
}

AudioSystem::~AudioSystem()
{
	for (int nCntSound = 0; nCntSound < static_cast<int>(mSoundIndex); nCntSound++)
	{
		if (mSourceVoice[nCntSound])
		{
			mSourceVoice[nCntSound]->Stop(0);
			mSourceVoice[nCntSound]->DestroyVoice();
			mSourceVoice[nCntSound] = nullptr;
			free(mDataAudio[nCntSound]);
			mDataAudio[nCntSound] = nullptr;
		}
	}

	mMasteringVoice->DestroyVoice();
	mMasteringVoice = nullptr;

	if (mXAudio2)
	{
		mXAudio2->Release();
		mXAudio2 = nullptr;
	}

	CoUninitialize();
}

int AudioSystem::LoadSound(const char* filename)
{
	HANDLE file;
	DWORD chunkSize = 0;
	DWORD chunkPosition = 0;
	DWORD filetype = 0;
	WAVEFORMATEXTENSIBLE wfx;
	XAUDIO2_BUFFER buffer;

	for (unsigned int i = 0; i < mSoundIndex; i++)
	{
		if (strcmp(mSoundName[i], filename) == 0)
		{
			return i;
		}
	}

	memset(&wfx, 0, sizeof(WAVEFORMATEXTENSIBLE));
	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));

	file = CreateFile(filename, GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING, 0, nullptr);

	CheckChunk(file, 'FFIR', &chunkSize, &chunkPosition);
	ReadChunkData(file, &filetype, sizeof(DWORD), chunkPosition);
	CheckChunk(file, ' tmf', &chunkSize, &chunkPosition);
	ReadChunkData(file, &wfx, chunkSize, chunkPosition);
	CheckChunk(file, 'atad', &mSizeAudio[mSoundIndex], &chunkPosition);

	mDataAudio[mSoundIndex] = (BYTE*)malloc(mSizeAudio[mSoundIndex]);
	ReadChunkData(file, mDataAudio[mSoundIndex], mSizeAudio[mSoundIndex], chunkPosition);
	mXAudio2->CreateSourceVoice(&mSourceVoice[mSoundIndex], &(wfx.Format));

	strcpy_s(mSoundName[mSoundIndex], 256, filename);

	int retIndex = mSoundIndex;

	mSoundIndex++;

	return retIndex;
}

void AudioSystem::PlaySoundEx(const char* filename, int loopCount)
{
	int index = LoadSound(filename);
	XAUDIO2_VOICE_STATE xa2state;
	XAUDIO2_BUFFER buffer;

	memset(&buffer, 0, sizeof(XAUDIO2_BUFFER));
	buffer.AudioBytes = mSizeAudio[index];
	buffer.pAudioData = mDataAudio[index];
	buffer.Flags = XAUDIO2_END_OF_STREAM;
	buffer.LoopCount = loopCount;

	mSourceVoice[index]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		mSourceVoice[index]->Stop(0);
		mSourceVoice[index]->FlushSourceBuffers();
	}

	mSourceVoice[index]->SubmitSourceBuffer(&buffer);
	mSourceVoice[index]->Start(0);
}

void AudioSystem::StopSound(int index)
{
	XAUDIO2_VOICE_STATE xa2state;

	mSourceVoice[index]->GetState(&xa2state);
	if (xa2state.BuffersQueued != 0)
	{
		mSourceVoice[index]->Stop(0);
		mSourceVoice[index]->FlushSourceBuffers();
	}
}

void AudioSystem::StopSoundAll()
{
	for (int nCntSound = 0; nCntSound < (int)mSoundIndex; nCntSound++)
	{
		if (mSourceVoice[nCntSound])
		{
			mSourceVoice[nCntSound]->Stop(0);
		}
	}
}

HRESULT AudioSystem::CheckChunk(HANDLE file, DWORD format, DWORD* chunkSize, DWORD* chunkDataPosition)
{
	HRESULT hr = S_OK;
	DWORD read = 0;
	DWORD chunkType = 0;
	DWORD chunkDataSize = 0;
	DWORD riffDataSize = 0;
	DWORD fileType = 0;
	DWORD bytesRead = 0;
	DWORD offset = 0;

	if (SetFilePointer(file, 0, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	while (hr == S_OK)
	{
		if (ReadFile(file, &chunkType, sizeof(DWORD), &read, nullptr) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		if (ReadFile(file, &chunkDataSize, sizeof(DWORD), &read, nullptr) == 0)
		{
			hr = HRESULT_FROM_WIN32(GetLastError());
		}

		switch (chunkType)
		{
		case 'FFIR':
			riffDataSize = chunkDataSize;
			chunkDataSize = 4;
			if (ReadFile(file, &fileType, sizeof(DWORD), &read, nullptr) == 0)
			{
				hr = HRESULT_FROM_WIN32(GetLastError());
			}
			break;

		default:
			if (SetFilePointer(file, chunkDataSize, nullptr, FILE_CURRENT) == INVALID_SET_FILE_POINTER)
			{
				return HRESULT_FROM_WIN32(GetLastError());
			}
		}

		offset += sizeof(DWORD) * 2;
		if (chunkType == format)
		{
			*chunkSize = chunkDataSize;
			*chunkDataPosition = offset;

			return S_OK;
		}

		offset += chunkDataSize;
		if (bytesRead >= riffDataSize)
		{
			return S_FALSE;
		}
	}

	return S_OK;
}

HRESULT AudioSystem::ReadChunkData(HANDLE file, void* buffer, DWORD buffersize, DWORD bufferoffset)
{
	DWORD read;

	if (SetFilePointer(file, bufferoffset, nullptr, FILE_BEGIN) == INVALID_SET_FILE_POINTER)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	if (ReadFile(file, buffer,buffersize, &read, nullptr) == 0)
	{
		return HRESULT_FROM_WIN32(GetLastError());
	}

	return S_OK;
}
