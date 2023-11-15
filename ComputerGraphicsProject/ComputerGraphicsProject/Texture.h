#pragma once
#include "stdafx.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

	std::string m_strTextureName;

	GLenum m_TextureType = GL_TEXTURE_2D;
	GLuint m_TextureID;

	virtual void BindShaderVariables(GLuint s_Program, GLenum TextureSlo);

	void LoadTextureFromPNG(std::string filePath, GLuint samplingMethod);
	void LoadTextureHDR(std::string filePath, GLuint samplingMethod);
	void SetName(std::string str) { m_strTextureName = str; };
	std::string GetName() { return m_strTextureName; };
};

