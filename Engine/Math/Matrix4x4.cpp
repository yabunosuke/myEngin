#include "Matrix4x4.h"
const Matrix4x4 Matrix4x4::identity		// 恒等行列
{
		1.0f,0.0f,0.0f,0.0,
		0.0f,1.0f,0.0f,0.0,
		0.0f,0.0f,1.0f,0.0,
		0.0f,0.0f,0.0f,1.0
};	
const Matrix4x4 Matrix4x4::zero			// 全ての要素を0にした行列
{
		0.0f,0.0f,0.0f,0.0,
		0.0f,0.0f,0.0f,0.0,
		0.0f,0.0f,0.0f,0.0,
		0.0f,0.0f,0.0f,0.0
};
