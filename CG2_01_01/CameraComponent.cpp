#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	Component("Camera"),
	clearFlag(ClearFlag::SKY_BOX)
{
}

void CameraComponent::VirtualUpdate()
{
	auto transfome = object.lock().get();
}
