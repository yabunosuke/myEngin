#include "LightManager.h"
#include "Component/LightComponent.h"

void LightManager::AddLight(std::shared_ptr<LightComponent> light)
{
	light_list_.emplace_back(light);
}
