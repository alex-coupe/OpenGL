#pragma once
#include "Logger.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "VertexArray.h"
#include "GLFW\glfw3.h"

class Renderer {
public: 
	void BeginFrame()const;
	void Draw(const IndexBuffer& ib) const;
	void Draw(GLint first, GLsizei count)const;
	void EndFrame(GLFWwindow* window)const
	{
		glfwSwapBuffers(window);
	}
};