#pragma once
#include "Object/Component/Behaviour.h"
#include "Collision/Collision.h"
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

    void ComponentInitialize() override;
    void ComponentFixedUpdate() override;
    void ComponentUpdate() override;


    virtual void Reset() {};
    virtual void Awake(){};
    virtual void Start() {};
    virtual void FixedUpdate(){};
    virtual void Update() {};
    virtual void LateUpdate(){};


    /// <summary>
    /// 触れた瞬間に呼び出される
    /// </summary>
    virtual void OnCollisionEnter(Collision &collision){};
    /// <summary>
    /// 離れた瞬間に呼び出される
    /// </summary>
    virtual void OnCollisionExit(Collision &collision) {};
    /// <summary>
    /// 触れている間呼び出される
    /// </summary>
    virtual void OnCollisionStay(Collision &collision) {};

    /// <summary>
    /// 触れた瞬間に呼び出される
    /// </summary>
    virtual void OnTriggerEnter(Collider &other) {};
    /// <summary>
    /// 離れた瞬間に呼び出される
    /// </summary>
    virtual void OnTriggerExit(Collider &other) {};
    /// <summary>
    /// 振れている間呼び出される
    /// </summary>
    virtual void OnTriggerStay(Collider &other) {};
private:
    bool do_start_ = true;
};

