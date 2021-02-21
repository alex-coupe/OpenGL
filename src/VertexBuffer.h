#pragma once
#include "GL\glew.h"
#include "spdlog\spdlog.h"

class VertexBuffer {
public:
	template <class V>
	VertexBuffer(const std::vector<V> vertices, GLenum usage)
	{
		unsigned int size = (vertices.size() * sizeof(vertices.front()));
		GLenum err;
		std::string error;
		while ((err = glGetError()) != GL_NO_ERROR);
		glGenBuffers(1, &m_Id);
		glBindBuffer(GL_ARRAY_BUFFER, m_Id);
		glBufferData(GL_ARRAY_BUFFER, size, vertices.data(), usage);
		while ((err = glGetError()) != GL_NO_ERROR)
		{
			error += err;
			error += __FILE__;
			spdlog::error(error);
		}
	}
private:
	unsigned int m_Id = 0;
};