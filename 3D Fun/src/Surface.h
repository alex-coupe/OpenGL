#pragma once
#include "stb\stb_image.h"
#include "Logger.h"

class Surface {
private:
	int m_Width, m_Height, m_Colors;
	unsigned int m_Id;
	unsigned char* m_Data;
public:
	Surface(const std::string& imagePath, unsigned int slot = 0)
		:
		m_Data(nullptr), m_Width(0), m_Height(0), m_Colors(0)
	{
		stbi_set_flip_vertically_on_load(true);
		m_Data = stbi_load(imagePath.c_str(), &m_Width, &m_Height, &m_Colors, 4);
		GLCatchError(glGenTextures(1, &m_Id));

		GLCatchError(glActiveTexture(GL_TEXTURE0 + slot));
		GLCatchError(glBindTexture(GL_TEXTURE_2D, m_Id));

		GLCatchError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR));
		GLCatchError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));
		GLCatchError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE));
		GLCatchError(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE));

		
		GLCatchError(glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_Width, m_Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_Data));
		GLCatchError(glGenerateMipmap(GL_TEXTURE_2D));
			
		if (m_Data)
		{
			stbi_image_free(m_Data);
		}
		else
			spdlog::warn("Failed to load texture");
	}


	~Surface()
	{
		GLCatchError(glDeleteTextures(1, &m_Id));
	}

	void SetTextureSlot(unsigned int slot)
	{
		GLCatchError(glActiveTexture(GL_TEXTURE0 + slot));

	}

	unsigned int GetId() const
	{
		return m_Id;
	}

	
};
