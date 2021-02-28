#pragma once
#include "Logger.h"
#include <glm/gtc/type_ptr.hpp>

class ConstantBuffer {
public:
	ConstantBuffer(const unsigned int programId, const std::string name)
		: m_Name(name), m_ProgramId(programId)
	{
		m_Id = glGetUniformLocation(programId, name.c_str());

		if (m_Id == -1)
			spdlog::warn("Uniform not found! {}", name);
	}

	void SetUniform4f(float v0, float v1, float v2, float v3)
	{
		GLCatchError(glUniform4f(glGetUniformLocation(m_ProgramId, m_Name.c_str()), v0, v1,v2,v3));
	}

	void SetUniform3f(float v0, float v1, float v2)
	{
		GLCatchError(glUniform3f(glGetUniformLocation(m_ProgramId, m_Name.c_str()), v0,v1,v2));
	}

	void SetUniform2f(float v0, float v1)
	{
		GLCatchError(glUniform2f(glGetUniformLocation(m_ProgramId, m_Name.c_str()), v0,v1));
	}

	void SetUniform1f(float v0)
	{
		GLCatchError(glUniform1f(glGetUniformLocation(m_ProgramId, m_Name.c_str()), v0));
	}

	void SetUniform1i(int v0)
	{
		GLCatchError(glUniform1i(glGetUniformLocation(m_ProgramId, m_Name.c_str()), v0));
	}

	void SetUniformMatrix4fv(glm::mat4 trans)
	{
		GLCatchError(glUniformMatrix4fv(glGetUniformLocation(m_ProgramId, m_Name.c_str()), 1, GL_FALSE, glm::value_ptr(trans)));
	}

	void SetProgram(unsigned int programId)
	{
		m_ProgramId = programId;
	}

private:
	unsigned int m_Id = 0;
	unsigned int m_ProgramId = 0;
	std::string m_Name;
};