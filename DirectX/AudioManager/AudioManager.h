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


public:		//構造体
	//ヘッダ
	struct ChunkHeader {
		char id[4];		//チャンク毎のID
		int32_t size;	//チャンクサイズ
	};
	//RIFFヘッダチャンク
	struct RiffHeader {
		ChunkHeader chunk;	//RIFF
		char type[4];		//WAVE
	};
	//FMTチャンク
	struct FormatChunck {
		ChunkHeader chunk;
		WAVEFORMATEX fmt;
	};
	//音声データ
	struct SoundData {
		//波形フォーマット
		WAVEFORMATEX wfex;
		//バッファの先頭
		BYTE *pBuffer;
		//バッファサイズ
		unsigned int bufferSize;
	};

	//再生番号
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

public:		//メンバ関数

	static void Initialize();
	static SoundData SoundLoadWave(const char *filename);
	static void SoundUnLoad(SoundData *soundData);
	static void LoadSoundAll();
	static void SoundPlayWave(SoundName soundName, bool loop);
	static void SoundStopWave(SoundName soundName);
};

