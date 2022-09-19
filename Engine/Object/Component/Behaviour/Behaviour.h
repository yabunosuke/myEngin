#pragma once
#include "../Component.h"
class Behaviour :
    public Component
{
public:

    //===========================================
    //
    //		�R���X�g���N�^
    //
    //===========================================

    Behaviour(const std::string &name, ComponentType component_type = ComponentType::Behaviour);
	void ImGuiDraw() override;

private:
    // �X�V�t���O
    bool enabled_;
    bool is_active_;
};

