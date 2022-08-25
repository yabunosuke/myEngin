#include "Time.h"
#include <thread>
Time *Time::time_instance_ = nullptr;	// インスタンス


void Time::Create()
{
    if(!time_instance_)
    {
        time_instance_ = new Time;
        time_instance_->frame_count_    = 0;                    // 経過フレーム初期化
        time_instance_->time_           = 0;                    // 経過時間初期化

        time_instance_->fixed_frame_rate_ = 0.02;       // 0.02秒ごとにFixedUpdate
        time_instance_->fixed_update_time_ = 0.0;

        // 仮で60FPSに設定
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
    // 周波数取得
    QueryPerformanceFrequency(&frequency_);
    // 計測開始
    QueryPerformanceCounter(&time_count_start_);
}

double Time::InstrumentationEnd()
{
    // 現在時間を取得
    QueryPerformanceCounter(&time_count_end_);
    // 経過時間計測
    delta_time_ = static_cast<double>(time_count_end_.QuadPart - time_count_start_.QuadPart) / static_cast<double>(frequency_.QuadPart);
    // 固定長更新用タイマーに加算
    fixed_update_time_ += delta_time_;


    // 経過秒数加算
    time_ += delta_time_;
    // 経過フレーム数増加
    ++frame_count_;

    return delta_time_;
}
