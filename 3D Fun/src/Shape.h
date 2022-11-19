#pragma once
#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <memory>

class Shape {
public:
	Shape(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		:
		m_Position(position),
		m_Scale(scale),
		m_Rotation(rotation),
		m_VertexArray(std::make_unique<VertexArray>()),
		m_VertexBuffer(std::make_unique<VertexBuffer>()),
		m_IndexBuffer(std::make_unique<IndexBuffer>())
	{	}
protected:
	struct Vertex {
		struct {
			float x, y, z;
		} position;
		struct {
			float u, v;
		} tex;
	};
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	std::unique_ptr<VertexArray> m_VertexArray;
	std::unique_ptr<VertexBuffer> m_VertexBuffer;
	std::unique_ptr<IndexBuffer> m_IndexBuffer;
public:
	
	const glm::mat4 GetModelMatrix() const
	{
		return m_ModelMatrix;
	}
	void UpdateModelMatrix()
	{
		m_ModelMatrix = glm::mat4(1.0f);
		m_ModelMatrix = glm::translate(m_ModelMatrix, m_Position);
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.x), glm::vec3(1.0f, 0.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.y), glm::vec3(0.0f, 1.0f, 0.0f));
		m_ModelMatrix = glm::rotate(m_ModelMatrix, glm::radians(m_Rotation.z), glm::vec3(0.0f, 0.0f, 1.0f));

		m_ModelMatrix = glm::scale(m_ModelMatrix, m_Scale);
	}
	glm::vec3& GetPosition()
	{
		return m_Position;
	}
	glm::vec3& GetRotation()
	{
		return m_Rotation;
	}
	glm::vec3& GetScale()
	{
		return m_Scale;
	}

	void SetScale(glm::vec3 scale)
	{
		m_Scale.x = scale.x;
		m_Scale.y = scale.y;
		m_Scale.z = scale.z;
	}
	
	void CleanUp()
	{
		m_VertexArray->Unbind();
	}
};