#pragma once
#include "Math/Vector2.h"
#include "Object/Component/Transform.h"

class RectTransform final :
    public Transform
{
public:
	RectTransform();

	enum  class Anchor
	{
		LEFT_TOP,		CENTER_TOP,		RIGHT_TOP,
		LEFT_MID,		CENTER_MID,		RIGHT_MID,
		LEFT_BOTTOM,	CENTER_BOTTOM,	RIGHT_BOTTOM,

		MAX
	};

	void Infomation() override;

private:
	void UpdateMatrix() override;

	Vector2 ancheor_positions_[static_cast<int>(Anchor::MAX)]
	{
		{0.0f,0.0f},	{0.5f,0.0f},	{1.0f,0.0f},
		{0.0f,0.5f},	{0.5f,0.5f},	{1.0f,0.5f},
		{0.0f,1.0f},	{0.5f,1.0f},	{1.0f,1.0f},
	};
	Anchor ancher_point_{Anchor::CENTER_MID };
};

