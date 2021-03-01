#pragma once
#include "Cube.h"
class Plane : public Cube {
public:
	Plane(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
};