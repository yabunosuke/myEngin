#pragma once
#include "Component/Behaviour/Behaviour.h"

class MonoBehaviour :
    public Behaviour
{
public:
    // 仮想関数

    /// <summary>
    /// スクリプトのインスタンスがロードされた時に呼び出される
    /// </summary>
    void Awake();
    void Update();
    void FixedUpdate();
    void LateUpdate();

	void OnDisable();
    void OnEnable();


    /// <summary>
    /// ゲームオブジェクト同士が接触したタイミングで実行
    /// </summary>
    void OnCollisionEnter();
    /// <summary>
    /// ゲームオブジェクト同士が接触している間、持続的に実行
    /// </summary>
    void OnCollisionStay();
    /// <summary>
    /// ゲームオブジェクト同士が接触から離れたタイミングで実行
    /// </summary>
    void OnCollisionExit();


    /// <summary>
    /// ゲームオブジェクト同士が接触したタイミングで実行
    /// </summary>
    void OnTriggerEnter();
    /// <summary>
    /// ゲームオブジェクト同士が接触している間、持続的に実行
    /// </summary>
    void OnTriggerStay();
    /// <summary>
    /// ゲームオブジェクト同士が接触から離れたタイミングで実行
    /// </summary>
    void OnTriggerExit();




};

