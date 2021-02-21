#include "ShaderProgram.h"

ShaderProgram::ShaderProgram(const unsigned int vertexShader, const unsigned int fragmentShader)
{
	m_Id = glCreateProgram();
	glAttachShader(m_Id, vertexShader);
	glAttachShader(m_Id, fragmentShader);
    glLinkProgram(m_Id);
    int success;
    char infoLog[512];
    glGetProgramiv(m_Id, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(m_Id, 512, NULL, infoLog);
        spdlog::error("ERROR::SHADER_PROGRAM: {}", infoLog);
    }
}

const void ShaderProgram::UseProgram()
{
	glUseProgram(m_Id);
}

const unsigned int ShaderProgram::GetId() const
{
    return m_Id;
}
