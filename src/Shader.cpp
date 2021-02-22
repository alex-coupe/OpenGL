#include "Shader.h"

Shader::Shader(const std::string& path, unsigned int shaderType)
{
    int success;
    char infoLog[512];
    ParseShader(path);
    const char* shaderSource = m_source.c_str();

    switch (shaderType)
    {
    case 1:
        m_Id = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(m_Id, 1, &shaderSource, NULL);
        glCompileShader(m_Id);
        glGetShaderiv(m_Id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(m_Id, 512, NULL, infoLog);
            spdlog::critical("ERROR::SHADER::VERTEX::COMPILATION_FAILED: {}", infoLog);
        }
        break;

    case 2:
        m_Id = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(m_Id, 1, &shaderSource, NULL);
        glCompileShader(m_Id);
        glGetShaderiv(m_Id, GL_COMPILE_STATUS, &success);

        if (!success)
        {
            glGetShaderInfoLog(m_Id, 512, NULL, infoLog);
            spdlog::critical("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED: {}", infoLog);
        }
        break;
    default:
        spdlog::error("ERROR::CREATESHADER::INVALID_SHADER_TYPE_PROVIDED");
        break;
    }
}

const unsigned int Shader::GetId() const
{
	return m_Id;
}

void Shader::DeleteShader()
{
    GLCatchError(glDeleteShader(m_Id));
}

void Shader::ParseShader(const std::string& path)
{
    std::ifstream input_file(path);
    if (!input_file.is_open())
    {
        spdlog::error("ERROR::PARSE_SHADER::OPEN_FILE");
    }

    std::string line;
    std::stringstream ss;
    while (getline(input_file, line))
    {
        ss << line << '\n';
    }

    m_source = ss.str();
}
