#pragma once
#include "Logger.h"
#include "InputLayout.h"

class VertexArray {
private:
	unsigned int m_Id = 0;
	std::vector<InputLayout> layouts;

public:
	VertexArray()
	{
		GLCatchError(glGenVertexArrays(1, &m_Id));
	}
	~VertexArray()
	{
		glDeleteVertexArrays(1, &m_Id);
	}

	void AddLayout(unsigned int index, GLint size, GLenum type, GLenum normalized, GLsizei stride, const void* offset)
	{
		layouts.push_back({ index, size, type, normalized, stride, offset });
	}

	void EnableLayout(unsigned int index)
	{
		glEnableVertexAttribArray(index);
	}

	void Bind()
	{
		GLCatchError(glBindVertexArray(m_Id));
	}

	void Unbind()
	{
		GLCatchError(glBindVertexArray(0));
	}
};