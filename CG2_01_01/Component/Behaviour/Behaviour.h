#pragma once
#include "Object/Component/Component.h"
#include "Property.h"

class Behaviour :
    public Component
{
public:

private:
    bool enabled_;      // �X�V�t���O


public: //�A�N�Z�b�T

    /// <summary>
    /// �A�^�b�`���ꂽ�Q�[���I�u�W�F�N�g���L�����R���|�[�l���g���L����
    /// </summary>
    Property<bool>isActiveAndEnabled{
    	enabled_ ,AccessorType::ReadOnly,
        [this]() {return enabled_; },
    };
};

