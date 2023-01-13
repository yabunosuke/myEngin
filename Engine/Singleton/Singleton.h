#pragma once

#include <mutex>

class SingletonFinalizer
{
public:
	using FinalizerFunc = void(*)();
	static void AddFinalizer(FinalizerFunc func);
	static void Finalize();
};

template <typename T>
class Singleton
{
public:
	static T &GetInstance()
	{
		std::call_once(init_flag_, Create);
		return *instance_;
	}

private:

	// ����
	static void Create()
	{
		instance_ = new T;
		SingletonFinalizer::AddFinalizer(&Singleton<T>::Destroy);

	}
	// �j��
	static void Destroy()
	{
		delete instance_;
		instance_ = nullptr;
	}

	static std::once_flag init_flag_;	// �Ăяo��
	static T *instance_;				// �C���X�^���X
};

template <typename T> std::once_flag Singleton<T>::init_flag_;
template <typename T> T *Singleton<T>::instance_ = nullptr;