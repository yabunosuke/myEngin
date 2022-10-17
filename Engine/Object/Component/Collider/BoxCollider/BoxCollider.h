#pragma once
#include "../Engine/Collision/Primitive.h"
#include "../Collider.h"
class BoxCollider final :
    public Collider,
    public yEngine::OBB
{
public:
    BoxCollider(Quaternion q = {0.0f,1.0f,0.0f,0.0f}, Vector3 extent = { 1.0f, 1.0f, 1.0f }, Vector3 center = { 0.0f,0.0f,0.0f });
	/// <summary>
	/// ������
	/// </summary>
    void ComponentInitialize() override;
    void ComponentUpdate() override;
private:
    Vector3 local_center_{ 0,0,0 };   // ���[�J���̒���
    Vector3 local_extent_{ 1.0f,1.0f,1.0f };   // ���[�J���̒���
};

