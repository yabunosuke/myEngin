#pragma once
#include <iostream>
#include <limits>

namespace Mathf
{

	// �C�v�V����
	inline static constexpr float epsilon{FLT_EPSILON};
	// �~����
	inline static const float pi = 3.14159265358979f;
	// ���̖���
	inline static const float infinity = std::numeric_limits<float>::infinity();
	// ���̖���
	inline static const float negative_infinity = -std::numeric_limits<float>::infinity();
	// �x���烉�W�A���֕ϊ�����萔
	inline static const float deg_to_rad = pi / 180.0f;
	// ���W�A������x�֕ϊ�����萔
	inline static const float rad_to_deg = 180.0f / pi;

	/// <summary>
	/// ��̒l�̍������e�͈͓��ɂ��邩�`�F�b�N����
	/// </summary>
	/// <param name="tolerance">���e�͈�</param>
	/// <returns>bool �ߎ�</returns>
	inline static bool Approximately(const float &lhs, const float &rhs, const float &tolerance = epsilon)
	{
		float difference = fabsf(lhs - rhs);
		if (difference < tolerance)
		{
			return true;
		}
		return false;
	}


	/// <summary>
	/// �l��͈͓��Ɏ��߂�
	/// </summary>
	/// <param name="value">��������l</param>
	/// <param name="min">�ŏ��l</param>
	/// <param name="max">�ő�l</param>
	/// <returns>float �v�Z����</returns>
	inline static float Clamp(const float &value, const float &min, const float &max)
	{
		if (value > max)
		{
			return max;
		}
		else if (value < min)
		{
			return min;
		}

		return value;
	};

	/// <summary>
	/// �ڕW�l�܂Ŏw�肵���ړ��ʂňړ�����B
	/// </summary>
	/// <param name="current">���ݒl</param>
	/// <param name="target">�ڕW�l</param>
	/// <param name="max_delta">�ړ���</param>
	/// <returns>float �ړ�����</returns>
	inline  static float MoveTowards(const float &current, const float &target, const float &max_delta)
	{
		if (current + max_delta < target)
		{
			return current + max_delta;
		}
		return  target;
	}

	/// <summary>
	/// ��l�̒�����ő�l��ԋp����
	/// </summary>
	/// <returns>float �ő�l</returns>
	inline static float Max(const float &lhs, const float &rhs)
	{
		return lhs > rhs ? lhs : rhs;
	}

	/// <summary>
	/// �����̒l�̒�����ő�l��ԋp����
	/// </summary>
	/// <returns>float �ő�l</returns>
	template<class  ...Args>
	inline static float Max(float a,float b,Args ...args)
	{
		float max = Max(a,b);
		for (float i : std::initializer_list<float>{ args... }) {
			max = Max(max, i);
		}
		return max;
	}


	/// <summary>
	/// ��l�̒�����ŏ��l��ԋp����
	/// </summary>
	/// <returns>float �ő�l</returns>
	inline static float Min(const float &lhs, const float &rhs)
	{
		return lhs < rhs ? lhs : rhs;
	}

	/// <summary>
	/// �����̒l�̒�����ŏ��l��ԋp����
	/// </summary>
	/// <returns>float �ő�l</returns>
	template<class  ...Args>
	inline static float Min(float a, float b, Args ...args)
	{
		float min = Min(a, b);
		for (float i : std::initializer_list<float>{ args... }) {
			min = Min(min,i);
		}
		return min;
	}

};