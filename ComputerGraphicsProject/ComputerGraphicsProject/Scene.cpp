#include "Scene.h"
#include "Global.h"
#include "Renderer.h"
#include "Mesh.h"
#include "Timer.h"
#include "ResourceManager.h"
#include "GUIManager.h"

CScene::CScene()
{
}

CScene::~CScene()
{
}

void CScene::Init()
{
	m_fbloomFilterRadius = 0.005f;

	m_pLights.reserve(MAX_LIGHTS);
	m_pLights.emplace_back();
	m_pSunLight = &m_pLights[0];
	m_pSunLight->m_vec3LightColor *= 1.0f;

	memcpy(UBOLightData.lights, m_pLights.data(), sizeof(CLight) * m_pLights.size());
	UBOLightData.nLights = m_pLights.size();



	GLuint sizeVec = sizeof(glm::vec3);
	GLuint size = sizeof(UBO_LIGHT);
	GLuint lightSize = sizeof(CLight);
	std::cout << lightSize << std::endl;

	glGenBuffers(1, &m_UBOLights);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOLights);
	glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
	if (glGetError())
		std::cout << "error BufferData" << std::endl;

	glBindBufferBase(GL_UNIFORM_BUFFER, 0, m_UBOLights);

	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOLights);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, lightSize * MAX_LIGHTS, UBOLightData.lights);
	if (glGetError())
		std::cout << "error BufferData" << std::endl;
	glBufferSubData(GL_UNIFORM_BUFFER, lightSize * MAX_LIGHTS, sizeof(UINT), &UBOLightData.nLights);
	if (glGetError())
		std::cout << "error BufferData" << std::endl;
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	//glGetActiveUniformBlockiv(g_Renderer->TestShader, m_UBOLightIndex, GL_UNIFORM_BLOCK_DATA_SIZE, )

	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	////glBindBufferBase(GL_UNIFORM_BUFFER, m_UBOLightIndex, m_UBOLights);
	//
	//glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOLights, 0, size);
	//if (glGetError())
	//	std::cout << "error glBindBufferRange" << std::endl;

	//glBindBuffer(GL_UNIFORM_BUFFER, m_UBOLights);
	//if (glGetError())
	//	std::cout << "error glBindBuffer" << std::endl;
	//glBufferSubData(GL_UNIFORM_BUFFER, 0, size, &UBOLightData);
	//if (glGetError())
	//	std::cout << "error glBindBuffer" << std::endl;
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);
	//


	//glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBOLights, (GLintptr)0, (GLintptr)(sizeof(UBO_LIGHT)));
	//glBindBuffer(GL_UNIFORM_BUFFER, 0);


	m_sptrCameras.reserve(10);
	std::shared_ptr<CCamera> pCamera = std::make_unique<CCamera>();

	pCamera->RegenarationViewMatrix();
	pCamera->GenerateProjectionMatrix(glm::radians(90.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);
	m_sptrCameras.push_back(std::move(pCamera));
	m_pMainCamera = m_sptrCameras[0].get();

	glm::quat rotate = m_pMainCamera->GetQauternion();
	rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_pMainCamera->SetQauternion(rotate);
	m_pMainCamera->SetPosision(m_pMainCamera->GetPosition() + glm::vec3(0, 5.0f, 0.0f));
}

void CScene::RenderScene()
{
}

void CScene::MouseInput(int button, int state, int x, int y)
{
}

void CScene::MouseMotion(int x, int y)
{
}

void CScene::KeyInput(unsigned char key, int x, int y)
{
}

void CScene::KeyUpInput(unsigned char key, int x, int y)
{
}

void CScene::SpecialKeyInput(int key, int x, int y)
{
}

void CScene::BindShaderVariables(GLuint s_Program)
{
	memcpy(UBOLightData.lights, m_pLights.data(), sizeof(CLight) * m_pLights.size());
	UBOLightData.nLights = m_pLights.size();


	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOLights);
	GLuint offset = 0u;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(UBOLightData), &UBOLightData);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}



void CScene::Update(float fElapsedTime)
{
}

void CScene::SetPolygonMode(GLenum face, GLenum mode)
{
	glPolygonMode(face, mode);
}

void CScene::CreateMultiRenderTargetObject(int nWidth, int nHeight, std::vector<RENDERTARGET_INFO> RBOinfos)
{
	m_RBOInfo = RBOinfos;
	int nRbos = m_RBOInfo.size();

	glGenFramebuffers(1, &m_FBOMultiRenderTarget);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOMultiRenderTarget);

	m_RBOs.resize(nRbos);

	for (int i = 0; i < nRbos; ++i) {
		glGenTextures(1, &m_RBOs[i]);
		glBindTexture(GL_TEXTURE_2D, m_RBOs[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, m_RBOInfo[i].InternalFormat, nWidth, nHeight, 0, m_RBOInfo[i].Format, m_RBOInfo[i].type, NULL);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	for (int i = 0; i < nRbos; ++i) {
		glFramebufferTexture2D(GL_FRAMEBUFFER, m_RBOInfo[i].Attachment, GL_TEXTURE_2D, m_RBOs[i], 0);
	}
	
	GLuint attachments[8];
	for (int i = 0; i < m_RBOs.size(); ++i)
		attachments[i] = m_RBOInfo[i].Attachment;

	glDrawBuffers(m_RBOs.size(), attachments);

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	m_bEnableMultiRenderTarget = true;
}

void CScene::BindFrameBufferObject(bool Is_FBO_Clear, GLbitfield FBO_Clear_Option)
{
	if (m_bEnableMultiRenderTarget == false) {
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		if (Is_FBO_Clear) {
			glClear(FBO_Clear_Option);
		}

		return;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOMultiRenderTarget);


	
	if (Is_FBO_Clear) {
		glClear(FBO_Clear_Option);
	}
}

void CScene::PostProcessing()
{
	if (!m_bEnableMultiRenderTarget) {
		return;
	}
}

void CScene::Enter()
{
	if (!m_bInitialized)
		Init();
}

void CScene::Exit()
{
}

CPBR_TestScene::CPBR_TestScene()
{
}

CPBR_TestScene::~CPBR_TestScene()
{
}

void CPBR_TestScene::Init()
{
	CScene::Init();

	CLight light;
	light.m_vec3Direction = glm::vec3(0, -1, 0);
	m_pLights.emplace_back(light);

	// HDRI 로부터 큐브 환경 맵 생성
	GLuint cubeFBO, cubeRBO;
	GLuint cubeMapWidth = 1024, cubeMapHeight = 1024;
	glGenFramebuffers(1, &cubeFBO);
	glGenRenderbuffers(1, &cubeRBO);



	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, cubeRBO);

	glRenderbufferStorage(GL_RENDERBUFFER, cubeRBO, cubeMapWidth, cubeMapHeight);
	/*GLenum error = glGetError();
	if (error)
		std::cout << "error glBindBuffer" << std::endl;*/
		//glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, cubeRBO);


	GLuint& cubeMapID = m_tCubeMapTexture.m_TextureID;
	glGenTextures(1, &cubeMapID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
	m_tCubeMapTexture.m_TextureType = GL_TEXTURE_CUBE_MAP;
	for (GLuint i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, cubeMapWidth, cubeMapHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glm::mat4 tempCaptureProjection = glm::perspective(glm::radians(90.f), 1.0f, 0.1f, 10.0f);
	glm::mat4 tempCaptureViews[6] = {
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	GLuint MakeCubeMapShader = g_Renderer->MakeCubeMapShader;
	glUseProgram(MakeCubeMapShader); // 셰이더 설정

	GLuint samplerULoc = glGetUniformLocation(MakeCubeMapShader, "u_BaseTexture");
	glUniform1i(samplerULoc, 0);

	//m_tCubeMapTexture.BindShaderVariables(MakeCubeMapShader, GL_TEXTURE_CUBE_MAP);

	GLuint projectionLocation;
	projectionLocation = glGetUniformLocation(MakeCubeMapShader, "projectionTransform");	// ProjMatrix
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &tempCaptureProjection[0][0]);

	std::shared_ptr<CTexture> pTexture = std::make_shared<CTexture>();
	//pTexture->LoadTextureFromPNG("./Textures/rgb.png", GL_NEAREST);
	pTexture->LoadTextureHDR("./Textures/scythian_tombs_4k.hdr", GL_LINEAR);
	pTexture->BindShaderVariables(MakeCubeMapShader, GL_TEXTURE0);

	std::shared_ptr<CMesh> pCubeMesh = std::make_shared<CMesh>();
	pCubeMesh = CMesh::CreateCubeMesh(1.f, 1.f, 1.f);
	pCubeMesh->CreateShaderVariables();

	glViewport(0, 0, cubeMapWidth, cubeMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	for (GLuint i = 0; i < 6; ++i) {
		GLuint viewLocation;
		viewLocation = glGetUniformLocation(MakeCubeMapShader, "viewTransform");	// ViewMatrix
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &tempCaptureViews[i][0][0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, cubeMapID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pCubeMesh->BindShaderVariables(MakeCubeMapShader);
		pCubeMesh->Render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 생성된 큐브 환경 맵에서 irradiance 큐브 맵 생성
	m_tIrradianceTexture = std::make_shared<CTexture>();
	GLuint& IrradianceID = m_tIrradianceTexture->m_TextureID;
	GLuint IrradianceMapWidth = 64, IrradianceMapHeight = 64;
	glGenTextures(1, &IrradianceID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceID);
	m_tIrradianceTexture->m_TextureType = GL_TEXTURE_CUBE_MAP;
	for (GLuint i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, IrradianceMapWidth, IrradianceMapHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindBuffer(GL_FRAMEBUFFER, cubeFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, cubeRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, cubeRBO, IrradianceMapWidth, IrradianceMapHeight);

	GLuint irradianceShader = g_Renderer->irradianceShader;
	glUseProgram(irradianceShader);

	samplerULoc = glGetUniformLocation(irradianceShader, "u_BaseTexture");
	glUniform1i(samplerULoc, 0);

	//m_tCubeMapTexture.BindShaderVariables(MakeCubeMapShader, GL_TEXTURE_CUBE_MAP);

	projectionLocation;
	projectionLocation = glGetUniformLocation(irradianceShader, "projectionTransform");	// ProjMatrix
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &tempCaptureProjection[0][0]);

	m_tCubeMapTexture.BindShaderVariables(irradianceShader, GL_TEXTURE0);

	glViewport(0, 0, IrradianceMapWidth, IrradianceMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	for (GLuint i = 0; i < 6; ++i) {
		GLuint viewLocation;
		viewLocation = glGetUniformLocation(irradianceShader, "viewTransform");	// ViewMatrix
		glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &tempCaptureViews[i][0][0]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradianceID, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		pCubeMesh->BindShaderVariables(irradianceShader);
		pCubeMesh->Render();
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	m_tFilteringedEnvironmentTexture = std::make_shared<CTexture>();
	GLuint& FilteringedEnvironmentID = m_tFilteringedEnvironmentTexture->m_TextureID;
	GLuint  FilteringedEnvironmentWidth = 512, FilteringedEnvironmentHeight = 512;
	glGenTextures(1, &FilteringedEnvironmentID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, FilteringedEnvironmentID);
	m_tFilteringedEnvironmentTexture->m_TextureType = GL_TEXTURE_CUBE_MAP;
	for (int i = 0; i < 6; ++i) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, FilteringedEnvironmentWidth, FilteringedEnvironmentHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


	GLuint preFilteringCubeMapShader = g_Renderer->preFilteringCubeMapShader;
	glUseProgram(preFilteringCubeMapShader);

	samplerULoc = glGetUniformLocation(preFilteringCubeMapShader, "u_BaseTexture");
	glUniform1i(samplerULoc, 0);

	projectionLocation = glGetUniformLocation(preFilteringCubeMapShader, "projectionTransform");	// ProjMatrix
	glUniformMatrix4fv(projectionLocation, 1, GL_FALSE, &tempCaptureProjection[0][0]);

	m_tCubeMapTexture.BindShaderVariables(preFilteringCubeMapShader, GL_TEXTURE0);

	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);

	UINT maxMipLevel = 5;
	for (unsigned int mip = 0; mip < maxMipLevel; mip++) {
		if (mip == 1) {
			glBindTexture(GL_TEXTURE_CUBE_MAP, FilteringedEnvironmentID);
			glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
			m_tCubeMapTexture.BindShaderVariables(preFilteringCubeMapShader, GL_TEXTURE0);
		}

		UINT mipWidth = FilteringedEnvironmentWidth * std::pow(0.5, mip);
		UINT mipHeight = FilteringedEnvironmentHeight * std::pow(0.5, mip);

		glBindFramebuffer(GL_RENDERBUFFER, cubeRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, mipWidth, mipHeight);
		glViewport(0, 0, mipWidth, mipHeight);

		float roughness = (float)mip / (float)(maxMipLevel - 1);
		GLuint roughnessLoc = glGetUniformLocation(preFilteringCubeMapShader, "roughness");
		glUniform1f(roughnessLoc, roughness);

		for (GLuint i = 0; i < 6; ++i) {
			GLuint viewLocation;
			viewLocation = glGetUniformLocation(preFilteringCubeMapShader, "viewTransform");	// ViewMatrix
			glUniformMatrix4fv(viewLocation, 1, GL_FALSE, &tempCaptureViews[i][0][0]);

			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, FilteringedEnvironmentID, mip);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			pCubeMesh->BindShaderVariables(preFilteringCubeMapShader);
			pCubeMesh->Render();
		}
	}
	m_tFilteringedEnvironmentTexture->BindShaderVariables(preFilteringCubeMapShader, GL_TEXTURE0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	m_tPreCoumputedBRDFLUTexture = std::make_shared<CTexture>();
	GLuint& PreCoumputedBRDFLUTID = m_tPreCoumputedBRDFLUTexture->m_TextureID;
	GLuint  PreCoumputedBRDFLUTWidth = 512, PreCoumputedBRDFLUTHeight = 512;
	glGenTextures(1, &PreCoumputedBRDFLUTID);
	glBindTexture(GL_TEXTURE_2D, PreCoumputedBRDFLUTID);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RG16F, PreCoumputedBRDFLUTWidth, PreCoumputedBRDFLUTHeight, 0, GL_RG, GL_FLOAT, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	GLuint preComputingBRDFShader = g_Renderer->preComputingBRDFShader;
	glUseProgram(preComputingBRDFShader);


	glBindFramebuffer(GL_FRAMEBUFFER, cubeFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, cubeRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, PreCoumputedBRDFLUTWidth, PreCoumputedBRDFLUTHeight);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, PreCoumputedBRDFLUTID, 0);

	glViewport(0, 0, PreCoumputedBRDFLUTWidth, PreCoumputedBRDFLUTHeight);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	std::shared_ptr<CMesh> NDCMesh = CMesh::CreateNDCMesh();
	NDCMesh->CreateShaderVariables();
	NDCMesh->BindShaderVariables(preComputingBRDFShader);
	NDCMesh->Render();

	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	// 섀도우 맵 텍스쳐 생성
	glGenFramebuffers(1, &m_FBOShadowDepth);

	m_nShadowMapWidth = 2048; m_nShadowMapHeight = 2048;

	m_tShadowDepthTexture = std::make_shared<CTexture>();
	GLuint& shadowDepthMap = m_tShadowDepthTexture->m_TextureID;
	glGenTextures(1, &shadowDepthMap);
	glBindTexture(GL_TEXTURE_2D, shadowDepthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, m_nShadowMapWidth, m_nShadowMapHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOShadowDepth);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, shadowDepthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	std::shared_ptr<CMesh> testCubeMesh;
	testCubeMesh = CMesh::CreateCubeMesh(1.0f, 1.0f, 1.0f);
	testCubeMesh->CreateShaderVariables();
	CResourceManager::GetInst()->RegisterMesh(testCubeMesh);



	m_pSkyBoxObject = std::make_shared<CObject>();

	std::shared_ptr<CMaterial> pMaterial = std::make_shared<CMaterial>();
	pTexture = m_tFilteringedEnvironmentTexture;
	pMaterial->SetBaseTexture(pTexture);
	//CResourceManager::GetInst()->RegisterMaterial(pMaterial);

	m_pSkyBoxObject->SetMesh(testCubeMesh);
	m_pSkyBoxObject->SetMaterial(pMaterial);

	BuildObjects();

	m_bInitialized = true;
}

void CPBR_TestScene::BuildObjects()
{
	if (m_PBBloomEffecter.Init(g_WindowSizeX, g_WindowSizeY))
		m_bEnablePhysicallyBasedBloom = true;

	std::vector<RENDERTARGET_INFO> RBOInfos;

	RBOInfos.resize(4);
	RBOInfos[0].InternalFormat = GL_RGBA16F;
	RBOInfos[0].Format = GL_RGBA;
	RBOInfos[0].type = GL_FLOAT;
	RBOInfos[0].Attachment = GL_COLOR_ATTACHMENT0;

	RBOInfos[1].InternalFormat = GL_RGBA16F;
	RBOInfos[1].Format = GL_RGBA;
	RBOInfos[1].type = GL_FLOAT;
	RBOInfos[1].Attachment = GL_COLOR_ATTACHMENT1;

	RBOInfos[2].InternalFormat = GL_RGBA16F;
	RBOInfos[2].Format = GL_RGBA;
	RBOInfos[2].type = GL_FLOAT;
	RBOInfos[2].Attachment = GL_COLOR_ATTACHMENT2;

	RBOInfos[3].InternalFormat = GL_DEPTH_COMPONENT;
	RBOInfos[3].Format = GL_DEPTH_COMPONENT;
	RBOInfos[3].type = GL_FLOAT;
	RBOInfos[3].Attachment = GL_DEPTH_ATTACHMENT;

	CreateMultiRenderTargetObject(g_WindowSizeX, g_WindowSizeY, RBOInfos);

	m_NdcMesh = CMesh::CreateNDCMesh();
	m_NdcMesh->CreateShaderVariables();

	std::shared_ptr<CMesh> testSphereMesh;
	testSphereMesh = CMesh::CreateSphereMesh(20, 20);
	testSphereMesh->CreateShaderVariables();
	CResourceManager::GetInst()->RegisterMesh(testSphereMesh);

	std::shared_ptr<CObject> planeObj = std::make_shared<CObject>();
	planeObj->LoadGeometryAndAnimationFromFile("./Objects/Plane.bin");
	planeObj->GetMaterial(0)->RoughnessColor = 1.0f;

	std::shared_ptr<CMaterial> testMaterial = std::make_shared<CMaterial>();
	std::shared_ptr<CTexture> texture = CResourceManager::GetInst()->ImportTexture("./Textures/rustediron2_basecolor.png", GL_LINEAR);
	testMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/rustediron2_normal.png", GL_LINEAR);
	testMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/rustediron2_metallic.png", GL_LINEAR);
	testMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/rustediron2_roughness.png", GL_LINEAR);
	testMaterial->SetRoughnessTexture(texture);
	CResourceManager::GetInst()->RegisterMaterial(testMaterial);

	testMaterial = std::make_shared<CMaterial>();
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/Iron-Scuffed_basecolor.png", GL_LINEAR);
	testMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/Iron-Scuffed_normal.png", GL_LINEAR);
	testMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/Iron-Scuffed_metallic.png", GL_LINEAR);
	testMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/Iron-Scuffed_roughness.png", GL_LINEAR);
	testMaterial->SetRoughnessTexture(texture);
	CResourceManager::GetInst()->RegisterMaterial(testMaterial);

	testMaterial = std::make_shared<CMaterial>();
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/older-padded-leather_albedo.png", GL_LINEAR);
	testMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/older-padded-leather_normal-ogl.png", GL_LINEAR);
	testMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/older-padded-leather_metallic.png", GL_LINEAR);
	testMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/older-padded-leather_roughness.png", GL_LINEAR);
	testMaterial->SetRoughnessTexture(texture);
	CResourceManager::GetInst()->RegisterMaterial(testMaterial);

	testMaterial = std::make_shared<CMaterial>();
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/white-quilted-diamond_albedo.png", GL_LINEAR);
	testMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/white-quilted-diamond_normal-ogl.png", GL_LINEAR);
	testMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/white-quilted-diamond_metallic.png", GL_LINEAR);
	testMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/white-quilted-diamond_roughness.png", GL_LINEAR);
	testMaterial->SetRoughnessTexture(texture);
	CResourceManager::GetInst()->RegisterMaterial(testMaterial);

	testMaterial = std::make_shared<CMaterial>();
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/wrinkled-paper-albedo.png", GL_LINEAR);
	testMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/wrinkled-paper-metalness.png", GL_LINEAR);
	testMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/wrinkled-paper-normal-ogl.png", GL_LINEAR);
	testMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture = CResourceManager::GetInst()->ImportTexture("./Textures/wrinkled-paper-roughness.png", GL_LINEAR);
	testMaterial->SetRoughnessTexture(texture);
	CResourceManager::GetInst()->RegisterMaterial(testMaterial);

	int nObj = 1;
	m_pObjects.resize(nObj);
	glm::vec3 basePos{ 0,1.5,0 };
	/*for (int i = 0; i < 10; ++i) {
		m_pObjects[i] = std::make_shared<CObject>();
		m_pObjects[i]->SetMesh(testSphereMesh);
		m_pObjects[i]->SetPosition(basePos);
		std::shared_ptr<CMaterial> Material = std::make_shared<CMaterial>();
		Material->RoughnessColor = (0.1f + (i * 0.1f));
		g_Renderer->RegisterMaterial(Material);
		m_pObjects[i]->SetMaterial(testMaterial);
		basePos.x += 2.0f;
	}*/
	for (int i = 0; i < nObj; ++i) {
		m_pObjects[i] = std::make_shared<CObject>();
		m_pObjects[i]->LoadGeometryAndAnimationFromFile("./Objects/TestModel.bin");
		CObject* frame = m_pObjects[i]->GetChild();
		int nMat = 0;
		/*while (frame != nullptr)
		{
			frame->SetMaterial(g_Renderer->GetMaterialFromIndex(nMat++ % 6));
			if(frame->GetSibling() != nullptr)
				frame = frame->GetSibling();
			else
				frame = frame->GetChild();
		}*/
		m_pObjects[i]->SetMaterial(0, CResourceManager::GetInst()->GetMaterialFromIndex(i % 6));
		m_pObjects[i]->SetPosition(basePos);
		//m_pObjects[i]->SetScale(glm::vec3(0.1, 0.1, 0.1));
		basePos.x += 2.0f;
	}

	m_pObjects.push_back(planeObj);

	std::shared_ptr<CObject> obj = std::make_shared<CObject>();
	obj->LoadGeometryAndAnimationFromFile("./Objects/Robot.bin");
	obj->SetPosition(glm::vec3(0, 0, 3));
	m_pObjects.emplace_back(obj);

	CGUIManager::GetInst()->SetSelectedObject(m_pObjects[0].get());
}

void CPBR_TestScene::RenderScene()
{
	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);



	glViewport(0, 0, m_nShadowMapWidth, m_nShadowMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOShadowDepth);

	glClear(GL_DEPTH_BUFFER_BIT);

	// Render ShadowDepthMap
	CShadowCamera shadowCamera;
	//shadowCamera.GenerateProjectionMatrix(glm::radians(90.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);
	float fNear = 1.0f, fFar = 20.0f;
	shadowCamera.m_mat4x4Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, fNear, fFar);
	shadowCamera.SetPosision(m_pSunLight->m_vec3Position);
	shadowCamera.m_mat4x4View = glm::lookAt(m_pSunLight->m_vec3Position, m_pSunLight->m_vec3Direction, glm::vec3(0, 1, 0));
	shadowCamera.BindShaderVariables(s_Program);

	glm::mat4 lightSpaceMatrix = shadowCamera.m_mat4x4Projection * shadowCamera.m_mat4x4View;

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	SetPolygonMode(m_ePolygonFace, m_ePolygonMode);

	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->UpdateTransform(nullptr);
		obj->Render(s_Program);
	}



	s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);


	BindShaderVariables(s_Program);
	GLuint lightSpaceLoc;

	lightSpaceLoc = glGetUniformLocation(s_Program, "lightSpaceMatrix");
	glUniformMatrix4fv(lightSpaceLoc, 1, GL_FALSE, &lightSpaceMatrix[0][0]);

	GLuint samplerULoc = glGetUniformLocation(s_Program, "u_IrradianceTexture");
	glUniform1i(samplerULoc, 5);
	m_tIrradianceTexture->BindShaderVariables(s_Program, GL_TEXTURE5);

	samplerULoc = glGetUniformLocation(s_Program, "u_BrdfLUT");
	glUniform1i(samplerULoc, 6);
	m_tPreCoumputedBRDFLUTexture->BindShaderVariables(s_Program, GL_TEXTURE6);

	samplerULoc = glGetUniformLocation(s_Program, "u_PreFilterMap");
	glUniform1i(samplerULoc, 7);
	m_tFilteringedEnvironmentTexture->BindShaderVariables(s_Program, GL_TEXTURE7);
	
	samplerULoc = glGetUniformLocation(s_Program, "u_ShadowMap");
	glUniform1i(samplerULoc, 8);
	glActiveTexture(GL_TEXTURE8);
	glBindTexture(GL_TEXTURE_2D, m_tShadowDepthTexture->m_TextureID);

	if (m_bEnableMultiRenderTarget) {
		glBindFramebuffer(GL_FRAMEBUFFER, m_FBOMultiRenderTarget);
	}
	else
		glBindFramebuffer(GL_FRAMEBUFFER, 0);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	//m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program, m_bRegenarateView);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->UpdateTransform(nullptr);
		obj->Render(s_Program);
	}

	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	SetPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	//m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program, m_bRegenarateView);


	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->UpdateTransform(nullptr);
	m_pSkyBoxObject->Render(s_Program);


	if (m_bEnablePhysicallyBasedBloom) {
		if (m_bEnableMultiRenderTarget) {
			m_PBBloomEffecter.RenderBloomTexture(m_RBOs[0], m_fbloomFilterRadius);

			s_Program = g_Renderer->BloomShader;
			glUseProgram(s_Program);
			glBindFramebuffer(GL_FRAMEBUFFER, m_FBOMultiRenderTarget);
			samplerULoc = glGetUniformLocation(s_Program, "scene");
			glUniform1i(samplerULoc, 0);
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, m_RBOs[0]);
			samplerULoc = glGetUniformLocation(s_Program, "bloomBlur");
			glUniform1i(samplerULoc, 1);
			glActiveTexture(GL_TEXTURE1);
			glBindTexture(GL_TEXTURE_2D, m_PBBloomEffecter.BloomTexture());

			GLuint exposureULoc = glGetUniformLocation(s_Program, "exposure");
			glUniform1f(exposureULoc, m_fExposure);

			m_NdcMesh->BindShaderVariables(s_Program);
			m_NdcMesh->Render();
		}
		else {

		}
	}


	if (m_bEnableMultiRenderTarget) {
		s_Program = g_Renderer->PostProcessShader;
		glUseProgram(s_Program);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		for (int i = 0; i < m_RBOs.size(); ++i) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, m_RBOs[i]);
		}

		m_NdcMesh->BindShaderVariables(s_Program);
		m_NdcMesh->Render();
	}

	/*static bool showDemo = true;
	static ImVec4 clear_color = { 0,0,0,1 };
	ImGui::ShowDemoWindow(&showDemo);

	static float f = 0.0f;
	static int counter = 0;*/
	
	CGUIManager::GetInst()->ShowAssetInspector(this);
}

void CPBR_TestScene::MouseInput(int button, int state, int x, int y)
{
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
		m_eMouseState = MOUSE_STATE::MOUSE_CILCK_RIGHT;
		m_ptOldMouseCursor = { x,y };
		m_ptOldMouseCursor.x -= g_WindowSizeX / 2;
		m_ptOldMouseCursor.y -= g_WindowSizeY / 2;
	}
	if (button == GLUT_RIGHT_BUTTON && state == GLUT_UP) {
		m_eMouseState = MOUSE_STATE::MOUSE_CILCK_NONE;
	}
}

void CPBR_TestScene::MouseMotion(int x, int y)
{
	switch (m_eMouseState)
	{
	case MOUSE_STATE::MOUSE_CILCK_LEFT:
		break;
	case MOUSE_STATE::MOUSE_CILCK_RIGHT:
		m_ptCurrMouseCuror = { x,y };
		m_ptCurrMouseCuror.x -= g_WindowSizeX / 2;
		m_ptCurrMouseCuror.y -= g_WindowSizeY / 2;
		cxDelta = -(float)(m_ptCurrMouseCuror.x - m_ptOldMouseCursor.x);
		cyDelta = (float)(m_ptCurrMouseCuror.y - m_ptOldMouseCursor.y);
		m_ptOldMouseCursor = m_ptCurrMouseCuror;

		break;
	case MOUSE_STATE::MOUSE_CILCK_NONE:
		break;
	default:
		break;
	}
}

void CPBR_TestScene::KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		DwDirection |= DW_FRONT;
		break;
	case 's':
		DwDirection |= DW_BACK;
		break;
	case 'd':
		DwDirection |= DW_RIGHT;
		break;
	case 'a':
		DwDirection |= DW_LEFT;
		break;
	case 'q':
		DwDirection |= DW_DOWN;
		break;
	case 'e':
		DwDirection |= DW_UP;
		break;
	case 'f':
		m_pMainCamera->m_vec3Look.x += 0.1f;
		m_pMainCamera->m_vec3Look = glm::normalize(m_pMainCamera->m_vec3Look);
		break;
	case 'g':
		m_pMainCamera->m_vec3Look.x -= 0.1f;
		m_pMainCamera->m_vec3Look = glm::normalize(m_pMainCamera->m_vec3Look);
		break;
	default:
		break;
	}
	glm::vec3 pos = m_pMainCamera->GetPosition();
	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
}

void CPBR_TestScene::KeyUpInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		DwDirection &= (~DW_FRONT);
		break;
	case 's':
		DwDirection &= (~DW_BACK);
		break;
	case 'd':
		DwDirection &= (~DW_RIGHT);
		break;
	case 'a':
		DwDirection &= (~DW_LEFT);
		break;
	case 'q':
		DwDirection &= (~DW_DOWN);
		break;
	case 'e':
		DwDirection &= (~DW_UP);
		break;
	default:
		break;
	}
}

void CPBR_TestScene::SpecialKeyInput(int key, int x, int y)
{
}



void CPBR_TestScene::Update(float fElapsedTime)
{
	glm::vec3 cameraPosition = m_pMainCamera->GetPosition();
	glm::vec3 cameraVelocity = glm::vec3(0);

	if (DwDirection != 0) {
		if (DwDirection & DW_FRONT)
			cameraVelocity -= m_pMainCamera->m_vec3Look;
		if (DwDirection & DW_BACK)
			cameraVelocity += m_pMainCamera->m_vec3Look;
		if (DwDirection & DW_RIGHT)
			cameraVelocity += m_pMainCamera->m_vec3Right;
		if (DwDirection & DW_LEFT)
			cameraVelocity -= m_pMainCamera->m_vec3Right;
		if (DwDirection & DW_UP)
			cameraVelocity += m_pMainCamera->m_vec3Up;
		if (DwDirection & DW_DOWN)
			cameraVelocity -= m_pMainCamera->m_vec3Up;
		cameraVelocity = glm::normalize(cameraVelocity);
	}

	if (!glm::all(glm::isnan(cameraVelocity)))
		cameraPosition += cameraVelocity * g_Timer->GetFrameTimeElapsed();
	m_pMainCamera->SetPosision(cameraPosition);

	if (m_eMouseState == MOUSE_STATE::MOUSE_CILCK_RIGHT) {
		if (abs(cxDelta) > 0.0001f || abs(cyDelta > 0.0001f)) {
			glm::vec3 axis = glm::vec3(0, 1, 0);
			glm::qua<float> qResult = glm::rotate(m_pMainCamera->m_vec4Rotation, glm::radians(-cxDelta * 180.0f) * fElapsedTime, glm::normalize(axis));
			axis = glm::vec3(1, 0, 0);
			qResult = glm::rotate(qResult, glm::radians(cyDelta * 180.0f) * fElapsedTime, glm::normalize(axis));

			m_pMainCamera->m_vec4Rotation = qResult;

			cxDelta = 0;
			cyDelta = 0;
		}
	}

	float fTime = g_Timer->GetTotalTime();

	m_pSunLight->m_vec3LightColor = glm::vec3(1, 1, 1);
	m_pSunLight->m_vec3Direction = glm::normalize(glm::vec3(-1, -1, -1));
	m_pSunLight->m_vec3Direction = glm::vec3(sin(fTime), -1, cos(fTime));
	m_pSunLight->m_vec3Position = (-m_pSunLight->m_vec3Direction) * 10.0f;


}
