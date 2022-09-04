#pragma once

#include <vector>

#include "Renderer.h"
#include "Object/Material/Material.h"

class MeshRenderer :
    public Renderer
{
public:
	MeshRenderer();

	void Infomation() override;

	void ComponentUpdate() override;
	void ComponentDraw() override;


private:
	// マテリアルコンテナ
	std::vector<std::weak_ptr<Material>> materials_;

};

