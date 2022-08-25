#pragma once
#include <Windows.h>
#include "Property.h"
class Time final
{
public:
	static Time *GetInstance()
	{
		return time_instance_;
	}

	static void Create();
	static void Destroy();


	void InstrumentationStart();
	double InstrumentationEnd();
	bool CheckFixedUpdate()
	{
		return fixed_update_time_ >= fixed_frame_rate_;
	}
	void SubFixedTimer()
	{
		fixed_update_time_ -= fixed_frame_rate_;
	}

	// �Ō�̃t���[�����猻�݂̃t���[���܂ł̌o�ߕb���iReadOnly�j
	Property<double> deltaTime{ fixed_update_time_,AccessorType::ReadOnly };
	// �ŏ��̃V�[�����N�����Ă���̌o�߃t���[�����iReadOnly�j
	Property<int> frameCount{ frame_count_,AccessorType::ReadOnly };
	// �ŏ��̃V�[�����N�����Ă���̌o�ߕb���ifloat�^�j�iReadOnly�j
	Property<double> timeAsDouble{ time_,AccessorType::ReadOnly };

private:
	//�V���O���g���p�^�[��
	Time() = default;
	~Time() = default;
	Time(const Time & r) = default;
	Time &operator= (const Time & r) = default;


	static Time *time_instance_;		// �C���X�^���X


	int frame_count_;					// �ŏ��̃V�[�����N�����Ă���̌o�߃t���[��
	double time_;						// �ŏ��̃V�[�����N�����Ă���̌o�ߎ��ԁi�b�j

	double frame_rate_;					// 1�t���[���ɂ����鎞��
	double fixed_frame_rate_;			// �Œ蒷�X�V�ɂ����鎞��
	LARGE_INTEGER frequency_;			// ���g��
	LARGE_INTEGER time_count_start_;	// �v���J�n����
	LARGE_INTEGER time_count_end_;		// �v���I������
	double delta_time_;				// 1�t���[���ł̌o�ߎ���
	double fixed_update_time_;			// �Œ蒷�X�V�p�^�C�}�[
};

