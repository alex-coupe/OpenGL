#include "Cube.h"

Cube::Cube(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
	: Shape(position, rotation, scale)
{
	m_VertexArray->Bind();
	m_VertexBuffer->Bind(m_Vertices, GL_STATIC_DRAW);
	m_VertexArray->AddLayout(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	m_VertexArray->AddLayout(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	m_VertexArray->AddLayout(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	m_VertexArray->EnableLayout(0);
	m_VertexArray->EnableLayout(1);
	m_VertexArray->EnableLayout(2);

	m_ModelMatrix = glm::translate(m_ModelMatrix, position);
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
	m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(scale.x, scale.y, scale.z));
}

void Cube::SetIndexBuffer()
{
	m_IndexBuffer->SetIndexBuffer(m_indices.data(), (unsigned short)m_indices.size());
}



void Cube::Bind()
{
	m_VertexArray->Bind();
}

const unsigned short Cube::GetIndicies() const
{
	return m_indices.size();
}
