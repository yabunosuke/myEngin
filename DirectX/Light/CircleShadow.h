#pragma once

#include <DirectXMath.h>

class CircleShadow
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	// �萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMVECTOR dir;
		XMFLOAT3 casterPos;
		float distanceCasterLight;
		XMFLOAT3 atten;
		float pad3;
		XMFLOAT2 factorAngleCos;
		unsigned int active;
		float pad4;
	};

public: // �����o�֐�
	// �������Z�b�g
	inline void SetDir(const XMVECTOR& dir) { this->dir = DirectX::XMVector3Normalize(dir); }
	// ���C�g�������擾
	inline const XMVECTOR& GetDir() { return dir; }
	// �L���X�^�[���W���Z�b�g
	inline void SetCasterPos(const XMFLOAT3& casterPos) { this->casterPos = casterPos; }
	// �L���X�^�[���W���擾
	inline const XMFLOAT3& GetCasterPos() { return casterPos; }
	// �L���X�^�[�ƃ��C�g�̋������Z�b�g
	inline void SetDistanceCasterLight(float distanceCasterLight) { this->distanceCasterLight = distanceCasterLight; }
	// �L���X�^�[�ƃ��C�g�̋������擾
	inline float GetDistanceCasterLight() { return distanceCasterLight; }
	// ���������W�����Z�b�g
	inline void SetAtten(const XMFLOAT3& atten) { this->atten = atten; }
	// ���������W�����擾
	inline const XMFLOAT3& GetAtten() { return atten; }
	// �����p�x���Z�b�g
	inline void SetFactorAngle(const XMFLOAT2& factorAngle)
	{
		this->factorAngleCos.x = cosf(DirectX::XMConvertToRadians(factorAngle.x));
		this->factorAngleCos.y = cosf(DirectX::XMConvertToRadians(factorAngle.y));
	}
	// �����p�x���擾
	inline const XMFLOAT2& GetFactorAngleCos() { return factorAngleCos; }
	// �L���t���O���Z�b�g
	inline void SetActive(bool active) { this->active = active; }
	// �L���`�F�b�N
	inline bool IsActive() { return active; }

private: // �����o�ϐ�
	// �����i�P�ʃx�N�g���j
	XMVECTOR dir = { 1,0,0,0 };
	// �L���X�^�[�ƃ��C�g�̋���
	float distanceCasterLight = 100.0f;
	// �L���X�^�[���W�i���[���h���W�n�j
	XMFLOAT3 casterPos = { 0,0,0 };
	// ���������W��
	XMFLOAT3 atten = { 0.5f, 0.6f, 0.0f };
	// �����p�x
	XMFLOAT2 factorAngleCos = { 0.2f, 0.5f };
	// �L���t���O
	bool active = false;
};