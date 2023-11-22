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

	float m_fWidth = 0;
	float m_fHeight = 0;
	UINT m_nWidth = 0;
	UINT m_nHeight = 0;

	virtual void BindShaderVariables(GLuint s_Program, GLenum TextureSlo);

	void LoadTextureFromPNG(std::string filePath, GLuint samplingMethod);
	void LoadTextureHDR(std::string filePath, GLuint samplingMethod);
	void SetName(std::string str) { m_strTextureName = str; };
	std::string GetName() { return m_strTextureName; };
};

class CMesh;
class CCamera;
class CViewerTexture : public CTexture
{
public:
	CViewerTexture();
	virtual ~CViewerTexture();

	virtual void Bake(GLuint s_Program, CMesh* pMesh, CCamera* pCamera);

	bool m_bBaked = false;
};