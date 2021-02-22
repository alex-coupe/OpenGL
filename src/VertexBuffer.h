#pragma once
#include "Logger.h"

class VertexBuffer {
public:
	VertexBuffer()
	{
		glGenBuffers(1, &m_Id);
	}
	template <class V>
	void Bind(const std::vector<V> vertices, GLenum usage)
	{
		unsigned int size = (vertices.size() * sizeof(vertices.front()));
		GLCatchError(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
		GLCatchError(glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), usage));
	}
private:
	unsigned int m_Id = 0;
};