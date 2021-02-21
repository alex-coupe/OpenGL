#pragma once
#include "GL\glew.h"
#include <fstream>
#include <string>
#include <sstream>
#include <spdlog\spdlog.h>


class Shader {
public:
	Shader(const std::string& path, unsigned int shaderType);
	const unsigned int GetId()const;
	void DeleteShader();
private:
	unsigned int m_Id = 0;
	std::string m_source = "";
	void ParseShader(const std::string& path);
};