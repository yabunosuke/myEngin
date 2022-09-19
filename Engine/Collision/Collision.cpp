#include "Collision/Collision.h"

Collision::Collision( std::weak_ptr<GameObject> game_object, Vector3 relative_velocity):
game_object_(game_object),
relative_velocity_(relative_velocity)
{
}
