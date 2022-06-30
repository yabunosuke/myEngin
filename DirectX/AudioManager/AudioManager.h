#pragma once
#include <xaudio2.h>
#pragma comment(lib,"xaudio2.lib")
#include <fstream>
#include <cassert>
#include <wrl.h>			//ComPtr
#pragma comment(lib, "d3dcompiler.lib")
using namespace  Microsoft::WRL;

class AudioManager final
{


public:		//�\����
	//�w�b�_
	struct ChunkHeader {
		char id[4];		//�`�����N����ID
		int32_t size;	//�`�����N�T�C�Y
	};
	//RIFF�w�b�_�`�����N
	struct RiffHeader {
		ChunkHeader chunk;	//RIFF
		char type[4];		//WAVE
	};
	//FMT�`�����N
	struct FormatChunck {
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};
	//�����f�[�^
	struct SoundData {
		//�g�`�t�H�[�}�b�g
		WAVEFORMATEX wfex;
		//�o�b�t�@�̐擪
		BYTE *pBuffer;
		//�o�b�t�@�T�C�Y
		unsigned int bufferSize;
	};

	//�Đ��ԍ�
	enum SoundName {
		Title,
		MAX
	};

private:
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;

	AudioManager();
	~AudioManager();
public:
	AudioManager(const AudioManager &obj) = delete;
	AudioManager &operator=(const AudioManager &obj) = delete;
	static AudioManager *GetIns();

	static ComPtr<IXAudio2> xAudio2;
	static IXAudio2MasteringVoice *mastervoice;
	static IXAudio2SourceVoice *pSoureceVoice[MAX];
	static SoundData sound[MAX];

public:		//�����o�֐�

	static void Initialize();
	static SoundData SoundLoadWave(const char *filename);
	static void SoundUnLoad(SoundData *soundData);
	static void LoadSoundAll();
	static void SoundPlayWave(SoundName soundName, bool loop);
	static void SoundStopWave(SoundName soundName);
};

