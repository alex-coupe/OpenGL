#pragma once
#include <fstream>
#include <sstream>
#include "Logger.h"


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