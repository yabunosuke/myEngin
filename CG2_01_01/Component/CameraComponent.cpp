#include "CameraComponent.h"

CameraComponent::CameraComponent() :
	Component("Camera", ComponentID::None),
	clearFlag(ClearFlag::SKY_BOX)
{
}
