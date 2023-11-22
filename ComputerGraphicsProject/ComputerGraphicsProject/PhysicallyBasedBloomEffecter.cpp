#include "PhysicallyBasedBloomEffecter.h"
#include "Renderer.h"

CBloomFBO::CBloomFBO()
{
}

CBloomFBO::~CBloomFBO()
{
}

bool CBloomFBO::Init(UINT width, UINT height, UINT mipLength)
{
    if (m_bInit)
        return true;

    glGenFramebuffers(1, &m_FBO);
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);

    glm::vec2 mipSize{ (float)width , (float)height };
    glm::ivec2 mipIntSize{ width , height };

    for (int i = 0; i < mipLength; ++i) {
        CTexture texture;

        mipSize *= 0.5f;
        mipIntSize /= 2;
        texture.m_fHeight = mipSize.x;
        texture.m_fHeight = mipSize.y;
        texture.m_nWidth = mipIntSize.x;
        texture.m_nHeight = mipIntSize.y;

        glGenTextures(1, &texture.m_TextureID);
        glBindTexture(GL_TEXTURE_2D, texture.m_TextureID);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_R11F_G11F_B10F,
            (int)mipSize.x, (int)mipSize.y,
            0, GL_RGB, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        m_tMipBloomTextures.emplace_back(texture);
    }

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_tMipBloomTextures[0].m_TextureID, 0);

    unsigned int attachments[1] = { GL_COLOR_ATTACHMENT0 };
    glDrawBuffers(1, attachments);

    int frameStatus = glCheckFramebufferStatus(GL_FRAMEBUFFER);

    if (frameStatus != GL_FRAMEBUFFER_COMPLETE)
    {
        printf("gbuffer FBO error, status: 0x\%x\n", frameStatus);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        return false;
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    m_bInit = true;
    return true;
}

void CBloomFBO::BindShaderVariables()
{
    glBindFramebuffer(GL_FRAMEBUFFER, m_FBO);
}

CPhysicallyBasedBloomEffecter::CPhysicallyBasedBloomEffecter()
{
}

CPhysicallyBasedBloomEffecter::~CPhysicallyBasedBloomEffecter()
{
}

bool CPhysicallyBasedBloomEffecter::Init(UINT width, UINT height)
{
    if (m_bInit) return true;

    m_nvec2SrcViewportSize = glm::ivec2(width, height);
    m_vec2SrcViewportSize = glm::vec2((float)width, (float)height);

    // Framebuffer
    const unsigned int num_bloom_mips = 5; // Experiment with this value
    bool status = m_BloomFBO.Init(width, height, num_bloom_mips);
    if (!status) {
        std::cerr << "Failed to initialize bloom FBO - cannot create bloom renderer!\n";
        return false;
    }

    m_NdcMesh = CMesh::CreateNDCMesh();
    m_NdcMesh->CreateShaderVariables();

    m_bInit = true;
    return true;
}

void CPhysicallyBasedBloomEffecter::RenderBloomTexture(GLuint srcTexture, float filterRadius)
{
    m_BloomFBO.BindShaderVariables();

    RenderDownsamples(srcTexture);
    RenderUpsamples(filterRadius);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    // Restore viewport
    glViewport(0, 0, m_nvec2SrcViewportSize.x, m_nvec2SrcViewportSize.y);
}

GLuint CPhysicallyBasedBloomEffecter::BloomTexture()
{
    return m_BloomFBO.GetTextures()[0].m_TextureID;
}

void CPhysicallyBasedBloomEffecter::RenderDownsamples(GLuint srcTexture)
{
    const std::vector<CTexture>& mipChain = m_BloomFBO.GetTextures();

    GLuint s_Program = g_Renderer->BloomDownSampleShader;
    glUseProgram(s_Program);
    GLuint resoultionLoc = glGetUniformLocation(s_Program, "srcResolution");
    glUniform2fv(resoultionLoc, 1, glm::value_ptr(m_vec2SrcViewportSize));

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, srcTexture);

    for (int i = 0; i < mipChain.size(); i++)
    {
        const CTexture& mip = mipChain[i];
        glViewport(0, 0, mip.m_nWidth, mip.m_nHeight);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, mip.m_TextureID, 0);

        m_NdcMesh->BindShaderVariables(s_Program);
        m_NdcMesh->Render();
        glBindVertexArray(0);

        resoultionLoc = glGetUniformLocation(s_Program, "srcResolution");
        glm::vec2 nextResoultion{ mip.m_nWidth, mip.m_nHeight };
        glUniform2fv(resoultionLoc, 1, glm::value_ptr(nextResoultion));

        glBindTexture(GL_TEXTURE_2D, mip.m_TextureID);
    }
}

void CPhysicallyBasedBloomEffecter::RenderUpsamples(float filterRadius)
{
    const std::vector<CTexture>& mipChain = m_BloomFBO.GetTextures();

    GLuint s_Program = g_Renderer->BloomDownSampleShader;
    glUseProgram(s_Program);
    GLuint resoultionLoc = glGetUniformLocation(s_Program, "filterRadius");
    glUniform1f(resoultionLoc, filterRadius);

    // Enable additive blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_ONE, GL_ONE);
    glBlendEquation(GL_FUNC_ADD);

    for (int i = mipChain.size() - 1; i > 0; i--)
    {
        const CTexture& mip = mipChain[i];
        const CTexture& nextMip = mipChain[i - 1];

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, mip.m_TextureID);

        glViewport(0, 0, mip.m_nWidth, mip.m_nHeight);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D, nextMip.m_TextureID, 0);

        m_NdcMesh->BindShaderVariables(s_Program);
        m_NdcMesh->Render();
        glBindVertexArray(0);
    }
    glDisable(GL_BLEND);
}
