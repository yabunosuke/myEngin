#include "LightManager.h"
#include "ConstantBufferManager/ConstantBufferManager.h"
#include <algorithm>


void LightManager::AddLight(std::weak_ptr<LightDate> light)
{
	light_list_.emplace_back(light);
}


void LightManager::BufferTransfer(
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
	UINT subresource,
	UINT rootparameta_index
)
{
	// �L���Ȃ��̂�����O�Ƀ\�[�g���Čv�Z�������炷
	std::sort(light_list_.begin(),light_list_.end(), active);
	
	// �S�Ẵ��C�g�f�[�^��ϊ����Ċi�[
	LightConstBufferData const_light_map;

	// ���C�g�̐���LIGHT_MAX�𒴂��Ă�����͈͓��������R�s�[
	if(light_list_.size() > LIGHT_MAX)
	{
		std::copy_n(
			light_list_.begin()->lock().get(),
			LIGHT_MAX,
			const_light_map.light);
	}
	// LIGHT_MAX�ȓ��Ȃ疖�[�܂ŃR�s�[
	else
	{
		int i = 0;
		for(; i < light_list_.size();++i)
		{
			const_light_map.light[i] = *light_list_[i].lock().get();
		}
		const_light_map.light[i + 1].is_active = false;
	}

	// �]��
	ConstantBufferManager::GetInstance()->BufferTransfer<LightConstBufferData>(cmd_list, subresource, rootparameta_index, BufferName::Light, &const_light_map);

}

bool LightManager::active(const std::weak_ptr<LightDate> &lhs, const std::weak_ptr<LightDate> &rhs)
{
	return lhs.lock()->is_active < rhs.lock()->is_active;
}