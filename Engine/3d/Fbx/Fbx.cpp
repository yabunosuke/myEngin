#include "Fbx.h"
#include "PipelineManager.h"
#include "Texture.h"
#include "ConstantBufferManager/ConstantBufferManager.h"
#include "3d/DrawFbx.h"

Fbx::Fbx(ID3D12Device *dev, const char *file_path)
{
	resource_ = std::make_shared<FbxResource>();
	// 後でモデルマネージャーに差し替える
	resource_->LoadModel(dev, file_path);

	// ノードの取得
	const std::vector<FbxResource::Node> &res_nodes = resource_->GetNodes();
	
	nodes_.resize(res_nodes.size());
	for (int node_index = 0; node_index < nodes_.size(); ++node_index) {
		auto &&src = res_nodes.at(node_index);	// 元データ
		auto &&dst = nodes_.at(node_index);		// コピー先

		dst.name = src.name.c_str();
		dst.parent = src.parent_index >= 0 ? &nodes_.at(src.parent_index) : nullptr;
		dst.scale = src.scaling;
		dst.rotate = src.rotation;
		dst.translate = src.translation;

		// 親ノードがいなければ
		if (dst.parent != nullptr)
		{
			dst.parent->children.emplace_back(&dst);
		}
	}
	const XMFLOAT4X4 transform = {
		1, 0, 0, 0,
		0, 1, 0, 0,
		0, 0, 1, 0,
		0, 0, 0, 1
	};

	UpdateTransform(transform);
}

void Fbx::UpdateTransform(const XMFLOAT4X4 &transform)
{
	DirectX::XMMATRIX node_transform = DirectX::XMLoadFloat4x4(&transform);

	for (Node &node : nodes_) {
		// ノードに全体のトランスフォームを計算
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX local_transform = S * R * T;

		// 親の行列を計算
		DirectX::XMMATRIX parent_transform;
		if (node.parent != nullptr) {
			parent_transform = DirectX::XMLoadFloat4x4(&node.parent->world_transform);
		}
		// 親がいなければtransformを渡す
		else {
			parent_transform = node_transform;
		}

		DirectX::XMMATRIX world_transform = local_transform * parent_transform;

		// 計算結果を反映
		DirectX::XMStoreFloat4x4(&node.local_transform, local_transform);
		DirectX::XMStoreFloat4x4(&node.world_transform, world_transform);
	}
}

void Fbx::Draw(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	// 後から描画
	Renderer::GetIns()->SetDrawResource(resource_, nodes_);
}

Fbx::Node *Fbx::FindNode(const char *name)
{
	for (auto &node : nodes_) {
		if (strcmp(node.name, name) == 0) return &node;
	}
	return nullptr;
}

void Fbx::UpdateAnimation(float elapsed_time)
{
	// アニメーションしていない間は更新しない
	if (IsPlayAnimation() == false) return;

	float blend_rate = 1.0f;
	if (animation_blend_time_ < animation_blend_seconds_)
	{
		blend_rate = animation_blend_time_ / animation_blend_seconds_;
		animation_blend_time_ += elapsed_time;
	}
	else
	{
		animation_blend_time_ = 0.0f;
		animation_blend_seconds_ = 0.0f;
	}

	const std::vector<FbxResource::Animation> &animations = resource_->GetAnimations();
	const FbxResource::Animation &animation = animations.at(current_animation_index_);

	const std::vector<FbxResource::Keyframe> &keyframes = animation.keyframes; 
	int keyCount = static_cast<int>(keyframes.size());
	for (int keyIndex = 0; keyIndex < keyCount - 1; ++keyIndex)
	{
		const FbxResource::Keyframe &keyframe0 = keyframes.at(keyIndex);
		const FbxResource::Keyframe &keyframe1 = keyframes.at(keyIndex + 1);
		if (current_animation_seconds_ >= keyframe0.seconds &&
			current_animation_seconds_ < keyframe1.seconds)
		{
			
			float rate = (current_animation_seconds_ - keyframe0.seconds) / (keyframe1.seconds - keyframe0.seconds);
			
			int nodeCount = static_cast<int>(nodes_.size());
			for (int nodeIndex = 0; nodeIndex < nodeCount; ++nodeIndex)
			{
				
				const FbxResource::NodeKeyData &key0 = keyframe0.nodeKeys.at(nodeIndex);
				const FbxResource::NodeKeyData &key1 = keyframe1.nodeKeys.at(nodeIndex);

				Node &node = nodes_[nodeIndex];


				DirectX::XMVECTOR Scale1 = DirectX::XMLoadFloat3(&key1.scale);
				DirectX::XMVECTOR Rotate1 = DirectX::XMLoadFloat4(&key1.rotate);
				DirectX::XMVECTOR Translate1 = DirectX::XMLoadFloat3(&key1.translate);


				if (blend_rate < 1.0f)
				{
					
					DirectX::XMVECTOR Scale0 = DirectX::XMLoadFloat3(&node.scale);
					DirectX::XMVECTOR Rotate0 = DirectX::XMLoadFloat4(&node.rotate);
					DirectX::XMVECTOR Translate0 = DirectX::XMLoadFloat3(&node.translate);

					
					DirectX::XMVECTOR Scale = DirectX::XMVectorLerp(Scale0, Scale1, blend_rate);

					
					DirectX::XMVECTOR Rotate = DirectX::XMQuaternionSlerp(Rotate0, Rotate1, blend_rate);

					
					DirectX::XMVECTOR Translate = DirectX::XMVectorLerp(Translate0, Translate1, blend_rate);

					
					DirectX::XMStoreFloat3(&node.scale, Scale);
					DirectX::XMStoreFloat4(&node.rotate, Rotate);
					DirectX::XMStoreFloat3(&node.translate, Translate);
				}
				
				else
				{
					
					DirectX::XMVECTOR Scale0 = DirectX::XMLoadFloat3(&key0.scale);
					DirectX::XMVECTOR Rotate0 = DirectX::XMLoadFloat4(&key0.rotate);
					DirectX::XMVECTOR Translate0 = DirectX::XMLoadFloat3(&key0.translate);

					

					DirectX::XMVECTOR Scale = DirectX::XMVectorLerp(Scale0, Scale1, rate);

					
					DirectX::XMVECTOR Rotate = DirectX::XMQuaternionSlerp(Rotate0, Rotate1, rate);

					
					DirectX::XMVECTOR Translate = DirectX::XMVectorLerp(Translate0, Translate1, rate);

					
					DirectX::XMStoreFloat3(&node.scale, Scale);
					DirectX::XMStoreFloat4(&node.rotate, Rotate);
					DirectX::XMStoreFloat3(&node.translate, Translate);
				}
			}
			break;
		}
	}

	if (animation_end_flag_ == true)
	{
		animation_end_flag_ = false;
		current_animation_index_ = -1;
		return;
	}

	
	current_animation_seconds_ += elapsed_time;

	
	if (current_animation_seconds_ >= animation.seconds_length)
	{
		if (animation_loop_flag_ == true)
		{
			current_animation_seconds_ -= animation.seconds_length;

		}
		else
		{
			animation_end_flag_ = true;
		}
	}
}

void Fbx::PlayAnimation(uint64_t index, bool loop, float blend_seconds)
{
	// 範囲外のアニメーションは行わない
	if (index == current_animation_index_) return;
	current_animation_index_ = index;
	current_animation_seconds_ = 0.0f;
	animation_loop_flag_ = loop;
	animation_end_flag_ = false;
	animation_blend_time_ = 0.0f;
	animation_blend_seconds_ = blend_seconds;
}

bool Fbx::IsPlayAnimation() const
{
	// 範囲内ならアニメーションしている
	if (current_animation_index_ < 0) return false;
	if (current_animation_index_ >= resource_->GetAnimations().size()) return false;
	return true;
}

