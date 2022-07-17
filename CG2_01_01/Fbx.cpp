#include "Fbx.h"
#include "PipelineManager.h"
#include "Camera.h"

Fbx::Fbx(ID3D12Device *dev, const char *file_path)
{
	resource_ = std::make_shared<FbxReader>();
	// 後でモデルマネージャーに差し替える
	resource_->LoadModel(dev, file_path);

	// ノードの取得
	const std::vector<FbxResource::Node> &res_nodes = resource_->GetNodes();
	
	nodes_.resize(res_nodes.size());
	for (int node_index = 0; node_index < nodes_.size(); +node_index) {
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

	// シーン定数バッファの生成
	HRESULT result;
	result = dev->CreateCommittedResource
	(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer((sizeof(SceneConstantBuffer) + 0xff) & ~0xff),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&scene_constant_buffer_)
	);

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
	HRESULT result;
	// シーン定数バッファをセット
	cmd_list->SetGraphicsRootConstantBufferView(0, scene_constant_buffer_->GetGPUVirtualAddress());
	SceneConstantBuffer *scene_constant_buffer_map = nullptr;
	result = scene_constant_buffer_->Map(0, nullptr, (void **)&scene_constant_buffer_map);
	if (SUCCEEDED(result))
	{
		//仮で古いカメラクラスを使用する
		DirectX::XMStoreFloat4x4(&scene_constant_buffer_map->view_projection, Camera::GetCam()->GetViewProjectionMatrix());
		scene_constant_buffer_map->light_direction = { 0,0.5,0.5, 0 };
		scene_constant_buffer_->Unmap(0, nullptr);
	}

	// メッシュ回数分処理する
	for (const FbxResource::Mesh &mesh : resource_->GetMeshes()) {
		
		// パイプラインステートとルートシグネチャ設定
		PipelineManager::GetInstance()->SetPipline(cmd_list,mesh.pipline_name);

		// メッシュ定数バッファをセット
		cmd_list->SetGraphicsRootConstantBufferView(1, mesh.mesh_constant_buffer_->GetGPUVirtualAddress());
		// メッシュ定数バッファ更新
		FbxResource::MeshConstantBuffer *mesh_constant_buffer_map = nullptr;
		result = mesh.mesh_constant_buffer_->Map(0, nullptr, (void **)&mesh_constant_buffer_map);
		if (SUCCEEDED(result))
		{
			// 親の1つ以上あれば親から計算
			if (mesh.node_indices.size() > 0)
			{
				for (size_t i = 0; i < mesh.node_indices.size(); ++i)
				{
					DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes_.at(mesh.node_indices.at(i)).world_transform);
					DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.offset_transforms.at(i));
					DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
					DirectX::XMStoreFloat4x4(&mesh_constant_buffer_map->bone_transforms[i], boneTransform);
				}
			}
			// 親がいなければワールドトランスフォームを使う
			else
			{
				mesh_constant_buffer_map->bone_transforms[0] = nodes_.at(mesh.id).world_transform;
			}
			
			mesh.mesh_constant_buffer_->Unmap(0, nullptr);
		}


		// 頂点バッファをセット(VBV)
		cmd_list->IASetVertexBuffers(0, 1, &mesh.vbView);
		// インデックスバッファをセット(IBV)
		cmd_list->IASetIndexBuffer(&mesh.ibView);
		// プリミティブ形状を設定
		cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// サブセット単位で描画
		for (const FbxResource::Subset &subset : mesh.subsets) {
			// サブセット定数バッファビューを0番にセット
			cmd_list->SetGraphicsRootConstantBufferView(2, subset.subset_constant_buffer_->GetGPUVirtualAddress());

			FbxResource::SubsetConstantBuffer *subset_constant_buffer_map = nullptr;
			// メッシュ定数バッファ更新
			result = subset.subset_constant_buffer_->Map(0, nullptr, (void **)&subset_constant_buffer_map);
			if (SUCCEEDED(result))
			{
				subset_constant_buffer_map->color = subset.material->color;
				subset.subset_constant_buffer_->Unmap(0, nullptr);
			}
			
			

			// シェーダリソースビューをセット
			cmd_list->SetGraphicsRootDescriptorTable(3, *subset.material->shader_resource_views[0]);

			//描画コマンド
			cmd_list->DrawIndexedInstanced(subset.index_count, 1, subset.start_index, 0, 0);
		}

	}

}

Fbx::Node *Fbx::FindNode(const char *name)
{
	for (auto &node : nodes_) {
		if (strcmp(node.name, name) == 0) return &node;
	}
	return nullptr;
}

