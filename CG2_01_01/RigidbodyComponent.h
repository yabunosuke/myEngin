#pragma once
#include "Component.h"
#include <DirectXMath.h>

struct Regidbody {
    float mass;         // ����
    float drag;         // �͂œ����ꍇ�̋�C��R
    float angular_drag; // �g���N�œ����ꍇ�̋�C��R
    bool use_gravity;   // �d�͂��g�p���邩

    struct FreezePosition { // ���[���h���W�̎w�肵�����Ń��W�b�h�{�f�B�̈ړ����~
        bool x;
        bool y;
        bool z;
    }freeze_position;

    struct FreezeRotation { // ���[�J�����W�̎w�肵�����Ń��W�b�h�{�f�B�̉�]���~
        bool x;
        bool y;
        bool z;
    }freeze_rotation;
};

class RigidbodyComponent :
    public Component
{
public:

private:
    // ���W�b�h�{�f�B
    Regidbody regidody;

};

