#include "LightManager.h"
#include <algorithm>
#include "../Engine/Object/Component/Camera.h"


void LightManager::AddLight(LightDate *light)
{
	light_list_.emplace_back(light);
}



void LightManager::BufferTransfer(
	Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list,
	UINT subresource,
	UINT rootparameta_index
)
{
	// 有効なものだけ手前にソートして計算数を減らす
	std::sort(light_list_.begin(),light_list_.end(), active);
	
	// 全てのライトデータを変換して格納
	LightConstBufferData const_light_map;

	// ライトの数がLIGHT_MAXを超えていたら範囲内だけをコピー
	if(light_list_.size() > LIGHT_MAX)
	{
		std::copy_n(
			light_list_[0],
			LIGHT_MAX,
			const_light_map.light);
	}
	// LIGHT_MAX以内なら末端までコピー
	else
	{
		int i = 0;
		for(; i < light_list_.size();++i)
		{
			const_light_map.light[i] = *light_list_[i];
		}
		const_light_map.light[i + 1].is_active = false;
	}
	Vector3 cam_pos = Camera::main.r_->transform_->position;
	const_light_map.eye_pos =
	{
		cam_pos.x,
		cam_pos.y,
		cam_pos.z,
		0
	};
	// 転送
	ConstantBufferManager::GetInstance()->BufferTransfer<LightConstBufferData>(cmd_list, subresource, rootparameta_index, BufferName::Light, &const_light_map);

}

bool LightManager::active(const LightDate *lhs, const LightDate *rhs)
{
	return lhs->is_active < rhs->is_active;
}
