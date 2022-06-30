#pragma once

#include <DirectXMath.h>

class DirectionalLight
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
		XMVECTOR lightv;
		XMFLOAT3 lightcolor;
		unsigned int active;
	};

public: // �����o�֐�
	//���C�g�������Z�b�g
	void SetLightDir(const XMVECTOR& lightdir) { this->lightdir = DirectX::XMVector3Normalize(lightdir); }
	//���C�g�������擾
	inline const XMVECTOR& GetLightDir() { return lightdir; }
	//���C�g�F���Z�b�g
	void SetLightColor(const XMFLOAT3& lightcolor) { this->lightcolor = lightcolor; }
	//���C�g�F���擾
	inline const XMFLOAT3& GetLightColor() { return lightcolor; }
	//�L���t���O���Z�b�g
	inline void SetActive(bool active) { this->active = active; }
	//�L���`�F�b�N
	inline bool IsActive() { return active; }

private: // �����o�ϐ�
	//���C�g�����i�P�ʃx�N�g���j
	XMVECTOR lightdir = { 1,0,0,0 };
	//���C�g�F
	XMFLOAT3 lightcolor = { 1,1,1 };
	//�L���t���O
	bool active = false;
};