#pragma once
#include "GL\glew.h"
#include "spdlog\spdlog.h"

class ConstantBuffer {
public:
	ConstantBuffer(const unsigned int programId, const std::string name)
	{
		m_Id = glGetUniformLocation(programId, name.c_str());
	}

	void SetVec4f(float x, float y, float z, float w)
	{
		glUniform4f(m_Id, x, y, z, w);
	}

	void SetVec3f(float x, float y, float z)
	{
		glUniform3f(m_Id, x, y, z);
	}

	void SetVec1f(float x)
	{
		glUniform1f(m_Id, x);
	}

private:
	unsigned int m_Id = 0;
};