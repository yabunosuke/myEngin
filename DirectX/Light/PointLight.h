#pragma once

#include <DirectXMath.h>

class PointLight
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT3 lightpos;
		float pad1;
		XMFLOAT3 lightcolor;
		float pad2;
		XMFLOAT3 lightatten;
		unsigned int active;
	};

public: // �����o�֐�
	//���C�g���W���Z�b�g
	inline void SetLightPos(const XMFLOAT3& lightpos) { this->lightpos = lightpos; }
	//���C�g���W���擾
	inline const XMFLOAT3& GetLightPos() { return lightpos; }
	//���C�g�F���Z�b�g
	inline void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }
	//���C�g�F���擾
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }
	//���C�g���������W�����Z�b�g
	inline void SetLightAtten(const XMFLOAT3& lightAtten) { this->lightAtten = lightAtten; }
	//���C�g���������W�����擾
	inline const XMFLOAT3& GetLightAtten() { return lightAtten; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool active) { this->active = active; }
	//�L���`�F�b�N
	inline bool IsActive() { return active; }

private: // �����o�ϐ�
	//���C�g���W�i���[���h���W�n�j
	XMFLOAT3 lightpos = { 0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//���C�g���������W��
	XMFLOAT3 lightAtten = { 1.0f, 1.0f, 1.0f };
	//�L���t���O
	bool active = false;
};