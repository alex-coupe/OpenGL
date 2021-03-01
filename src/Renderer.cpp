#include "Renderer.h"

Renderer::Renderer(bool enableDepth)
{
	if (enableDepth)
		GLCatchError(glEnable(GL_DEPTH_TEST));
}

void Renderer::BeginFrame() const
{
	GLCatchError(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT));
}

void Renderer::Draw(const unsigned short indexCount) const
{
	GLCatchError(glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_SHORT, nullptr));
}

void Renderer::Draw(GLint first, GLsizei count) const
{
	GLCatchError(glDrawArrays(GL_TRIANGLES, first, count));
}
