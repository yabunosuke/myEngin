#include "AudioManager.h"
ComPtr<IXAudio2> AudioManager::xAudio2;
IXAudio2MasteringVoice *AudioManager::mastervoice;
IXAudio2SourceVoice *AudioManager::pSoureceVoice[MAX];
AudioManager::SoundData AudioManager::sound[MAX];

AudioManager::AudioManager()
{
}

AudioManager::~AudioManager()
{
}
AudioManager *AudioManager::GetIns()
{
	static AudioManager ins;
	return &ins;
}

void AudioManager::Initialize()
{
	XAudio2Create(&xAudio2, 0, XAUDIO2_DEFAULT_PROCESSOR);
	xAudio2->CreateMasteringVoice(&mastervoice);
	LoadSoundAll();
}

AudioManager::SoundData AudioManager::SoundLoadWave(const char *filename)
{
	//ファイルオープン
	std::ifstream file;
	file.open(filename, std::ios_base::binary);
	assert(file.is_open());
	
	//読み込み
	//ヘッダー読み込み
	RiffHeader riff;
	file.read((char *)&riff, sizeof(riff));
	if (strncmp(riff.chunk.id, "RIFF", 4) != 0) {
		assert(0);
	}
	if (strncmp(riff.type, "WAVE", 4) != 0) {
		assert(0);
	}

	FormatChunck format = {};
	while (true) {
		file.read((char *)&format, sizeof(ChunkHeader));
		if (strncmp(format.chunk.id, "fmt ", 4) == 0) {
			break;
		}
		else {
			file.seekg(format.chunk.size, std::ios_base::cur);
		}
	}
	assert(format.chunk.size <= sizeof(format.fmt));
	file.read((char *)&format.fmt, format.chunk.size);
	
	ChunkHeader data;
	file.read((char *)&data, sizeof(data));
	if (strncmp(data.id, "JUNK ", 4) == 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char *)&data, sizeof(data));
	}
	if (strncmp(data.id, "data ", 4) != 0) {
		file.seekg(data.size, std::ios_base::cur);
		file.read((char *)&data, sizeof(data));
	}
	char *pBuffer = new char[data.size];
	file.read(pBuffer, data.size);
	file.close();


	SoundData soundData = {};

	soundData.wfex = format.fmt;
	soundData.pBuffer = reinterpret_cast<BYTE *>(pBuffer);
	soundData.bufferSize = data.size;
	return soundData;
}

void AudioManager::SoundUnLoad(SoundData *soundData)
{
	xAudio2.Reset();
	delete[] soundData->pBuffer;
	soundData->pBuffer = 0;
	soundData->bufferSize = 0;
	soundData->wfex = {};
}

void AudioManager::LoadSoundAll()
{
}

void AudioManager::SoundPlayWave(SoundName soundName, bool loop)
{
	
	xAudio2->CreateSourceVoice(&pSoureceVoice[soundName], &sound[soundName].wfex);
	
	XAUDIO2_BUFFER buf{};
	buf.pAudioData = sound[soundName].pBuffer;
	buf.AudioBytes = sound[soundName].bufferSize;
	buf.Flags = XAUDIO2_END_OF_STREAM;
	if (loop) {
		buf.LoopCount = 255;
	}
	else {
		buf.LoopCount = 0;
	}

	pSoureceVoice[soundName]->SubmitSourceBuffer(&buf);
	pSoureceVoice[soundName]->Start();
}

void AudioManager::SoundStopWave(SoundName soundName)
{
	pSoureceVoice[soundName]->Stop();
	
}
