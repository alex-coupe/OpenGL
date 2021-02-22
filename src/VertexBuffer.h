#pragma once
#include "Logger.h"

class VertexBuffer {
public:
	template <class V>
	VertexBuffer(const std::vector<V> vertices, GLenum usage)
	{
		unsigned int size = (vertices.size() * sizeof(vertices.front()));
		glGenBuffers(1, &m_Id);
		GLCatchError(glBindBuffer(GL_ARRAY_BUFFER, m_Id));
		GLCatchError(glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), usage));
	}
private:
	unsigned int m_Id = 0;
};