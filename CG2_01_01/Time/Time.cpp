#include "Time.h"
#include <thread>
Time *Time::time_instance_ = nullptr;	// �C���X�^���X


void Time::Create()
{
    if(!time_instance_)
    {
        time_instance_ = new Time;
        time_instance_->frame_count_    = 0;                    // �o�߃t���[��������
        time_instance_->time_           = 0;                    // �o�ߎ��ԏ�����

        time_instance_->fixed_frame_rate_ = 0.02;       // 0.02�b���Ƃ�FixedUpdate
        time_instance_->fixed_update_time_ = 0.0;

        // ����60FPS�ɐݒ�
        time_instance_->frame_rate_ = 1.0f / 60.0f;
    }
}

void Time::Destroy()
{
    delete time_instance_;
    time_instance_ = nullptr;
}

void Time::InstrumentationStart()
{
    // ���g���擾
    QueryPerformanceFrequency(&frequency_);
    // �v���J�n
    QueryPerformanceCounter(&time_count_start_);
}

double Time::InstrumentationEnd()
{
    // ���ݎ��Ԃ��擾
    QueryPerformanceCounter(&time_count_end_);
    // �o�ߎ��Ԍv��
    delta_time_ = static_cast<double>(time_count_end_.QuadPart - time_count_start_.QuadPart) / static_cast<double>(frequency_.QuadPart);
    // �Œ蒷�X�V�p�^�C�}�[�ɉ��Z
    fixed_update_time_ += delta_time_;


    // �o�ߕb�����Z
    time_ += delta_time_;
    // �o�߃t���[��������
    ++frame_count_;

    return delta_time_;
}
