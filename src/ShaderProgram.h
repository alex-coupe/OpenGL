#pragma once
#include "Logger.h"

class ShaderProgram {
public:
	ShaderProgram(const unsigned int vertexShader, const unsigned int fragmentShader);
	const void UseProgram();
	const unsigned int GetId()const;
	const void DeleteProgram();
private:
	unsigned int m_Id = 0;
};