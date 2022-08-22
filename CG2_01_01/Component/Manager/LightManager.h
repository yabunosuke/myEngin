#pragma once
#include <vector>
#include <memory>
#include <wrl/client.h>
#include <d3d12.h>
#include "ConstantBufferManager/ConstantBufferManager.h"


class LightManager
{
public:

	// ���C�g�R���|�[�l���g�����X�g�ɒǉ�
	void AddLight(std::weak_ptr<LightDate> light);
	
	/// <summary>
	/// ���C�g���o�b�t�@�ɓ]��
	/// </summary>
	void BufferTransfer(
		Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
		UINT subresource,
		UINT rootparameta_index
		);

private:
	// �\�[�g�p�֐�
	static bool active(const std::weak_ptr<LightDate> &lhs, const std::weak_ptr<LightDate> &rhs);


	std::vector<std::weak_ptr<LightDate>> light_list_;
};

