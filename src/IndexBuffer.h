#pragma once
#include "Logger.h"

class IndexBuffer {
public:
	IndexBuffer(const unsigned short indices[])
	{
		glGenBuffers(1, &m_Id);
		GLCatchError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
		GLCatchError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW));
	}
private:
	unsigned int m_Id = 0;
};