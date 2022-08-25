#pragma once
#include "Component/Behaviour/Behaviour.h"

class MonoBehaviour :
    public Behaviour
{
public:
    // ���z�֐�

    /// <summary>
    /// �X�N���v�g�̃C���X�^���X�����[�h���ꂽ���ɌĂяo�����
    /// </summary>
    void Awake();
    void Update();
    void FixedUpdate();
    void LateUpdate();

	void OnDisable();
    void OnEnable();


    /// <summary>
    /// �Q�[���I�u�W�F�N�g���m���ڐG�����^�C�~���O�Ŏ��s
    /// </summary>
    void OnCollisionEnter();
    /// <summary>
    /// �Q�[���I�u�W�F�N�g���m���ڐG���Ă���ԁA�����I�Ɏ��s
    /// </summary>
    void OnCollisionStay();
    /// <summary>
    /// �Q�[���I�u�W�F�N�g���m���ڐG���痣�ꂽ�^�C�~���O�Ŏ��s
    /// </summary>
    void OnCollisionExit();


    /// <summary>
    /// �Q�[���I�u�W�F�N�g���m���ڐG�����^�C�~���O�Ŏ��s
    /// </summary>
    void OnTriggerEnter();
    /// <summary>
    /// �Q�[���I�u�W�F�N�g���m���ڐG���Ă���ԁA�����I�Ɏ��s
    /// </summary>
    void OnTriggerStay();
    /// <summary>
    /// �Q�[���I�u�W�F�N�g���m���ڐG���痣�ꂽ�^�C�~���O�Ŏ��s
    /// </summary>
    void OnTriggerExit();




};

