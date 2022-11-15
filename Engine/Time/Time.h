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
		return fixed_update_time_ >= fixed_delta_time_;
	}
	void SubFixedTimer()
	{
		fixed_update_time_ -= fixed_delta_time_;
	}
	void ClearFixedTimer()
	{
		fixed_update_time_ = 0.0f;
	}
	// 最後のフレームから現在のフレームまでの経過秒数（ReadOnly）
	yEngine::Property<double> time{ &delta_time_,yEngine::AccessorType::ReadOnly };

	yEngine::Property<double> deltaTime{ &fixed_update_time_,yEngine::AccessorType::ReadOnly };
	// 固定フレームレートの更新を実行するインターバル
	yEngine::Property<double> fixedDeltaTime{ &fixed_delta_time_,yEngine::AccessorType::AllAccess };

	// 最初のシーンが起動してからの経過フレーム数（ReadOnly）
	yEngine::Property<int> frameCount{ &frame_count_,yEngine::AccessorType::ReadOnly };
	// 最初のシーンが起動してからの経過秒数（float型）（ReadOnly）
	yEngine::Property<double> timeAsDouble{ &time_,yEngine::AccessorType::ReadOnly };

private:
	//シングルトンパターン
	Time() = default;
	~Time() = default;
	Time(const Time & r) = default;
	Time &operator= (const Time & r) = default;


	static Time *time_instance_;		// インスタンス


	int frame_count_;					// 最初のシーンが起動してからの経過フレーム
	double time_;						// 最初のシーンが起動してからの経過時間（秒）

	double frame_rate_;					// 1フレームにかける時間
	double fixed_delta_time_;			// 固定長更新にかける時間
	LARGE_INTEGER frequency_;			// 周波数
	LARGE_INTEGER time_count_start_;	// 計測開始時間
	LARGE_INTEGER time_count_end_;		// 計測終了時間
	double delta_time_{0.0};					// 1フレームでの経過時間
	double fixed_update_time_;			// 固定長更新用タイマー
};

