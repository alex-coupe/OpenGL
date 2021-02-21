#pragma once
#include "GL\glew.h"

class IndexBuffer {
public:
	IndexBuffer(const unsigned short indices[]);
	void DrawIndexed(GLenum mode, GLsizei count, GLenum type);
private:
	unsigned int m_Id = 0;
};