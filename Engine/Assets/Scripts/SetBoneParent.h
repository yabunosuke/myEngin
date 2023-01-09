#pragma once
#include "Object/Component/Behaviour/MonoBehaviour.h"

class SetBoneParent :
    public MonoBehaviour
{
public:
    SetBoneParent(const std::string& boneName);
	void Start()override;

private:
    DirectX::XMFLOAT4X4 *hand_bone_ = nullptr;
    std::string bone_name_;
};

