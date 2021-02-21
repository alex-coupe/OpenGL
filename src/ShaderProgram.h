#pragma once
#include "GL\glew.h"
#include "spdlog\spdlog.h"

class ShaderProgram {
public:
	ShaderProgram(const unsigned int vertexShader, const unsigned int fragmentShader);
	const void UseProgram();
private:
	unsigned int m_Id = 0;
};