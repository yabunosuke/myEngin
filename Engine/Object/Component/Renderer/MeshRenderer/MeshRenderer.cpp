#include "MeshRenderer.h"

MeshRenderer::MeshRenderer():
	Renderer()
{
	// 名前をメッシュレンダラーに変更
	name = "MeshRenderer";
}

void MeshRenderer::Infomation()
{
	// マテリアル設定
	if (ImGui::TreeNode("Mesh"))
	{
		ImGui::TreePop();
	}
	// マテリアル設定
	if (ImGui::TreeNode("Materials")) 
	{
		ImGui::TreePop();
	}
	// ライティング設定
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

	// 描画キューに追加
}
