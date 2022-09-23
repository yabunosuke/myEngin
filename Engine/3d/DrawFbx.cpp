#include "DrawFbx.h"
#include "PipelineManager.h"
#include "ConstantBufferManager/ConstantBufferManager.h"
#include "oldTexture.h"

std::vector<std::shared_ptr<FbxResource>>	Renderer::deferred_shading_datas_;
std::vector<std::shared_ptr<FbxResource>>	Renderer::forward_shading_datas_;
std::vector<std::vector<Fbx::Node>>			Renderer::nodes_;

Renderer *Renderer::GetIns()
{
	static Renderer *instans;
	return instans;
}

void Renderer::SetDrawResource(std::weak_ptr<FbxResource> resorces, std::vector<Fbx::Node> nodes)
{
	deferred_shading_datas_.emplace_back(resorces);
	nodes_.emplace_back(nodes);
}

void Renderer::DrawDeferred(Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{

	int draw_num = 0;
	HRESULT result = S_OK;
	for(const auto &resouce : deferred_shading_datas_)
	{
		// ���b�V���񐔕���������
		for (const FbxResource::Mesh &mesh : resouce->GetMeshes()) {

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
						DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes_[draw_num].at(mesh.node_indices.at(i)).world_transform);
						DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.offset_transforms.at(i));
						DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
						DirectX::XMStoreFloat4x4(&mesh_constant_buffer_map->bone_transforms[i], boneTransform);
					}
				}
				// �e�����Ȃ���΃��[���h�g�����X�t�H�[�����g��
				else
				{
					mesh_constant_buffer_map->bone_transforms[0] = nodes_[draw_num].at(mesh.id).world_transform;
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
					subset_constant_buffer_map->material_color = {
						subset.material->color.x,
						subset.material->color.y,
						subset.material->color.z,
						subset.material->color.w
					};
					subset.subset_constant_buffer_->Unmap(0, nullptr);
				}


				// �f�X�N���v�^�q�[�v�̃Z�b�g
				ID3D12DescriptorHeap *ppHeaps[] = { oldTexture::descriptor_heap_.Get() };
				cmd_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

				// �V�F�[�_���\�[�X�r���[���Z�b�g
				cmd_list->SetGraphicsRootDescriptorTable(3, subset.material->shader_resource_views[0]);

				//�`��R�}���h
				cmd_list->DrawIndexedInstanced(subset.index_count, 1, subset.start_index, 0, 0);
			}

		}
		++draw_num;
	}

	deferred_shading_datas_.clear();
	nodes_.clear();

}

void Renderer::DrawForward(Microsoft::WRL::ComPtr<ID3D12Device> dev, Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> cmd_list)
{

	int draw_num = 0;
	HRESULT result = S_OK;
	for (const auto &resouce : forward_shading_datas_)
	{
		// ���b�V���񐔕���������
		for (const FbxResource::Mesh &mesh : resouce->GetMeshes()) {

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
						DirectX::XMMATRIX worldTransform = DirectX::XMLoadFloat4x4(&nodes_[draw_num].at(mesh.node_indices.at(i)).world_transform);
						DirectX::XMMATRIX offsetTransform = DirectX::XMLoadFloat4x4(&mesh.offset_transforms.at(i));
						DirectX::XMMATRIX boneTransform = offsetTransform * worldTransform;
						DirectX::XMStoreFloat4x4(&mesh_constant_buffer_map->bone_transforms[i], boneTransform);
					}
				}
				// �e�����Ȃ���΃��[���h�g�����X�t�H�[�����g��
				else
				{
					mesh_constant_buffer_map->bone_transforms[0] = nodes_[draw_num].at(mesh.id).world_transform;
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
					subset_constant_buffer_map->material_color = {
						subset.material->color.x,
						subset.material->color.y,
						subset.material->color.z,
						subset.material->color.w
					};
					subset.subset_constant_buffer_->Unmap(0, nullptr);
				}


				// �f�X�N���v�^�q�[�v�̃Z�b�g
				ID3D12DescriptorHeap *ppHeaps[] = { oldTexture::descriptor_heap_.Get() };
				cmd_list->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

				// �V�F�[�_���\�[�X�r���[���Z�b�g
				cmd_list->SetGraphicsRootDescriptorTable(3, subset.material->shader_resource_views[0]);

				//�`��R�}���h
				cmd_list->DrawIndexedInstanced(subset.index_count, 1, subset.start_index, 0, 0);
			}

		}
		++draw_num;
	}

	deferred_shading_datas_.clear();
	nodes_.clear();

}
