#pragma once
#include "Object/Component/Behaviour/Behaviour.h"

class MonoBehaviour :
    public Behaviour
{
public:

    //===========================================
    //
    //		コンストラクタ
    //
    //===========================================

    MonoBehaviour(const std::string &name);

    void ComponentFixedUpdate() override;
    void ComponentUpdate() override;


    virtual void Reset() {};
    virtual void Awake(){};
    virtual void Start() {};
    virtual void FixedUpdate(){};
    virtual void Update() {};
    virtual void LateUpdate(){};

private:
    bool do_start_ = true;
};

