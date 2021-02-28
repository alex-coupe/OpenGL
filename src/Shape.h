#pragma once
#include "Renderer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include "ShaderProgram.h"
#include "VertexBuffer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Shape {
public:
	Shape(glm::vec3 position, glm::vec3 rotation, glm::vec3 scale)
		:
		m_Position(position),
		m_Scale(scale),
		m_Rotation(rotation),
		m_VertexArray(),
		m_VertexBuffer()
	{}
protected:
	struct Vertex {
		struct {
			float x, y, z;
		} position;
		struct {
			float r, g, b;
		} color;
		struct {
			float u, v;
		} tex;
	};
	glm::vec3 m_Position;
	glm::vec3 m_Rotation;
	glm::vec3 m_Scale;
	glm::mat4 m_ModelMatrix = glm::mat4(1.0f);
	VertexArray m_VertexArray;
	VertexBuffer m_VertexBuffer;
	IndexBuffer m_IndexBuffer;
};