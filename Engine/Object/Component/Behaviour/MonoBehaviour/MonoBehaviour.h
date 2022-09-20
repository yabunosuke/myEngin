#pragma once
#include "Object/Component/Behaviour/Behaviour.h"
#include "Collision/Collision.h"
class MonoBehaviour :
    public Behaviour
{
public:

    //===========================================
    //
    //		�R���X�g���N�^
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
    /// �G�ꂽ�u�ԂɌĂяo�����
    /// </summary>
    virtual void OnCollisionEnter(Collision &collision){};
    /// <summary>
    /// ���ꂽ�u�ԂɌĂяo�����
    /// </summary>
    virtual void OnCollisionExit(Collision &collision) {};
    /// <summary>
    /// �G��Ă���ԌĂяo�����
    /// </summary>
    virtual void OnCollisionStay(Collision &collision) {};

    /// <summary>
    /// �G�ꂽ�u�ԂɌĂяo�����
    /// </summary>
    virtual void OnTriggerEnter(Collision collision) {};
    /// <summary>
    /// ���ꂽ�u�ԂɌĂяo�����
    /// </summary>
    virtual void OnTriggerExit(Collision collision) {};
    /// <summary>
    /// �U��Ă���ԌĂяo�����
    /// </summary>
    virtual void OnTriggerStay(Collision collision) {};
private:
    bool do_start_ = true;
};

