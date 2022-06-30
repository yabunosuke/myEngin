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
    //いったんVectorに追加
    std::vector<std::pair<std::string, void(*)(GameObject *)>> vec;
    vec.push_back({ "Transform" , addTransform });
    vec.push_back({ "Light" ,     addLight});
    vec.push_back({ "Camera" ,    addCamera });


    vec.push_back({ "Box Collider" ,    addDummy });
    vec.push_back({ "Capsulue Collider" ,    addDummy });
    vec.push_back({ "Character Collider" ,    addDummy });
    vec.push_back({ "Mesh Collider" ,    addDummy });
    vec.push_back({ "Sphere Collider" ,    addDummy });
    
    //昇順に並べ替え
    std::sort(vec.begin(), vec.end());

    //マップに追加
    for (auto &v : vec) {
        componentList[v.first] = v.second;
    }
    
    //種別ごとにまとめ
    componentGroup[Group::Rendering].emplace_back("Camera");
    componentGroup[Group::Rendering].emplace_back("Light");
}

