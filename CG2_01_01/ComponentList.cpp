#include "ComponentList.h"
#include <algorithm>

std::map<std::string, void(*)(GameObject *)> ComponentList::componentList;
std::map<ComponentList::Group, std::vector<std::string>> ComponentList::componentGroup;

ComponentList *ComponentList::GetIns()
{
    static ComponentList *instance;
    return instance;
}

void ComponentList::Initialize()
{
    //��������Vector�ɒǉ�
    std::vector<std::pair<std::string, void(*)(GameObject *)>> vec;
    vec.push_back({ "Transform" , addTransform });
    vec.push_back({ "Light" ,     addLight});
    vec.push_back({ "Camera" ,    addCamera });


    vec.push_back({ "Box Collider" ,    addDummy });
    vec.push_back({ "Capsulue Collider" ,    addDummy });
    vec.push_back({ "Character Collider" ,    addDummy });
    vec.push_back({ "Mesh Collider" ,    addDummy });
    vec.push_back({ "Sphere Collider" ,    addDummy });
    
    //�����ɕ��בւ�
    std::sort(vec.begin(), vec.end());

    //�}�b�v�ɒǉ�
    for (auto &v : vec) {
        componentList[v.first] = v.second;
    }
    
    //��ʂ��Ƃɂ܂Ƃ�
    componentGroup[Group::Rendering].emplace_back("Camera");
    componentGroup[Group::Rendering].emplace_back("Light");
}

