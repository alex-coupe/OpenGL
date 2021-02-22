#pragma once
#include "Logger.h"

class ConstantBuffer {
public:
	ConstantBuffer(const unsigned int programId, const std::string name)
	{
		m_Id = glGetUniformLocation(programId, name.c_str());

		if (m_Id == -1)
			spdlog::warn("Uniform not found!");
	}

	void SetUniform4f(float x, float y, float z, float w)
	{
		GLCatchError(glUniform4f(m_Id, x, y, z, w));
	}

	void SetUniform3f(float x, float y, float z)
	{
		GLCatchError(glUniform3f(m_Id, x, y, z));
	}

	void SetUniform2f(float x, float y)
	{
		GLCatchError(glUniform2f(m_Id, x, y));
	}

	void SetUniform1f(float x)
	{
		GLCatchError(glUniform1f(m_Id, x));
	}

	void SetUniform1i(int x)
	{
		GLCatchError(glUniform1i(m_Id, x));
	}

private:
	unsigned int m_Id = 0;
};