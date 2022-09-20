#pragma once
#include "Object/Component/Behaviour/Behaviour.h"
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
    virtual void OnTriggerEnter(Collision collision) {};
    /// <summary>
    /// 離れた瞬間に呼び出される
    /// </summary>
    virtual void OnTriggerExit(Collision collision) {};
    /// <summary>
    /// 振れている間呼び出される
    /// </summary>
    virtual void OnTriggerStay(Collision collision) {};
private:
    bool do_start_ = true;
};

