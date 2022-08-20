#pragma once
#include <DirectXMath.h>
#include "Math/Vector3.h"
using namespace DirectX;

class Camera
{
public:
	Camera(Vector3 eye, Vector3 target = { 0,0,0 }, Vector3 up = { 0,1,0 });
	//ビュー行列更新
	void UpdateViewMatrix();
	//射影行列更新
	void UpdateProjectionMatrix();

	static void Initialize();
	static void SetCam(Camera *cam);
	//カメラを取得
	static Camera *GetCam() { return NowCamera; }

	//ビュー行列を取得
	const XMMATRIX &GetViewMatrix() { return matView; }
	//射影行列を取得
	const XMMATRIX &GetProjectionMatrix() { return matProjection; }
	//ビュー射影行列を取得
	const XMMATRIX &GetViewProjectionMatrix() { return matViewProjection; }


	Vector3 eye;				//視点座標
	Vector3 target;			//注視点座標
	Vector3 up;				//上方向ベクトル
private:
	//現在使用しているカメラ
	static Camera *NowCamera;

private:
	//ビュー行列
	XMMATRIX matView = XMMatrixIdentity();
	//射影行列
	XMMATRIX matProjection = XMMatrixIdentity();
	//ビュー射影行列
	XMMATRIX matViewProjection = XMMatrixIdentity();
	//ビュー行列ダーティフラグ
	bool viewDirty = false;
	//射影行列ダーティフラグ
	bool projectionDirty = false;
	//アスペクト比
	float aspectRatio = 1.0f;

};

