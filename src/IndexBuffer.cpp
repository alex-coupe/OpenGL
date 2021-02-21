#include "IndexBuffer.h"

IndexBuffer::IndexBuffer(const unsigned short indices[])
{
    glGenBuffers(1, &m_Id);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_Id);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
}

void IndexBuffer::DrawIndexed(GLenum mode, GLsizei count, GLenum type)
{
    glDrawElements(mode, count, type, 0);
}
