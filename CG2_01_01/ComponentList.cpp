#include "ComponentList.h"
#include <algorithm>

std::map<std::string, void(*)(GameObject *)> ComponentList::components_;
std::map<ComponentList::Group, std::vector<std::string>> ComponentList::component_group_;

ComponentList *ComponentList::GetIns()
{
    static ComponentList *instance;
    return instance;
}

void ComponentList::Initialize()
{
    
    //Ží•Ê‚²‚Æ‚É‚Ü‚Æ‚ß
    component_group_[Group::Physics].emplace_back("Box Collider");
    component_group_[Group::Physics].emplace_back("Capsule Collider");
    component_group_[Group::Physics].emplace_back("Sphere Collider");


    component_group_[Group::Rendering].emplace_back("Camera");
    component_group_[Group::Rendering].emplace_back("Camera");
    component_group_[Group::Rendering].emplace_back("Light");
}

void ComponentList::ComponentGroupSet(Group group, std::string name)
{
    component_group_[group].emplace_back(name);
}

