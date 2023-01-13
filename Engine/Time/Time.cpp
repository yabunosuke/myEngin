#include "Time.h"
#include <thread>
#include <ImGui/imgui.h>

Time::Time():

	frame_count_(0),             // �o�߃t���[��������
	time_(0),                    // �o�ߎ��ԏ�����

	fixed_delta_time_(0.02),     // 0.02�b���Ƃ�FixedUpdate
	fixed_update_time_(0.0),

	frame_rate_(1.0f / 60.0f)
{

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
