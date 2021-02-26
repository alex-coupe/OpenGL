#pragma once
#include "GLFW\glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera {
private:
	glm::vec3 m_Position;
	glm::vec3 m_Direction;
	glm::vec3 m_WorldUp;
    float m_Speed = 2.5f;
    float m_Fov = 45.0f;
    float m_Pitch = 0.0f;
    float m_Yaw = -90.0f;
    float m_LastX;
    float m_LastY;
    bool m_rawMouse = true;
    bool m_firstMouse = true;
public:
	Camera(glm::vec3 position, float screenWidth, float screenHeight)
		:
		m_Position(position),
		m_Direction(glm::vec3(0.0f, 0.0f, -1.0f)),
		m_WorldUp(glm::vec3(0.0f, 1.0f, 0.0f)),
        m_LastX(screenWidth/2),
        m_LastY(screenHeight/2)
	{}

	glm::mat4 GetLookAt()
	{
		return glm::lookAt(m_Position, m_Position + m_Direction, m_WorldUp);
	}

    void UpdateFieldOfView(GLFWwindow* window, double xoffset, double yoffset)
    {
        m_Fov -= (float)yoffset;
        if (m_Fov < 1.0f)
            m_Fov = 1.0f;
        if (m_Fov > 45.0f)
            m_Fov = 45.0f;
    }

    const float GetFieldOfView() const
    {
        return m_Fov;
    }

    void UpdateCameraRotation(GLFWwindow* window, double xpos, double ypos)
    {
        if (m_rawMouse)
        {
            if (m_firstMouse)
            {
                m_LastX = (float)xpos;
                m_LastY = (float)ypos;
                m_firstMouse = false;
            }

            float xoffset = (float)xpos - m_LastX;
            float yoffset = m_LastY - (float)ypos;
            m_LastX = (float)xpos;
            m_LastY = (float)ypos;

            float sensitivity = 0.1f;
            xoffset *= sensitivity;
            yoffset *= sensitivity;

            m_Yaw += xoffset;
            m_Pitch += yoffset;

            if (m_Pitch > 89.0f)
                m_Pitch = 89.0f;
            if (m_Pitch < -89.0f)
                m_Pitch = -89.0f;

            glm::vec3 direction;
            direction.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            direction.y = sin(glm::radians(m_Pitch));
            direction.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
            m_Direction = glm::normalize(direction);
        }
    }

	void UpdateCameraInput(GLFWwindow* window, float deltaTime)
	{
        if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        {
            if (m_rawMouse)
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
                m_rawMouse = false;
            }
            else
            {
                glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
                m_rawMouse = true;
            }
        }

        float cameraSpeed = m_Speed * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            m_Position += cameraSpeed * m_Direction;

        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            m_Position -= cameraSpeed * m_Direction;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            m_Position -= glm::normalize(glm::cross(m_Direction, m_WorldUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            m_Position += glm::normalize(glm::cross(m_Direction, m_WorldUp)) * cameraSpeed;

        m_Position.y = 0.0f;
	}
};