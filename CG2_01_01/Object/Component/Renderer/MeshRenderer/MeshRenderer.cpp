#include "MeshRenderer.h"

MeshRenderer::MeshRenderer():
	Renderer()
{
	// ���O�����b�V�������_���[�ɕύX
	name = "MeshRenderer";
}

void MeshRenderer::Infomation()
{
	// �}�e���A���ݒ�
	if (ImGui::TreeNode("Mesh"))
	{
		ImGui::TreePop();
	}
	// �}�e���A���ݒ�
	if (ImGui::TreeNode("Materials")) 
	{
		ImGui::TreePop();
	}
	// ���C�e�B���O�ݒ�
	if (ImGui::TreeNode("Lighting"))
	{
		ImGui::TreePop();
	}
}

void MeshRenderer::ComponentUpdate()
{
}

void MeshRenderer::ComponentDraw()
{

	// �`��L���[�ɒǉ�
}
