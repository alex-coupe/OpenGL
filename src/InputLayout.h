#pragma once
#include "Logger.h"

class InputLayout {
public:
	InputLayout(unsigned int index, GLint size, GLenum type, GLenum normalized, GLsizei stride, const void* offset)
	{
		GLCatchError(glVertexAttribPointer(index, size, type, normalized, stride, offset));
	}
};