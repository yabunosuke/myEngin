#include "Fbx.h"
#include "PipelineManager.h"
#include "Camera.h"

Fbx::Fbx(ID3D12Device *dev, const char *file_path)
{
	resource_ = std::make_shared<FbxReader>();
	// ��Ń��f���}�l�[�W���[�ɍ����ւ���
	resource_->LoadModel(dev, file_path);

	// �m�[�h�̎擾
	const std::vector<FbxResource::Node> &res_nodes = resource_->GetNodes();
	
	nodes_.resize(res_nodes.size());
	for (int node_index = 0; node_index < nodes_.size(); +node_index) {
		auto &&src = res_nodes.at(node_index);	// ���f�[�^
		auto &&dst = nodes_.at(node_index);		// �R�s�[��

		dst.name = src.name.c_str();
		dst.parent = src.parent_index >= 0 ? &nodes_.at(src.parent_index) : nullptr;
		dst.scale = src.scaling;
		dst.rotate = src.rotation;
		dst.translate = src.translation;

		// �e�m�[�h�����Ȃ����
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

	// �V�[���萔�o�b�t�@�̐���
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
		// �m�[�h�ɑS�̂̃g�����X�t�H�[�����v�Z
		DirectX::XMMATRIX S = DirectX::XMMatrixScaling(node.scale.x, node.scale.y, node.scale.z);
		DirectX::XMMATRIX R = DirectX::XMMatrixRotationQuaternion(DirectX::XMLoadFloat4(&node.rotate));
		DirectX::XMMATRIX T = DirectX::XMMatrixTranslation(node.translate.x, node.translate.y, node.translate.z);
		DirectX::XMMATRIX local_transform = S * R * T;

		// �e�̍s����v�Z
		DirectX::XMMATRIX parent_transform;
		if (node.parent != nullptr) {
			parent_transform = DirectX::XMLoadFloat4x4(&node.parent->world_transform);
		}
		// �e�����Ȃ����transform��n��
		else {
			parent_transform = node_transform;
		}

		DirectX::XMMATRIX world_transform = local_transform * parent_transform;

		// �v�Z���ʂ𔽉f
		DirectX::XMStoreFloat4x4(&node.local_transform, local_transform);
		DirectX::XMStoreFloat4x4(&node.world_transform, world_transform);
	}
}

void Fbx::Draw(ComPtr<ID3D12Device> dev, ComPtr<ID3D12GraphicsCommandList> cmd_list)
{
	HRESULT result;
	// �V�[���萔�o�b�t�@���Z�b�g
	cmd_list->SetGraphicsRootConstantBufferView(0, scene_constant_buffer_->GetGPUVirtualAddress());
	SceneConstantBuffer *scene_constant_buffer_map = nullptr;
	result = scene_constant_buffer_->Map(0, nullptr, (void **)&scene_constant_buffer_map);
	if (SUCCEEDED(result))
	{
		//���ŌÂ��J�����N���X���g�p����
		DirectX::XMStoreFloat4x4(&scene_constant_buffer_map->view_projection, Camera::GetCam()->GetViewProjectionMatrix());
		scene_constant_buffer_map->light_direction = { 0,0.5,0.5, 0 };
		scene_constant_buffer_->Unmap(0, nullptr);
	}

	// ���b�V���񐔕���������
	for (const FbxResource::Mesh &mesh : resource_->GetMeshes()) {
		
		// �p�C�v���C���X�e�[�g�ƃ��[�g�V�O�l�`���ݒ�
		PipelineManager::GetInstance()->SetPipline(cmd_list,mesh.pipline_name);

		// ���b�V���萔�o�b�t�@���Z�b�g
		cmd_list->SetGraphicsRootConstantBufferView(1, mesh.mesh_constant_buffer_->GetGPUVirtualAddress());
		// ���b�V���萔�o�b�t�@�X�V
		FbxResource::MeshConstantBuffer *mesh_constant_buffer_map = nullptr;
		result = mesh.mesh_constant_buffer_->Map(0, nullptr, (void **)&mesh_constant_buffer_map);
		if (SUCCEEDED(result))
		{
			// �e��1�ȏ゠��ΐe����v�Z
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
			// �e�����Ȃ���΃��[���h�g�����X�t�H�[�����g��
			else
			{
				mesh_constant_buffer_map->bone_transforms[0] = nodes_.at(mesh.id).world_transform;
			}
			
			mesh.mesh_constant_buffer_->Unmap(0, nullptr);
		}


		// ���_�o�b�t�@���Z�b�g(VBV)
		cmd_list->IASetVertexBuffers(0, 1, &mesh.vbView);
		// �C���f�b�N�X�o�b�t�@���Z�b�g(IBV)
		cmd_list->IASetIndexBuffer(&mesh.ibView);
		// �v���~�e�B�u�`���ݒ�
		cmd_list->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		// �T�u�Z�b�g�P�ʂŕ`��
		for (const FbxResource::Subset &subset : mesh.subsets) {
			// �T�u�Z�b�g�萔�o�b�t�@�r���[��0�ԂɃZ�b�g
			cmd_list->SetGraphicsRootConstantBufferView(2, subset.subset_constant_buffer_->GetGPUVirtualAddress());

			FbxResource::SubsetConstantBuffer *subset_constant_buffer_map = nullptr;
			// ���b�V���萔�o�b�t�@�X�V
			result = subset.subset_constant_buffer_->Map(0, nullptr, (void **)&subset_constant_buffer_map);
			if (SUCCEEDED(result))
			{
				subset_constant_buffer_map->color = subset.material->color;
				subset.subset_constant_buffer_->Unmap(0, nullptr);
			}
			
			

			// �V�F�[�_���\�[�X�r���[���Z�b�g
			cmd_list->SetGraphicsRootDescriptorTable(3, *subset.material->shader_resource_views[0]);

			//�`��R�}���h
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

