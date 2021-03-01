#include "Plane.h"

Plane::Plane(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	:
	Cube(position, rotation,glm::vec3(scale.x,scale.y,0))
{
	
}


