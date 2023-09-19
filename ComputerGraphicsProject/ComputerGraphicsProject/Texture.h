#pragma once
#include "stdafx.h"

class CTexture
{
public:
	CTexture();
	virtual ~CTexture();

	GLenum m_TextureType = GL_TEXTURE_2D;
	GLuint m_TextureID;

	virtual void BindShaderVariables(GLuint s_Program, GLenum TextureSlo);

	void LoadTextureFromPNG(std::string filePath, GLuint samplingMethod);
};

