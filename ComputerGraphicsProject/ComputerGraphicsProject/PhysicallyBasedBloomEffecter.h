#pragma once
#include "stdafx.h"
#include "Texture.h"

class CBloomFBO
{
	bool m_bInit = false;
	GLuint m_FBO;
	std::vector<CTexture> m_tMipBloomTextures;
public:
	CBloomFBO();
	~CBloomFBO();
	bool Init(UINT width,UINT height, UINT mipLength);
	void BindShaderVariables();
	std::vector<CTexture>& GetTextures() { return m_tMipBloomTextures; };
};

class CPhysicallyBasedBloomEffecter
{
public:
	CPhysicallyBasedBloomEffecter();
	~CPhysicallyBasedBloomEffecter();

	bool Init(UINT width, UINT height);
	void RenderBloomTexture(GLuint srcTexture, float filterRadius);
	GLuint BloomTexture();

private:
	void RenderDownsamples(GLuint srcTexture);
	void RenderUpsamples(float filterRadius);

	bool m_bInit = false;
	CBloomFBO m_BloomFBO;
	glm::ivec2 m_nvec2SrcViewportSize;
	glm::vec2 m_vec2SrcViewportSize;
	std::shared_ptr<CMesh> m_NdcMesh;
};

