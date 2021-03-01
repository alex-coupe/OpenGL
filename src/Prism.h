#pragma once
#include "Shape.h"

class Prism : public Shape {
public:
	Prism(glm::vec3 position, glm::vec3 rotiation, glm::vec3 scale);
	void SetIndexBuffer();
	void Bind();
private:

};