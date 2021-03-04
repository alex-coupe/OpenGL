#pragma once
#include "spdlog\spdlog.h"
#include "GL\glew.h"

#define GLCatchError(x) GLClearErrors();x;GLLogErrors(#x,__LINE__, __FILE__);

static void GLClearErrors()
{
	while (glGetError() != GL_NO_ERROR);
}

static void GLLogErrors(const char* function, const int line, const char* file)
{
	while (GLenum err = glGetError())
	{
		spdlog::error("[OpenGL Error] {} in {} on line {} of {}", err, function, line, file);
	}
}

static void LogError(const char* message)
{
	spdlog::error("[Error] {}", message);
}