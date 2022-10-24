#include "Collision.h"

Collision::Collision(GameObject *game_object, Collider *collider, Vector3 relative_velocity) :
	game_object_(game_object),
	collider_(collider),
	relative_velocity_(relative_velocity),
	is_child_hit_(false)
{

}
