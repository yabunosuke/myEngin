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
	// �}�e���A���R���e�i
	std::vector<std::weak_ptr<Material>> materials_;

};

