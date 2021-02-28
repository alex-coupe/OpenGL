#pragma once
#include "Logger.h"

class IndexBuffer {
public:
	IndexBuffer()
	{
		glGenBuffers(1, &m_Id);
	}

	void SetIndexBuffer(const unsigned short* data, unsigned short count)
	{
		m_Count = count;
		GLCatchError(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id));
		GLCatchError(glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Count * sizeof(unsigned short), data, GL_STATIC_DRAW));
	}
	
	~IndexBuffer()
	{
		GLCatchError(glDeleteBuffers(1, &m_Id));
	}

	unsigned short GetCount()const
	{
		return m_Count;
	}
private:
	unsigned int m_Id = 0;
	unsigned short m_Count = 0;
};