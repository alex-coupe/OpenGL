#pragma once
#include <memory>
#include "Shape.h"

class ShapeFactory {
public:
	template <typename T>
	static std::unique_ptr<T> Make(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale);
};
template <typename T>
std::unique_ptr<T>  ShapeFactory::Make(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
{
	return std::make_unique<T>(position, rotation, scale);
}