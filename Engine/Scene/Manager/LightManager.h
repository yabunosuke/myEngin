#pragma once
#include <vector>
#include <memory>
#include <wrl/client.h>
#include <d3d12.h>
#include "ConstantBufferManager/ConstantBufferManager.h"


class LightManager
{
public:

	// ���C�g�����X�g�ɒǉ�
	void AddLight(LightDate *light);
	/// <summary>
	/// ���C�g���o�b�t�@�ɓ]��
	/// </summary>
	void BufferTransfer(
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index
		);

	std::vector<LightDate *> &GetLightList() { return light_list_; }
private:
	// �\�[�g�p�֐�
	static bool active(const LightDate *lhs, const LightDate *rhs);


	std::vector<LightDate *> light_list_;
};

