#include "ExamScene.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Timer.h"
#include <cmath>
#include <vector>
#include <set>

CExamScene_7::CExamScene_7()
{
}

CExamScene_7::~CExamScene_7()
{
}

void CExamScene_7::Init()
{
	CExamScene::Init();

	m_pLights.emplace_back();
	m_pSunLight = &m_pLights.back();
	m_pSunLight->m_vec3LightColor *= 2.0f;
	m_pSunLight->m_vec3Direction = glm::vec3(0.0f, -1.0f, 0.0f);

	m_sptrCameras.resize(1);
	m_sptrCameras[0] = std::make_unique<CCamera>();

	m_sptrCameras[0]->RegenarationViewMatrix();
	m_sptrCameras[0]->GenerateProjectionMatrix(glm::radians(90.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);
	m_pMainCamera = m_sptrCameras[0].get();

	glm::quat rotate = m_pMainCamera->GetQauternion();
	rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
	m_pMainCamera->SetQauternion(rotate);
	m_pMainCamera->SetPosision(m_pMainCamera->GetPosition() + glm::vec3(0, 5.0f, 0.0f));

	std::shared_ptr<CMesh> testCubeMesh;
	testCubeMesh = CMesh::CreateCubeMesh(1.0f, 1.0f, 1.0f);
	testCubeMesh->CreateShaderVariables();

	std::shared_ptr<CMaterial> pMaterial = std::make_shared<CMaterial>();

	m_pSkyBoxObject = std::make_shared<CObject>();
	m_pSkyBoxObject->SetMesh(testCubeMesh);
	m_pSkyBoxObject->SetMaterial(pMaterial);
}

void CExamScene_7::RenderScene()
{
	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);


	GLuint samplerULoc = glGetUniformLocation(s_Program, "u_IrradianceTexture");
	glUniform1i(samplerULoc, 5);
	m_tIrradianceTexture->BindShaderVariables(s_Program, GL_TEXTURE5);

	samplerULoc = glGetUniformLocation(s_Program, "u_BrdfLUT");
	glUniform1i(samplerULoc, 6);
	m_tPreCoumputedBRDFLUTexture->BindShaderVariables(s_Program, GL_TEXTURE6);

	samplerULoc = glGetUniformLocation(s_Program, "u_PreFilterMap");
	glUniform1i(samplerULoc, 7);
	m_tFilteringedEnvironmentTexture->BindShaderVariables(s_Program, GL_TEXTURE7);


	m_pMainCamera->BindShaderVariables(s_Program);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->Render(s_Program);
	}

	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	m_pMainCamera->BindShaderVariables(s_Program);

	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->Render(s_Program);
}

void CExamScene_7::KeyInput(unsigned char key, int x, int y)
{
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);
	static std::uniform_real_distribution<float> urd_pos(-3.0f, 3.0f);
	switch (key)
	{
	case 'p':
	case 'l':
	case 't':
	case 'r':

		if (m_pObjects.size() < 10) {
			std::shared_ptr<CObject> obj = std::make_shared<CObject>();
			std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>();
			material->BaseColor = glm::vec3(urd_color(dre), urd_color(dre), urd_color(dre));
			material->RoughnessColor = 0.0f;

			std::shared_ptr<CMesh> mesh;
			if (key == 'p')
				mesh = CMesh::CreateCubeMesh(0.1, 0.1, 0.1);
			else if (key == 'l')
				mesh = CMesh::CreateLineMesh();
			else if (key == 't')
				mesh = CMesh::CreateTriangleMesh();
			else if (key == 'r')
				mesh = CMesh::CreateRectMesh();

			mesh->CreateShaderVariables();
			obj->SetMesh(mesh);
			obj->SetMaterial(material);
			obj->SetPosition(glm::vec3(urd_pos(dre), 0.0f, urd_pos(dre)));


			m_pObjects.push_back(obj);
		}
		break;
	case 'w':
		m_AimbientMoveDir = glm::vec3(0.0f, 0.0f, 1.0f);
		break;
	case 'a':
		m_AimbientMoveDir = glm::vec3(-1.0f, 0.0f, 0.0f);
		break;
	case 's':
		m_AimbientMoveDir = glm::vec3(0.0f, 0.0f, -1.0f);
		break;
	case 'd':
		m_AimbientMoveDir = glm::vec3(1.0f, 0.0f, 0.0f);
		break;
	case 'c':
		m_pObjects.clear();
		break;
	default:
		break;
	}
}

void CExamScene_7::Update(float fElapsedTime)
{
	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		glm::vec3 pos = obj->GetPosition();
		pos = pos + m_AimbientMoveDir * fElapsedTime;

		if (pos.x > 3.0f)
			pos.x -= 6.0f;
		if (pos.z > 3.0f)
			pos.z -= 6.0f;
		if (pos.x < -3.0f)
			pos.x += 6.0f;
		if (pos.z < -3.0f)
			pos.z += 6.0f;
		obj->SetPosition(pos);
	}
}

void CExamScene::Init()
{
	GLuint cubeFBO, cubeRBO;
	GLuint cubeMapWidth = 1024, cubeMapHeight = 1024;
	glGenFramebuffers(1, &cubeFBO);
	glGenRenderbuffers(1, &cubeRBO);

	glBindBuffer(GL_FRAMEBUFFER, cubeFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, cubeRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, cubeRBO, cubeMapWidth, cubeMapHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, cubeRBO);

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

	m_bInitialized = true;
}

CExamScene_8::CExamScene_8()
{
}

CExamScene_8::~CExamScene_8()
{
}

void CExamScene_8::Init()
{
	static std::uniform_real_distribution<float> urd_pos{ 0.0f, 3.0f };
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);
	static std::uniform_real_distribution<float> urd_scale(0.5f, 2.0f);

	CExamScene_7::Init();

	for (int i = 0; i < 4; ++i) {
		std::shared_ptr<CObject> obj = std::make_shared<CObject>();
		std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>();
		material->BaseColor = glm::vec3(urd_color(dre), urd_color(dre), urd_color(dre));
		material->RoughnessColor = 0.0f;
		material->MetallicColor = 1.0f;

		std::shared_ptr<CMesh> mesh;
		mesh = CMesh::CreateTriangleMesh();
		mesh->CreateShaderVariables();

		obj->SetMesh(mesh);
		obj->SetMaterial(material);
		float scale = urd_scale(dre);
		obj->SetScale(glm::vec3(scale, 1.0f, scale));

		m_pObjects.push_back(obj);
	}

	m_pObjects[0]->SetPosition(glm::vec3(urd_pos(dre), 0.0f, -urd_pos(dre)));
	m_pObjects[1]->SetPosition(glm::vec3(-urd_pos(dre), 0.0f, -urd_pos(dre)));
	m_pObjects[2]->SetPosition(glm::vec3(-urd_pos(dre), 0.0f, urd_pos(dre)));
	m_pObjects[3]->SetPosition(glm::vec3(urd_pos(dre), 0.0f, urd_pos(dre)));

	std::shared_ptr<CMesh> mesh = CMesh::CreateLineMesh();
	mesh->CreateShaderVariables();

	std::shared_ptr<CObject> obj = std::make_shared<CObject>();
	std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>();
	material->BaseColor = glm::vec3(1, 1, 1);
	material->RoughnessColor = 0.0f;
	material->MetallicColor = 1.0f;

	obj->SetMesh(mesh);
	obj->SetMaterial(material);
	obj->SetScale(glm::vec3(10.0f, 10.0f, 10.0f));
	obj->SetPosition(glm::vec3(0, 0, -5));

	m_pObjects.push_back(obj);

	obj = std::make_shared<CObject>();
	obj->SetMesh(mesh);
	obj->SetMaterial(material);
	obj->SetScale(glm::vec3(10.0f, 10.0f, 20.0f));
	obj->SetPosition(glm::vec3(-10, 0, 0));
	obj->RotationQuat(glm::radians(90.0f), glm::vec3(0, 1, 0));

	m_pObjects.push_back(obj);
}

void CExamScene_8::RenderScene()
{
	glPolygonMode(GL_FRONT_AND_BACK, m_eFillmode);

	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);


	GLuint samplerULoc = glGetUniformLocation(s_Program, "u_IrradianceTexture");
	glUniform1i(samplerULoc, 5);
	m_tIrradianceTexture->BindShaderVariables(s_Program, GL_TEXTURE5);

	samplerULoc = glGetUniformLocation(s_Program, "u_BrdfLUT");
	glUniform1i(samplerULoc, 6);
	m_tPreCoumputedBRDFLUTexture->BindShaderVariables(s_Program, GL_TEXTURE6);

	samplerULoc = glGetUniformLocation(s_Program, "u_PreFilterMap");
	glUniform1i(samplerULoc, 7);
	m_tFilteringedEnvironmentTexture->BindShaderVariables(s_Program, GL_TEXTURE7);


	m_pMainCamera->BindShaderVariables(s_Program);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->Render(s_Program);
	}


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	m_pMainCamera->BindShaderVariables(s_Program);

	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->Render(s_Program);
}

void CExamScene_8::KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'a':
		m_eFillmode = GL_FILL;
		break;
	case 'b':
		m_eFillmode = GL_LINE;
		break;
	default:
		break;
	}
}

void CExamScene_8::MouseInput(int button, int state, int x, int y)
{
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);
	static std::uniform_real_distribution<float> urd_scale(0.5f, 2.0f);

	if (button == GLUT_LEFT && state == GLUT_DOWN) {
		float mouse_X = x;
		float mouse_Y = y;

		glm::vec3 mousePos(x, y, 0.0f);
		mousePos = glm::inverse(m_pMainCamera->m_mat4x4View) * glm::inverse(m_pMainCamera->m_mat4x4Projection) * glm::inverse(viewPortMatrix) * glm::vec4(mousePos, 1.0f);
		float scale = urd_scale(dre);
		if (mousePos.x > 0.0f && mousePos.z < 0.0f) {
			m_pObjects[0]->SetPosition(glm::vec3(mousePos.x * 5, 0.0f, mousePos.z * 5));
			m_pObjects[0]->GetMaterial(0)->BaseColor = glm::vec3(urd_color(dre), urd_color(dre), urd_color(dre));
			m_pObjects[0]->SetScale(glm::vec3(scale, 1.0f, scale));
		}
		if (mousePos.x < 0.0f && mousePos.z < 0.0f) {
			m_pObjects[1]->SetPosition(glm::vec3(mousePos.x * 5, 0.0f, mousePos.z * 5));
			m_pObjects[1]->GetMaterial(0)->BaseColor = glm::vec3(urd_color(dre), urd_color(dre), urd_color(dre));
			m_pObjects[1]->SetScale(glm::vec3(scale, 1.0f, scale));
		}
		if (mousePos.x < 0.0f && mousePos.z  > 0.0f) {
			m_pObjects[2]->SetPosition(glm::vec3(mousePos.x * 5, 0.0f, mousePos.z * 5));
			m_pObjects[2]->GetMaterial(0)->BaseColor = glm::vec3(urd_color(dre), urd_color(dre), urd_color(dre));
			m_pObjects[2]->SetScale(glm::vec3(scale, 1.0f, scale));
		}
		if (mousePos.x > 0.0f && mousePos.z > 0.0f) {
			m_pObjects[3]->SetPosition(glm::vec3(mousePos.x * 5, 0.0f, mousePos.z * 5));
			m_pObjects[3]->GetMaterial(0)->BaseColor = glm::vec3(urd_color(dre), urd_color(dre), urd_color(dre));
			m_pObjects[3]->SetScale(glm::vec3(scale, 1.0f, scale));
		}
	}
}

void CExamScene_8::Update(float fElapsedTime)
{
	/*float fTime = g_Timer->GetTotalTime();

	m_pSunLight->m_vec3LightColor = glm::vec3(1, 1, 1);
	m_pSunLight->m_vec3Direction = glm::normalize(glm::vec3(-1, -1, -1));
	m_pSunLight->m_vec3Direction = glm::vec3(sin(fTime), cos(fTime), -1);*/
}

CExamScene_9::CExamScene_9()
{
	Init();
}

CExamScene_9::~CExamScene_9()
{
}

void CExamScene_9::Init()
{
	CExamScene_7::Init();

}

void CExamScene_9::RenderScene()
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);


	GLuint samplerULoc = glGetUniformLocation(s_Program, "u_IrradianceTexture");
	glUniform1i(samplerULoc, 5);
	m_tIrradianceTexture->BindShaderVariables(s_Program, GL_TEXTURE5);

	samplerULoc = glGetUniformLocation(s_Program, "u_BrdfLUT");
	glUniform1i(samplerULoc, 6);
	m_tPreCoumputedBRDFLUTexture->BindShaderVariables(s_Program, GL_TEXTURE6);

	samplerULoc = glGetUniformLocation(s_Program, "u_PreFilterMap");
	glUniform1i(samplerULoc, 7);
	m_tFilteringedEnvironmentTexture->BindShaderVariables(s_Program, GL_TEXTURE7);


	m_pMainCamera->BindShaderVariables(s_Program);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->Render(s_Program);
	}


	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	m_pMainCamera->BindShaderVariables(s_Program);

	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->Render(s_Program);
}

void CExamScene_9::KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		for (int i = 0; i < m_moveDir.size(); ++i)
			m_moveDir[i] = glm::normalize(glm::vec3(-1.0, 0.0, -1.0f));
		m_moveType = 1;
		break;
	case '2':
		for (int i = 0; i < m_moveDir.size(); ++i) {
			glm::quat rotate{ 1,0,0,0 };
			rotate = glm::rotate(rotate, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // 오른쪽을 보게
			m_pObjects[i]->SetRotate(rotate);
			m_moveDir[i] = glm::normalize(glm::vec3(-1.0f, 0.0f, 0.0f));
		}
		m_moveType = 2;
		break;
	default:
		break;
	}
}

void CExamScene_9::MouseInput(int button, int state, int x, int y)
{
	static std::uniform_real_distribution<float> urd_color(0.0f, 1.0f);
	static std::uniform_real_distribution<float> urd_scale(0.5f, 2.0f);

	if (button == GLUT_LEFT && state == GLUT_DOWN) {
		if (m_pObjects.size() < 4) {
			float mouse_X = x;
			float mouse_Y = y;

			glm::vec3 mousePos(x, y, 0.0f);
			mousePos = glm::inverse(m_pMainCamera->m_mat4x4View) * glm::inverse(m_pMainCamera->m_mat4x4Projection) * glm::inverse(viewPortMatrix) * glm::vec4(mousePos, 1.0f);

			std::shared_ptr<CObject> obj = std::make_shared<CObject>();
			std::shared_ptr<CMesh> mesh = CMesh::CreateTriangleMesh();
			mesh->CreateShaderVariables();
			std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>();
			material->BaseColor = glm::vec3(urd_color(dre), urd_color(dre), urd_color(dre));
			obj->SetMesh(mesh);
			obj->SetPosition(glm::vec3(mousePos.x * 5, 0.0f, mousePos.z * 5));
			obj->SetMaterial(material);
			obj->RotationQuat(glm::radians(180.f), glm::vec3(0.0f, 1.0f, 0.0f));

			m_pObjects.emplace_back(obj);
			m_moveDir.emplace_back(glm::vec3(0));
			m_collisionDelay.emplace_back(0.5f);
		}
	}
}

void CExamScene_9::Update(float fElapsedTime)
{
	auto checkInside = [&](glm::vec4& rect) {
		if (rect.x < -width)
			return false;
		if (rect.y < -height)
			return false;
		if (rect.z > width)
			return false;
		if (rect.w > height)
			return false;
		return true;
	};
	static glm::vec4 rectColide{ -0.5, -1.0f, 0.5f, 1.0f };
	for (int i = 0; i < m_pObjects.size(); ++i) {
		CObject* obj = m_pObjects[i].get();
		if (m_moveType)
			obj->SetPosition(obj->GetPosition() + m_moveDir[i] * fElapsedTime * 5.0f);
		switch (m_moveType)
		{
		case 1:
		{
			glm::vec4 point1{ rectColide.x, 0.0f, rectColide.y,1.0f };
			point1 = obj->m_mat4x4Wolrd * point1;
			glm::vec4 point2{ rectColide.z, 0.0f,  rectColide.w, 1.0f };
			point2 = obj->m_mat4x4Wolrd * point2;

			glm::vec3 pos = obj->GetPosition();
			glm::vec4 rect{ point2.x, point2.z, point1.x , point1.z };
			//rect += rectColide;
			m_collisionDelay[i] -= fElapsedTime;
			if (m_collisionDelay[i] < 0.0f) {
				if (rect.x < -width) {
					m_moveDir[i].x *= -1;
					m_collisionDelay[i] = 0.5f;
				}
				if (rect.y < -height) {
					m_moveDir[i].z *= -1;
					m_collisionDelay[i] = 0.5f;
				}
				if (rect.z > width) {
					m_moveDir[i].x *= -1;
					m_collisionDelay[i] = 0.5f;
				}
				if (rect.w > height) {
					m_moveDir[i].z *= -1;
					m_collisionDelay[i] = 0.5f;
				}
			}
			std::cout << rect.x << " " << rect.y << " " << rect.z << " " << rect.w << std::endl;
		}
		break;
		case 2:
		{
			glm::vec4 point1{ rectColide.x, 0.0f, rectColide.y,1.0f };
			point1 = obj->m_mat4x4Wolrd * point1;
			glm::vec4 point2{ rectColide.z, 0.0f,  rectColide.w, 1.0f };
			point2 = obj->m_mat4x4Wolrd * point2;

			glm::vec4 rect{ point2.x, point2.z, point1.x , point1.z };

			m_collisionDelay[i] -= fElapsedTime;
			if (checkInside(rect) && m_collisionDelay[i] < 0.0f) {
				if (rect.x < -width) {
					m_moveDir[i].x *= -1;
					m_collisionDelay[i] = 0.5f;
				}
				if (rect.z > width) {
					m_moveDir[i].x *= -1;
					m_collisionDelay[i] = 0.5f;
				}
			}
		}
		break;
		default:
			break;
		}

	}
}

CExamScene_18::CExamScene_18()
{
}

CExamScene_18::~CExamScene_18()
{
}

void CExamScene_18::Init()
{
	CPBR_TestScene::Init();

	std::shared_ptr<CMesh> mesh = CMesh::CreateRectMesh();
	std::shared_ptr<CMaterial> material[6];
	std::shared_ptr<CObject> child[6]; std::make_shared<CObject>();
	std::shared_ptr<CObject> master; std::make_shared<CObject>();

	for (int i = 0; i < 6; ++i) {
		material[i] = std::make_shared<CMaterial>();
		child[i] = std::make_shared<CObject>();
		child[i]->SetMesh(mesh);
		child[i]->SetMaterial(material[i]);
	}

	std::shared_ptr<CObject> obj = std::make_shared<CObject>();

	for (int i = 0; i < 6; ++i)
		obj->SetChild(child[i]);

	//obj->RotationQuat(glm::radians(90.0f), glm::vec3(0, 0, 1));

	m_pObjects.emplace_back(obj);
}


void CExamScene_18::BuildObjects()
{
}


void CExamScene_18::Update(float fElapsedTime)
{
	CPBR_TestScene::Update(fElapsedTime);
}

std::uniform_real_distribution<float> CSPScene::urd_velocityScale = std::uniform_real_distribution<float>(0.8f, 1.2f);
std::uniform_int_distribution<unsigned int> CSPScene::urd_material = std::uniform_int_distribution<unsigned int>();
std::uniform_real_distribution<float>  CSPScene::urd_rotate = std::uniform_real_distribution<float>(-1.0f, 1.0f);

CSPScene::CSPScene()
{
}

CSPScene::~CSPScene()
{
}

void CSPScene::BuildObjects()
{
	std::shared_ptr<CMaterial> newMaterial = std::make_shared<CMaterial>();
	std::shared_ptr<CTexture> texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/rustediron2_basecolor.png", GL_LINEAR);
	newMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/rustediron2_normal.png", GL_LINEAR);
	newMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/rustediron2_metallic.png", GL_LINEAR);
	newMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/rustediron2_roughness.png", GL_LINEAR);
	newMaterial->SetRoughnessTexture(texture);
	g_Renderer->RegisterMaterial(newMaterial);

	newMaterial = std::make_shared<CMaterial>();
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/Iron-Scuffed_basecolor.png", GL_LINEAR);
	newMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/Iron-Scuffed_normal.png", GL_LINEAR);
	newMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/Iron-Scuffed_metallic.png", GL_LINEAR);
	newMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/Iron-Scuffed_roughness.png", GL_LINEAR);
	newMaterial->SetRoughnessTexture(texture);
	g_Renderer->RegisterMaterial(newMaterial);

	newMaterial = std::make_shared<CMaterial>();
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/older-padded-leather_albedo.png", GL_LINEAR);
	newMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/older-padded-leather_normal-ogl.png", GL_LINEAR);
	newMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/older-padded-leather_metallic.png", GL_LINEAR);
	newMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/older-padded-leather_roughness.png", GL_LINEAR);
	newMaterial->SetRoughnessTexture(texture);
	g_Renderer->RegisterMaterial(newMaterial);

	newMaterial = std::make_shared<CMaterial>();
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/white-quilted-diamond_albedo.png", GL_LINEAR);
	newMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/white-quilted-diamond_normal-ogl.png", GL_LINEAR);
	newMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/white-quilted-diamond_metallic.png", GL_LINEAR);
	newMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/white-quilted-diamond_roughness.png", GL_LINEAR);
	newMaterial->SetRoughnessTexture(texture);
	g_Renderer->RegisterMaterial(newMaterial);

	newMaterial = std::make_shared<CMaterial>();
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/wrinkled-paper-albedo.png", GL_LINEAR);
	newMaterial->SetBaseTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/wrinkled-paper-metalness.png", GL_LINEAR);
	newMaterial->SetNormalTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/wrinkled-paper-normal-ogl.png", GL_LINEAR);
	newMaterial->SetMetallicTexture(texture);
	texture = std::make_shared<CTexture>();
	texture->LoadTextureFromPNG("./Textures/wrinkled-paper-roughness.png", GL_LINEAR);
	newMaterial->SetRoughnessTexture(texture);
	g_Renderer->RegisterMaterial(newMaterial);

	//m_pObjects.resize(nObj);
	glm::vec3 basePos{ 5.0f,-2.0f,0 };
	basePos *= 2.0f;

	std::shared_ptr<CDynamicObject> obj = std::make_shared<CDynamicObject>();
	obj->LoadGeometryAndAnimationFromFile("./Objects/TestModel.bin");

	//m_pObjects[i]->SetMaterial(g_Renderer->GetMaterialFromIndex(i % 6));
	obj->SetPosition(basePos);
	obj->AddAcceleration(glm::normalize(glm::vec3(-1.0f, 2.0f, 0.0f)), 6.5f);
	obj->SetRotateAxis(glm::vec3(0, 0, 1));
	obj->AddTorqueAcceleration(15.0f);

	baseObject.emplace_back(obj);

	obj = std::make_shared<CDynamicObject>();
	obj->LoadGeometryAndAnimationFromFile("./Objects/obstacle.bin");

	obj->SetPosition(basePos);
	obj->AddAcceleration(glm::normalize(glm::vec3(-1.0f, 2.0f, 0.0f)), 6.5f);
	obj->SetRotateAxis(glm::vec3(0, 0, 1));
	obj->AddTorqueAcceleration(15.0f);

	baseObject.emplace_back(obj);

	m_pbasket = std::make_shared<CObject>();
	m_pbasket->LoadGeometryAndAnimationFromFile("./Objects/basket.bin");

	m_vec3BasketRoute[0] = glm::vec3(5, -3, 0);
	m_vec3BasketRoute[1] = glm::vec3(-5, -3, 0);

	m_pbasket->SetPosition(m_vec3BasketRoute[0]);

	pointObj.resize(2);
	pointObj[0] = std::make_shared<CObject>();
	pointObj[0]->LoadGeometryAndAnimationFromFile("./Objects/TestModel.bin");
	pointObj[1] = std::make_shared<CObject>();
	pointObj[1]->LoadGeometryAndAnimationFromFile("./Objects/TestModel.bin");

	m_pMainCamera->SetPosision(glm::vec3(0, 0, -5.0f));
	m_pMainCamera->SetQauternion(glm::quat(1, 0, 0, 0));
}

void CSPScene::MouseInput(int button, int state, int x, int y)
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

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		m_startPos = { x,y };

	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		m_endPos = { x,y };



		p1 = { m_startPos.x, m_startPos.y, 1.0f,1.0f };
		p2 = { m_endPos.x, m_endPos.y, 1.0f,1.0f };
		p3 = { 0.0f, 0.0f, 0.0f,1.0f };

		p1.x = ((2.0f * p1.x / g_WindowSizeX) - 1.0f);
		p1.y = ((-2.0f * p1.y / g_WindowSizeY) + 1.0f);

		p2.x = ((2.0f * p2.x / g_WindowSizeX) - 1.0f);
		p2.y = ((-2.0f * p2.y / g_WindowSizeY) + 1.0f);



		p1 = glm::inverse(m_pMainCamera->m_mat4x4Projection) * p1;
		p1 /= p1.w;
		p1 = glm::inverse(m_pMainCamera->m_mat4x4View) * p1;
		p2 = glm::inverse(m_pMainCamera->m_mat4x4Projection) * p2;
		p2 /= p2.w;
		p2 = glm::inverse(m_pMainCamera->m_mat4x4View) * p2;

		glm::mat4x4 testMat = glm::lookAt(glm::vec3(0, 5, 0.000001), glm::vec3(0), glm::vec3(0, 1, 0));
		glm::mat4x4 inverseTestMat = glm::inverse(testMat);

		p3 = glm::vec4(m_pMainCamera->GetPosition(), 1.0f);



		glm::vec4 plane;


		if (m_pObjects.size() > 0) {
			std::vector<std::shared_ptr<CObject>> objectBuf;
			objectBuf = m_pObjects;
			m_pObjects.clear();
			glm::vec3 n;
			for (int i = 0; i < objectBuf.size(); ++i) {
				CObject* obj = objectBuf[i].get();

				glm::mat4x4 inverseWorld = glm::inverse(obj->m_mat4x4Wolrd);

				glm::vec3 a = inverseWorld * p1;
				glm::vec3 b = inverseWorld * p2;
				glm::vec3 c = inverseWorld * p3;

				glm::vec3 dir1 = a - c;
				glm::vec3 dir2 = b - c;

				dir1 = glm::normalize(dir1);
				dir2 = glm::normalize(dir2);

				n = glm::normalize(glm::cross(dir1, dir2));

				plane = glm::vec4(n.x, n.y, n.z, 0);
				plane.w = -(c.x * n.x + c.y * n.y + c.z * n.z);


				CMesh* mesh = nullptr;
				mesh = obj->GetMesh();
				if (mesh == nullptr) {
					m_pObjects.emplace_back(objectBuf[i]);
					continue;
				}

				std::vector<CMesh::Vertex> vertexs = mesh->GetVertexs();
				std::vector<UINT> indices = mesh->GetSubSetIndice(0);
				int nIndices = indices.size();

				/*std::vector<CMesh::Vertex> Upside;
				std::vector<CMesh::Vertex> Underside;*/

				std::vector<UINT> Upside;
				std::vector<UINT> Underside;

				std::vector<CMesh::Vertex> NewVertexs;
				UINT newVertexIndexOffset = vertexs.size();
				UINT IndexOffsetRegister = newVertexIndexOffset;


				if (nIndices > 0) {
					for (int i = 0; i < nIndices;) {
						CMesh::Vertex v[3];
						UINT vIndex[3];
						v[0] = vertexs[indices[i]];
						vIndex[0] = indices[i++];
						v[1] = vertexs[indices[i]];
						vIndex[1] = indices[i++];
						v[2] = vertexs[indices[i]];
						vIndex[2] = indices[i++];

						bool UpAndDowns[3];
						int nDistribution = 0;
						for (int j = 0; j < 3; ++j) {
							UpAndDowns[j] = Distance(plane, v[j].position) >= 0.0f;
							if (UpAndDowns[j]) {
								nDistribution++;
								Upside.emplace_back(vIndex[j]);
							}
							else
								Underside.emplace_back(vIndex[j]);
						}

						if (nDistribution != 3 && nDistribution != 0) {
							CMesh::Vertex v1, v2, v3;
							CMesh::Vertex C1, C2;
							UINT C1Index = IndexOffsetRegister++;
							UINT C2Index = IndexOffsetRegister++;
							if (nDistribution == 2) {
								UINT v1Index = Upside[Upside.size() - 2];
								UINT v2Index = Upside[Upside.size() - 1];
								UINT v3Index = Underside[Underside.size() - 1];
								v2 = vertexs[v2Index];
								v1 = vertexs[v1Index];
								v3 = vertexs[v3Index];

								float D = plane.w;
								glm::vec3 N{ plane.x,plane.y, plane.z };
								float t1 = (-D - glm::dot(N, v3.position)) / (glm::dot(N, (v1.position - v3.position)));
								C1.position = v3.position + t1 * (v1.position - v3.position);
								C1.normal = glm::normalize(v3.normal + ((v1.normal - v3.normal) * t1));
								C1.tangent = glm::normalize(v3.tangent + ((v1.tangent - v3.tangent) * t1));
								C1.bitangent = glm::normalize(v3.bitangent + ((v1.bitangent - v3.bitangent) * t1));
								C1.texcoord0 = glm::mix(v3.texcoord0, v1.texcoord0, t1);

								float t2 = (-D - glm::dot(N, v3.position)) / (glm::dot(N, (v2.position - v3.position)));
								C2.position = v3.position + t2 * (v2.position - v3.position);
								C2.normal = glm::normalize(v3.normal + ((v2.normal - v3.normal) * t2));
								C2.tangent = glm::normalize(v3.tangent + ((v2.tangent - v3.tangent) * t2));
								C2.bitangent = glm::normalize(v3.bitangent + ((v2.bitangent - v3.bitangent) * t2));
								C2.texcoord0 = glm::mix(v3.texcoord0, v2.texcoord0, t2);

								Upside.emplace_back(C1Index);
								Upside.emplace_back(v2Index);
								Upside.emplace_back(C1Index);
								Upside.emplace_back(C2Index);

								Underside.emplace_back(C1Index);
								Underside.emplace_back(C2Index);


							}
							else {
								UINT v1Index = Underside[Underside.size() - 2];
								UINT v2Index = Underside[Underside.size() - 1];
								UINT v3Index = Upside[Upside.size() - 1];
								v2 = vertexs[v2Index];
								v1 = vertexs[v1Index];
								v3 = vertexs[v3Index];

								float D = plane.w;
								glm::vec3 N{ plane.x,plane.y, plane.z };
								float t1 = (-D - glm::dot(N, v3.position)) / (glm::dot(N, (v1.position - v3.position)));
								C1.position = v3.position + t1 * (v1.position - v3.position);
								C1.normal = glm::normalize(v3.normal + ((v1.normal - v3.normal) * t1));
								C1.tangent = glm::normalize(v3.tangent + ((v1.tangent - v3.tangent) * t1));
								C1.bitangent = glm::normalize(v3.bitangent + ((v1.bitangent - v3.bitangent) * t1));
								C1.texcoord0 = glm::mix(v3.texcoord0, v1.texcoord0, t1);

								float t2 = (-D - glm::dot(N, v3.position)) / (glm::dot(N, (v2.position - v3.position)));
								C2.position = v3.position + t2 * (v2.position - v3.position);
								C2.normal = glm::normalize(v3.normal + ((v2.normal - v3.normal) * t2));
								C2.tangent = glm::normalize(v3.tangent + ((v2.tangent - v3.tangent) * t2));
								C2.bitangent = glm::normalize(v3.bitangent + ((v2.bitangent - v3.bitangent) * t2));
								C2.texcoord0 = glm::mix(v3.texcoord0, v2.texcoord0, t2);

								Underside.emplace_back(C1Index);

								Underside.emplace_back(v2Index);
								Underside.emplace_back(C1Index);
								Underside.emplace_back(C2Index);

								Upside.emplace_back(C1Index);
								Upside.emplace_back(C2Index);
							}
							NewVertexs.emplace_back(C1);
							NewVertexs.emplace_back(C2);
						}
					}
				}

				if (NewVertexs.size() > 0) {
					std::vector<CMesh::Vertex> mergedVertexs;
					mergedVertexs.reserve(vertexs.size() + NewVertexs.size() + 2);
					mergedVertexs = vertexs;
					for (CMesh::Vertex& v : NewVertexs) {
						mergedVertexs.emplace_back(v);
					}

					CMesh::Vertex Center;
					UINT CenterIndex = IndexOffsetRegister++;
					UINT CenterDownIndex = IndexOffsetRegister++;

					Center.position = glm::vec3(0, 0, 0);
					Center.normal = -glm::vec3(plane.x, plane.y, plane.z);
					for (CMesh::Vertex& v : NewVertexs) {
						Center.position += v.position;
					}
					Center.position /= NewVertexs.size();

					CMesh::Vertex CenterDown;
					CenterDown = Center;
					CenterDown.normal = -Center.normal;

					mergedVertexs.emplace_back(Center);
					mergedVertexs.emplace_back(CenterDown);

					for (int i = 0; i < NewVertexs.size() - 1; i += 2) {
						UINT v2Index = newVertexIndexOffset + i;
						UINT v3Index = newVertexIndexOffset + i + 1;
						CMesh::Vertex v2 = mergedVertexs[v2Index];
						CMesh::Vertex v3 = mergedVertexs[v3Index];

						v2.normal = Center.normal;
						v3.normal = Center.normal;
						Upside.emplace_back(CenterIndex);
						Upside.emplace_back(v2Index);
						Upside.emplace_back(v3Index);

						v2.normal = CenterDown.normal;
						v3.normal = CenterDown.normal;
						Underside.emplace_back(CenterDownIndex);
						Underside.emplace_back(v2Index);
						Underside.emplace_back(v3Index);
					}


					std::shared_ptr<CMesh> newMesh1 = std::make_shared<CMesh>();
					newMesh1->SetVertexs(mergedVertexs);
					newMesh1->SetSubmesh(Upside);
					newMesh1->CreateShaderVariables();


					std::shared_ptr<CMesh> newMesh2 = std::make_shared<CMesh>();
					newMesh2->SetVertexs(mergedVertexs);
					newMesh2->SetSubmesh(Underside);
					newMesh2->CreateShaderVariables();

					glm::vec3 objPos = obj->GetPosition();

					std::shared_ptr<CDynamicObject> newObj1 = std::make_shared<CDynamicObject>();
					newObj1->SetMaterial(obj->GetMaterial(0));
					newObj1->SetName(obj->GetName());
					newObj1->SetPosition(objPos);


					std::shared_ptr<CDynamicObject> newObj2 = std::make_shared<CDynamicObject>();
					newObj2->SetMaterial(obj->GetMaterial(0));
					newObj2->SetName(obj->GetName());
					newObj2->SetPosition(objPos);

					if (dynamic_cast<CDynamicObject*>(obj)) {
						newObj1->GetPhysics() = dynamic_cast<CDynamicObject*>(obj)->GetPhysics();
						newObj1->ScaleLinearVelocity(0.2f);
						newObj1->ScaleLinearAcceleration(0.5f);
						newObj1->AddAcceleration(glm::vec3(n), 1.5f);
						newObj1->SetRotate(obj->GetRotation());
						newObj2->GetPhysics() = dynamic_cast<CDynamicObject*>(obj)->GetPhysics();
						newObj2->ScaleLinearVelocity(0.2f);
						newObj2->ScaleLinearAcceleration(0.5f);
						newObj2->SetRotate(obj->GetRotation());
						newObj2->AddAcceleration(glm::vec3(n), -1.5f);
					}

					newObj1->SetMesh(newMesh1);
					newObj2->SetMesh(newMesh2);

					m_pObjects.emplace_back(newObj1);
					m_pObjects.emplace_back(newObj2);
				}
				else {
					m_pObjects.emplace_back(objectBuf[i]);
				}
			}
			//m_pObjects = objectBuf;
			//m_pObjects.clear();
			/*for (int i = 0; i < objectBuf.size(); ++i) {
				std::shared_ptr<CObject> o = objectBuf[i];
				m_pObjects.emplace_back(objectBuf[i]);
			}*/
		}
	}
}

void CSPScene::Update(float fElapsedTime)
{
	//CPBR_TestScene::Update(fElapsedTime);
	float fTime = g_Timer->GetTotalTime();

	m_pSunLight->m_vec3LightColor = glm::vec3(1, 1, 1);
	m_pSunLight->m_vec3Direction = glm::normalize(glm::vec3(-1, -1, -1));
	m_pSunLight->m_vec3Direction = glm::vec3(sin(fTime), cos(fTime), -1);
	m_pSunLight->m_vec3Position = (-m_pSunLight->m_vec3Direction) * 10.0f;

	m_fBasketDT += fElapsedTime;

	glm::vec3 vec3DbasketMove;
	if (m_fBasketDT < 1.0f) {
		glm::vec3 basketPos = m_pbasket->GetPosition();
		vec3DbasketMove = basketPos;
		basketPos = glm::mix(m_vec3BasketRoute[0], m_vec3BasketRoute[1], m_fBasketDT);
		m_pbasket->SetPosition(basketPos);
		vec3DbasketMove = basketPos - vec3DbasketMove;
	}
	else {
		vec3DbasketMove = m_pbasket->GetPosition();
		m_pbasket->SetPosition(m_vec3BasketRoute[1]);
		glm::vec3 temp = m_vec3BasketRoute[1];
		m_vec3BasketRoute[1] = m_vec3BasketRoute[0];
		m_vec3BasketRoute[0] = temp;
		m_fBasketDT = 0.0f;
		vec3DbasketMove = temp - vec3DbasketMove;
	}


	std::vector<bool> collisionObjectIndexs;
	collisionObjectIndexs.resize(m_pObjects.size());
	for (int i = 0; i < m_pObjects.size(); ++i) {
		m_pObjects[i]->Update(fElapsedTime);
		glm::vec3 objPos = m_pObjects[i]->GetPosition();


		glm::vec3 basketPos = m_pbasket->GetPosition();
		glm::vec2 basketOffset{ 2.5f / 2,2.5f / 2 };
		if (basketPos.y <= objPos.y && objPos.y < basketPos.y + 0.5f) {
			if (basketPos.x - basketOffset.x <= objPos.x && objPos.x <= basketPos.x + basketOffset.x) {
				if (basketPos.z - basketOffset.y <= objPos.z && objPos.z <= basketPos.z + basketOffset.y) {
					collisionObjectIndexs[i] = true;
				}
			}
		}
	}

	for (int i = 0; i < m_pInbasketObjects.size(); ++i) {
		m_pInbasketObjects[i]->SetPosition(m_pInbasketObjects[i]->GetPosition() + vec3DbasketMove);
	}

	std::vector<std::shared_ptr<CObject>> buf = m_pObjects;
	m_pObjects.clear();
	for (int i = 0; i < buf.size(); ++i) {
		if (collisionObjectIndexs[i] == true) {
			m_pInbasketObjects.emplace_back(buf[i]);
		}
		else {
			if (buf[i]->GetPosition().y >= -20.0f)
				m_pObjects.emplace_back(buf[i]);
		}
	}

	static float spawnCooltime = 1.0f;
	spawnCooltime -= fElapsedTime;
	if (spawnCooltime < 0.0f) {
		int baseIndex = urd_material(dre) % 2;
		int startRandom = urd_material(dre) % 2;
		std::shared_ptr<CDynamicObject> newObj = std::make_shared<CDynamicObject>();
		std::shared_ptr<CMesh> mesh = baseObject[baseIndex]->GetMeshByShared();
		std::shared_ptr<CMaterial> material = g_Renderer->GetMaterialFromIndex(urd_material(dre) % 6);
		newObj->SetMesh(mesh);
		newObj->SetMaterial(material);
		newObj->GetPhysics() = baseObject[baseIndex]->GetPhysics();
		newObj->SetPosition(baseObject[baseIndex]->GetPosition());
		if (startRandom == 1) {
			glm::vec3 pos = newObj->GetPosition();
			pos.x = -pos.x;
			newObj->SetPosition(pos);
			glm::vec3 accel = newObj->GetPhysics().GetLinearAcceleration();
			accel.x = -accel.x;
			newObj->SetLinearAcceleration(accel);
		}
		newObj->ScaleLinearAcceleration(urd_velocityScale(dre) * m_fCorrectionSpeed);

		glm::vec3 axis = glm::vec3(urd_rotate(dre), urd_rotate(dre), urd_rotate(dre));
		axis = glm::normalize(axis);
		newObj->SetRotateAxis(axis);

		m_pObjects.emplace_back(newObj);
		spawnCooltime = 1.0f;
	}
}

void CSPScene::RenderScene()
{
	CPBR_TestScene::RenderScene();

	pointObj[0]->SetPosition(p1);
	pointObj[1]->SetPosition(p2);


	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program);

	for (int i = 0; i < pointObj.size(); ++i) {
		pointObj[i]->BindShaderVariables(s_Program);
		pointObj[i]->UpdateTransform(nullptr);
		pointObj[i]->Render(s_Program);
	}

	

	for (int i = 0; i < m_pInbasketObjects.size(); ++i) {
		m_pInbasketObjects[i]->BindShaderVariables(s_Program);
		m_pInbasketObjects[i]->UpdateTransform(nullptr);
		m_pInbasketObjects[i]->Render(s_Program);
	}

	m_pbasket->BindShaderVariables(s_Program);
	m_pbasket->UpdateTransform(nullptr);
	m_pbasket->Render(s_Program);

	//GLuint lineVBO;
	//glGenBuffers(1, &lineVBO);

	//glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	//CMesh::Vertex lineVertexs[2];
	//lineVertexs[0].position = p1;
	//lineVertexs[1].position = p2;
	//glBufferData(GL_ARRAY_BUFFER, sizeof(lineVertexs), lineVertexs, GL_STATIC_DRAW);

	//GLuint s_program = g_Renderer->LineShader;

	//GLuint posLoc = glGetAttribLocation(s_program, "v_Position");
	//glEnableVertexAttribArray(posLoc);
	//glBindBuffer(GL_ARRAY_BUFFER, lineVBO);
	//glVertexAttribPointer(posLoc, 3, GL_FLOAT, GL_FALSE, sizeof(CMesh::Vertex), 0);

	//glDrawArrays(GL_LINES, 0, 2);
}

void CSPScene::KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'l':
		m_ePolygonMode = GL_LINE;
		break;
	case 'f':
		m_ePolygonMode = GL_FILL;
		break;
	case '+':
		m_fCorrectionSpeed = std::min(2.0f, m_fCorrectionSpeed + 0.1f);
		break;
	case '-':
		m_fCorrectionSpeed = std::max(0.5f, m_fCorrectionSpeed - 0.1f);
		break;
	case 'q':
		glutLeaveMainLoop();
		break;
	default:
		break;
	}
	glm::vec3 pos = m_pMainCamera->GetPosition();
	std::cout << pos.x << " " << pos.y << " " << pos.z << std::endl;
}

CExamScene_20::CExamScene_20()
{
}

CExamScene_20::~CExamScene_20()
{
}

void CExamScene_20::Init()
{
	CPBR_TestScene::Init();

	cameraSpring = glm::vec3(1, 1, 1) * 2.0f;

	m_pMainCamera->SetPosision(cameraSpring);
	m_pMainCamera->m_mat4x4View = glm::lookAt(m_pMainCamera->GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	for (int i = 0; i < 3; ++i) {
		std::shared_ptr<CObject> obj = std::make_shared<CObject>();
		std::shared_ptr<CMaterial> material = std::make_shared<CMaterial>();

		if (i == 0)
			material->BaseColor = glm::vec3(1, 0, 0);

		if (i == 1)
			material->BaseColor = glm::vec3(0, 0, 1);

		if (i == 2)
			material->BaseColor = glm::vec3(0, 1, 0);

		material->RoughnessColor = 0.0f;

		std::shared_ptr<CMesh> mesh;

		mesh = CMesh::CreateLineMesh();

		mesh->CreateShaderVariables();
		obj->SetMesh(mesh);
		obj->SetMaterial(material);
		obj->SetScale(glm::vec3(2, 2, 2));

		if (i == 1) {
			obj->RotationQuat(glm::radians(-90.f), glm::vec3(0, 1, 0));
			obj->RotationQuat(glm::radians(-90.f), glm::vec3(1, 0, 0));
		}
		if (i == 2) {
			obj->RotationQuat(glm::radians(90.f), glm::vec3(0, 1, 0));
		}

		m_pObjects.push_back(obj);
	}

	std::string name("Right_Arm_Joint");
	CObject* obj = CObject::FindFrameByName(m_pObjects[0].get(), name);

	posBarrel = obj->GetPosition();


}

void CExamScene_20::RenderScene()
{
	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);



	glViewport(0, 0, m_nShadowMapWidth, m_nShadowMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOShadowDepth);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Render ShadowDepthMap
	CShadowCamera shadowCamera;
	//shadowCamera.GenerateProjectionMatrix(glm::radians(90.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);
	float fNear = 1.0f, fFar = 7.5f;
	shadowCamera.m_mat4x4Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, fNear, fFar);
	shadowCamera.SetPosision(m_pSunLight->m_vec3Position);
	shadowCamera.m_mat4x4View = glm::lookAt(m_pSunLight->m_vec3Position, m_pSunLight->m_vec3Direction, glm::vec3(0, 1, 0));
	shadowCamera.BindShaderVariables(s_Program);


	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->UpdateTransform(nullptr);
		obj->Render(s_Program);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);





	BindShaderVariables(s_Program);

	GLuint samplerULoc = glGetUniformLocation(s_Program, "u_IrradianceTexture");
	glUniform1i(samplerULoc, 5);
	m_tIrradianceTexture->BindShaderVariables(s_Program, GL_TEXTURE5);

	samplerULoc = glGetUniformLocation(s_Program, "u_BrdfLUT");
	glUniform1i(samplerULoc, 6);
	m_tPreCoumputedBRDFLUTexture->BindShaderVariables(s_Program, GL_TEXTURE6);

	samplerULoc = glGetUniformLocation(s_Program, "u_PreFilterMap");
	glUniform1i(samplerULoc, 7);
	m_tFilteringedEnvironmentTexture->BindShaderVariables(s_Program, GL_TEXTURE7);





	//m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program, false);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->UpdateTransform(nullptr);
		obj->Render(s_Program);
	}

	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	//m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program, false);


	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->UpdateTransform(nullptr);
	m_pSkyBoxObject->Render(s_Program);
}

void CExamScene_20::BuildObjects()
{
	m_pObjects.resize(1);
	m_pObjects[0] = std::make_shared<CObject>();
	m_pObjects[0]->LoadGeometryAndAnimationFromFile("./Objects/Crain.bin");
}

void CExamScene_20::KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'b':
		MoveBase = MoveBase == 1 ? 0 : 1;
		break;
	case 'B':
		MoveBase = MoveBase == -1 ? 0 : -1;
		break;
	case 'm':
		MoveTop = MoveTop == 1 ? 0 : 1;
		break;
	case 'M':
		MoveTop = MoveTop == -1 ? 0 : -1;
		break;
	case 'f':
		RotateBarrel = RotateBarrel == 1 ? 0 : 1;
		break;
	case 'F':
		RotateBarrel = RotateBarrel == -1 ? 0 : -1;
		break;
	case 'e':
		MoveBarrel = MoveBarrel == 1 ? 0 : 1;
		break;
	case 'E':
		MoveBarrel = MoveBarrel == -1 ? 0 : -1;
		break;
	case 't':
		RotateArm = RotateArm == 1 ? 0 : 1;
		break;
	case 'T':
		RotateArm = RotateArm == -1 ? 0 : -1;
		break;
	case 'x':
		cameraLookAt = true;
		cameraMoveX = cameraMoveX == 1 ? 0 : 1;
		break;
	case 'X':
		cameraLookAt = true;
		cameraMoveX = cameraMoveX == -1 ? 0 : -1;
		break;
	case 'y':
		cameraLookAt = true;
		cameraMoveY = cameraMoveY == 1 ? 0 : 1;
		break;
	case 'Y':
		cameraLookAt = true;
		cameraMoveY = cameraMoveY == -1 ? 0 : -1;
		break;
	case 'z':
		cameraLookAt = true;
		cameraMoveZ = cameraMoveZ == 1 ? 0 : 1;
		break;
	case 'Z':
		cameraLookAt = true;
		cameraMoveZ = cameraMoveZ == -1 ? 0 : -1;
		break;
	case 'r':
		cameraLookAt = false;
		cameraRevolution = cameraRevolution == 1 ? 0 : 1;
		break;
	case 'R':
		cameraLookAt = false;
		cameraRevolution = cameraRevolution == -1 ? 0 : -1;
		break;
	case 'a':
	case 'A':
		cameraLookAt = true;
		break;
	case 'c':
	case 'C':
	{
		std::string name("Crain_Base");
		CObject* obj = CObject::FindFrameByName(m_pObjects[0].get(), name);
		obj->SetRotate(glm::quat(1, 0, 0, 0));

		name = "Crain_Top";
		obj = CObject::FindFrameByName(m_pObjects[0].get(), name);
		obj->SetRotate(glm::quat(1, 0, 0, 0));

		name = "Right_Arm_Joint";
		obj = CObject::FindFrameByName(m_pObjects[0].get(), name);
		obj->SetRotate(glm::quat(1, 0, 0, 0));
		obj->SetPosition(posBarrel);

		name = "Left_Arm_Joint";
		obj = CObject::FindFrameByName(m_pObjects[0].get(), name);
		obj->SetRotate(glm::quat(1, 0, 0, 0));
		obj->SetPosition(glm::vec3(-posBarrel.x, posBarrel.y, posBarrel.z));

		name = "Right_Top_Joint";
		obj = CObject::FindFrameByName(m_pObjects[0].get(), name);
		obj->SetRotate(glm::quat(1, 0, 0, 0));

		name = "Left_Top_Joint";
		obj = CObject::FindFrameByName(m_pObjects[0].get(), name);
		obj->SetRotate(glm::quat(1, 0, 0, 0));

		cameraSpring = glm::vec3(1, 1, 1) * 2.0f;
	}
	case 's':
	case 'S':
		MoveBase = 0;

		MoveTop = 0;

		RotateBarrel = 0;

		MoveBarrel = 0;

		RotateArm = 0;

		cameraMoveX = 0;
		cameraMoveY = 0;
		cameraMoveZ = 0;

		cameraRevolution = 0;
		cameraLookAt = true;

		break;
	default:
		break;
	}
}

void CExamScene_20::Update(float fElapsedTime)
{

	UpdateCameraSpring();
	/*else {
		m_pMainCamera->m_mat4x4View = glm::lookAt(m_pMainCamera->GetPosition(), m_pMainCamera->GetPosition() + glm::vec3(m_pMainCamera->m_mat4x4View[0][2],
			m_pMainCamera->m_mat4x4View[1][2],
			m_pMainCamera->m_mat4x4View[2][2])
			, glm::vec3(0, 1, 0));
	}*/

	if (MoveBase != 0) {
		std::string name("Crain_Base");
		CObject* obj = CObject::FindFrameByName(m_pObjects[0].get(), name);

		glm::vec3 axis(0, 1, 0);
		float degree = 90.0f;
		obj->RotationQuat(glm::radians(degree * MoveBase * fElapsedTime), axis);
	}
	if (MoveTop != 0) {
		std::string name("Crain_Top");
		CObject* obj = CObject::FindFrameByName(m_pObjects[0].get(), name);

		glm::vec3 axis(0, 1, 0);
		float degree = 90.0f;
		obj->RotationQuat(glm::radians(degree * MoveTop * fElapsedTime), axis);
	}

	if (RotateBarrel != 0) {
		std::string name("Right_Arm_Joint");
		CObject* obj = CObject::FindFrameByName(m_pObjects[0].get(), name);

		glm::vec3 axis(0, 1, 0);
		float degree = 90.0f;
		obj->RotationQuat(glm::radians(degree * RotateBarrel * fElapsedTime), axis);


		name = std::string("Left_Arm_Joint");
		obj = CObject::FindFrameByName(m_pObjects[0].get(), name);

		obj->RotationQuat(glm::radians(degree * -RotateBarrel * fElapsedTime), axis);
	}

	if (MoveBarrel == 1) {
		std::string name("Right_Arm_Joint");
		CObject* obj1 = CObject::FindFrameByName(m_pObjects[0].get(), name);

		glm::vec3 pos1 = obj1->GetPosition();
		glm::vec3 dir{ 1,0,0 };

		name = std::string("Left_Arm_Joint");
		CObject* obj2 = CObject::FindFrameByName(m_pObjects[0].get(), name);
		glm::vec3 pos2 = obj2->GetPosition();

		if (pos1.x < 0.0f) {
			MoveBarrel = 0;
			pos1.x = 0;
			pos2.x = 0;
		}

		obj1->SetPosition(pos1 + dir * glm::vec3(-MoveBarrel) * glm::vec3(fElapsedTime));
		obj2->SetPosition(pos2 + dir * glm::vec3(MoveBarrel) * glm::vec3(fElapsedTime));
	}

	if (MoveBarrel == -1) {
		std::string name("Right_Arm_Joint");
		CObject* obj1 = CObject::FindFrameByName(m_pObjects[0].get(), name);

		glm::vec3 pos1 = obj1->GetPosition();
		glm::vec3 dir{ 1,0,0 };

		name = std::string("Left_Arm_Joint");
		CObject* obj2 = CObject::FindFrameByName(m_pObjects[0].get(), name);
		glm::vec3 pos2 = obj2->GetPosition();

		if (pos1.x > 0.3f) {
			MoveBarrel = 0;
			pos1.x = 0.3;
			pos2.x = -0.3;
		}

		obj1->SetPosition(pos1 + dir * glm::vec3(-MoveBarrel) * glm::vec3(fElapsedTime));
		obj2->SetPosition(pos2 + dir * glm::vec3(MoveBarrel) * glm::vec3(fElapsedTime));
	}

	if (RotateArm != 0) {
		std::string name("Right_Top_Joint");
		CObject* obj1 = CObject::FindFrameByName(m_pObjects[0].get(), name);

		glm::vec3 axis(0, 0, 1);
		float degree = 90.0f;

		name = std::string("Left_Top_Joint");
		CObject* obj2 = CObject::FindFrameByName(m_pObjects[0].get(), name);

		obj1->RotationQuat(glm::radians(degree * RotateArm * fElapsedTime), axis);
		obj2->RotationQuat(glm::radians(degree * -RotateArm * fElapsedTime), axis);
	}

	if (cameraMoveX != 0) {
		glm::vec3 dir(1, 0, 0);
		cameraSpring += dir * static_cast<float>(cameraMoveX) * fElapsedTime;
	}
	if (cameraMoveY != 0) {
		glm::vec3 dir(0, 1, 0);
		cameraSpring += dir * static_cast<float>(cameraMoveY) * fElapsedTime;
	}
	if (cameraMoveZ != 0) {
		glm::vec3 dir(0, 0, 1);
		cameraSpring += dir * static_cast<float>(cameraMoveZ) * fElapsedTime;
	}
	if (cameraRevolution != 0) {
		glm::vec3 axis(0, 1, 0);
		float degree = 90.0f;

		glm::mat4x4 mat = glm::mat4_cast(glm::rotate(glm::quat(1, 0, 0, 0), glm::radians(degree * cameraRevolution * fElapsedTime), axis));

		cameraSpring = glm::vec3(mat * glm::vec4(cameraSpring, 0.0f));
	}
}

void CExamScene_20::UpdateCameraSpring()
{
	m_pMainCamera->SetPosision(cameraSpring);
	//if(cameraLookAt == true)
	m_pMainCamera->m_mat4x4View = glm::lookAt(m_pMainCamera->GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

CExamScene_21::CExamScene_21()
{
}

CExamScene_21::~CExamScene_21()
{
}

void CExamScene_21::Init()
{
	CExamScene_20::Init();

	std::shared_ptr<CCamera> pCamera;
	pCamera = std::make_shared<CCamera>();

	float fNear = 0.0f, fFar = 5.0f;
	pCamera->m_mat4x4Projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, fNear, fFar);
	otherCameraSpring[1] = glm::normalize(glm::vec3(0.00001f, 1.0f, 0.0f));
	pCamera->SetPosision(otherCameraSpring[1]);
	pCamera->m_mat4x4View = glm::lookAt(pCamera->GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_sptrCameras.push_back(std::move(pCamera));


	pCamera = std::make_shared<CCamera>();
	pCamera->m_mat4x4Projection = glm::ortho(-5.0f, 5.0f, -5.0f, 5.0f, fNear, fFar);
	otherCameraSpring[2] = glm::vec3(0.0f, 0.0f, 1.0f);
	pCamera->SetPosision(otherCameraSpring[2]);
	pCamera->m_mat4x4View = glm::lookAt(pCamera->GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_sptrCameras.push_back(std::move(pCamera));
}

void CExamScene_21::KeyInput(unsigned char key, int x, int y)
{
	CExamScene_20::KeyInput(key, x, y);

	switch (key)
	{
	case '1':
		m_pMainCamera = m_sptrCameras[0].get();
		currentCameraIndex = 0;
		break;
	case '2':
		m_pMainCamera = m_sptrCameras[1].get();
		currentCameraIndex = 1;
		break;
	case '3':
		m_pMainCamera = m_sptrCameras[2].get();
		currentCameraIndex = 2;
		break;
	default:
		break;
	}
}

void CExamScene_21::UpdateCameraSpring()
{
	otherCameraSpring[0] = cameraSpring;
	m_pMainCamera->SetPosision(otherCameraSpring[currentCameraIndex]);
	m_pMainCamera->m_mat4x4View = glm::lookAt(m_pMainCamera->GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
}

CExamScene_22::CExamScene_22()
{
	m_vec3RobotDir = glm::vec3(0);
	m_DwRobotDir = 0;
}

CExamScene_22::~CExamScene_22()
{
}

void CExamScene_22::Init()
{
	CPBR_TestScene::Init();
}

void CExamScene_22::KeyInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'o':
		b_OpenDoor = true;
		break;
	case 'w':
		m_DwRobotDir |= DW_FRONT;
		break;
	case 's':
		m_DwRobotDir |= DW_BACK;
		break;
	case 'd':
		m_DwRobotDir |= DW_RIGHT;
		break;
	case 'a':
		m_DwRobotDir |= DW_LEFT;
		break;
	case '+':
		m_fRobotSpeed = std::min(m_fMaxRobotSpeed, m_fRobotSpeed + 0.1f);
		break;
	case '-':
		m_fRobotSpeed = std::max(m_fMinRobotSpeed, m_fRobotSpeed - 0.1f);
		break;
	case 'j':
		m_fJumpVelocity = 9.8f;
		break;
	case 'i':
		cameraSpring = BaseCameraSpring;
		m_Robot->SetRotate(glm::quat(1, 0, 0, 0));
		m_Robot->SetPosition(glm::vec3(0, 3, 0));
		R_Arm_Joint->SetRotate(glm::quat(1, 0, 0, 0));
		L_Arm_Joint->SetRotate(glm::quat(1, 0, 0, 0));
		R_Leg_Joint->SetRotate(glm::quat(1, 0, 0, 0));
		L_Leg_Joint->SetRotate(glm::quat(1, 0, 0, 0));
		R_DoorFrame->SetPosition(BaseDoorPos[0]);
		L_DoorFrame->SetPosition(BaseDoorPos[1]);

		cameraMoveX = 0;
		cameraMoveY = 0;
		cameraMoveZ = 0;

		m_DwRobotDir = 0;
		m_fRobotSpeed = 1.0f;
		m_vec3RobotDir = glm::vec3(0);
		break;
	default:
		break;
	}
}

void CExamScene_22::Update(float fElapsedTime)
{
	m_pMainCamera->SetPosision(cameraSpring);
	m_pMainCamera->m_mat4x4View = glm::lookAt(m_pMainCamera->GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));

	m_fCurrentTime += fElapsedTime;

	if (b_OpenDoor) {
		if (L_DoorFrame->GetPosition().x >= 7.45f) {
			b_OpenDoor = false;
		}

		glm::vec3 dir{ 1,0,0 };

		L_DoorFrame->SetPosition(L_DoorFrame->GetPosition() + (dir * fElapsedTime));
		R_DoorFrame->SetPosition(R_DoorFrame->GetPosition() + (-dir * fElapsedTime));
	}

	if (m_DwRobotDir != 0) {
		if (m_DwRobotDir & DW_FRONT)
			m_vec3RobotDir -= glm::vec3(0, 0, 1);
		if (m_DwRobotDir & DW_BACK)
			m_vec3RobotDir += glm::vec3(0, 0, 1);
		if (m_DwRobotDir & DW_RIGHT)
			m_vec3RobotDir += glm::vec3(1, 0, 0);
		if (m_DwRobotDir & DW_LEFT)
			m_vec3RobotDir -= glm::vec3(1, 0, 0);
		if (glm::all(glm::isnan(m_vec3RobotDir)))
			m_vec3RobotDir = glm::vec3(0, 0, 0);
		else
			m_vec3RobotDir = glm::normalize(m_vec3RobotDir);


		glm::quat rotate{ 1,0,0,0 };
		rotate = glm::quatLookAt(-m_vec3RobotDir, glm::vec3(0, 1, 0));
		m_Robot->SetRotate(rotate);
	}

	if (glm::length(m_vec3RobotDir)) {
		float fcos = glm::cos(m_fCurrentTime);

		glm::quat rotate{ 1,0,0,0 };
		float degree = glm::radians(m_fBaseMoveDegree * m_fRobotSpeed);
		degree *= fcos;

		rotate = glm::angleAxis(degree, glm::vec3(1, 0, 0));
		R_Arm_Joint->SetRotate(rotate);
		L_Leg_Joint->SetRotate(rotate);

		rotate = glm::angleAxis(degree, glm::vec3(-1, 0, 0));
		L_Arm_Joint->SetRotate(rotate);
		R_Leg_Joint->SetRotate(rotate);

	}


	glm::vec3 robotPos = m_Robot->GetPosition();
	if (glm::any(glm::isnan(robotPos)))
		robotPos = glm::vec3(0, 0, 0);
	glm::vec3 prevPos = robotPos;
	robotPos = robotPos + (m_vec3RobotDir * fElapsedTime * m_fRobotSpeed);
	robotPos.y = robotPos.y + (m_fJumpVelocity * fElapsedTime);
	robotPos.y = std::max(m_fFloor, robotPos.y);
	robotPos.x = std::min(std::max(-10.f, robotPos.x), 10.0f);
	robotPos.z = std::min(std::max(-10.f, robotPos.z), 10.0f);
	m_fJumpVelocity += fElapsedTime * -9.8f;

	m_Robot->SetPosition(robotPos);

	if (CheckCollision()) {
		m_Robot->SetPosition(prevPos);
	}

	if (cameraMoveX != 0) {
		glm::vec3 dir(1, 0, 0);
		cameraSpring += dir * static_cast<float>(cameraMoveX) * fElapsedTime;
	}
	if (cameraMoveY != 0) {
		glm::vec3 axis(0, 1, 0);
		float degree = 90.0f;

		glm::mat4x4 mat = glm::mat4_cast(glm::rotate(glm::quat(1, 0, 0, 0), glm::radians(degree * cameraMoveY * fElapsedTime), axis));

		cameraSpring = glm::vec3(mat * glm::vec4(cameraSpring, 0.0f));
	}
	if (cameraMoveZ != 0) {
		glm::vec3 dir(0, 0, 1);
		cameraSpring += dir * static_cast<float>(cameraMoveZ) * fElapsedTime;
	}


	m_pSunLight->m_vec3LightColor = glm::vec3(1, 1, 1);
	m_pSunLight->m_vec3Direction = glm::normalize(glm::vec3(-1, -1, -1));
	m_pSunLight->m_vec3Direction = glm::vec3(sin(m_fCurrentTime), cos(m_fCurrentTime), -1);
	m_pSunLight->m_vec3Position = (-m_pSunLight->m_vec3Direction) * 10.0f;
}

void CExamScene_22::BuildObjects()
{
	std::uniform_real_distribution <float> urd_pos{ -10,10 };
	m_pObjects.resize(2);
	m_pObjects[0] = std::make_shared<CObject>();
	m_pObjects[0]->LoadGeometryAndAnimationFromFile("./Objects/Robot.bin");

	m_Robot = m_pObjects[0].get();

	glm::vec3 pos = m_Robot->GetPosition();
	m_Robot->SetPosition(glm::vec3(pos.x, 3.0f, pos.z));

	m_pObjects[1] = std::make_shared<CObject>();
	m_pObjects[1]->LoadGeometryAndAnimationFromFile("./Objects/stage.bin");

	std::string frameName = "Door_R";
	R_DoorFrame = CObject::FindFrameByName(m_pObjects[1].get(), frameName);

	frameName = "Door_L";
	L_DoorFrame = CObject::FindFrameByName(m_pObjects[1].get(), frameName);
	BaseDoorPos[0] = R_DoorFrame->GetPosition();
	BaseDoorPos[1] = L_DoorFrame->GetPosition();

	frameName = "R_Arm_Joint";
	R_Arm_Joint = CObject::FindFrameByName(m_pObjects[0].get(), frameName);
	frameName = "L_Arm_Joint";
	L_Arm_Joint = CObject::FindFrameByName(m_pObjects[0].get(), frameName);
	frameName = "R_Leg_Joint";
	R_Leg_Joint = CObject::FindFrameByName(m_pObjects[0].get(), frameName);
	frameName = "L_Leg_Joint";
	L_Leg_Joint = CObject::FindFrameByName(m_pObjects[0].get(), frameName);


	for (int i = 0; i < 3; ++i) {
		std::shared_ptr<CObject> obj = std::make_shared<CObject>();
		obj->LoadGeometryAndAnimationFromFile("./Objects/obstacle.bin");
		obj->SetPosition(glm::vec3(urd_pos(dre), 0.5f, urd_pos(dre)));
		m_pObjects.emplace_back(std::move(obj));
		m_obstacles.emplace_back(m_pObjects.back().get());
	}

	cameraSpring = glm::vec3(0, 8, 15);
	BaseCameraSpring = cameraSpring;

	m_pMainCamera->SetPosision(cameraSpring);
	m_pMainCamera->m_mat4x4View = glm::lookAt(m_pMainCamera->GetPosition(), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0));
	// camera pos 0.409097 8.71877 21.2325
}

void CExamScene_22::KeyUpInput(unsigned char key, int x, int y)
{
	switch (key)
	{
	case 'w':
		m_DwRobotDir &= (~DW_FRONT);
		break;
	case 's':
		m_DwRobotDir &= (~DW_BACK);
		break;
	case 'd':
		m_DwRobotDir &= (~DW_RIGHT);
		break;
	case 'a':
		m_DwRobotDir &= (~DW_LEFT);
		break;
	case 'x':
		cameraMoveX = cameraMoveX == 1 ? 0 : 1;
		break;
	case 'X':
		cameraMoveX = cameraMoveX == -1 ? 0 : -1;
		break;
	case 'y':
		cameraMoveY = cameraMoveY == 1 ? 0 : 1;
		break;
	case 'Y':
		cameraMoveY = cameraMoveY == -1 ? 0 : -1;
		break;
	case 'z':
		cameraMoveZ = cameraMoveZ == 1 ? 0 : 1;
		break;
	case 'Z':
		cameraMoveZ = cameraMoveZ == -1 ? 0 : -1;
		break;
	}
}

void CExamScene_22::RenderScene()
{
	GLuint s_Program = g_Renderer->TestShader;
	glUseProgram(s_Program);



	glViewport(0, 0, m_nShadowMapWidth, m_nShadowMapHeight);
	glBindFramebuffer(GL_FRAMEBUFFER, m_FBOShadowDepth);
	glClear(GL_DEPTH_BUFFER_BIT);

	// Render ShadowDepthMap
	CShadowCamera shadowCamera;
	//shadowCamera.GenerateProjectionMatrix(glm::radians(90.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);
	float fNear = 1.0f, fFar = 7.5f;
	shadowCamera.m_mat4x4Projection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, fNear, fFar);
	shadowCamera.SetPosision(m_pSunLight->m_vec3Position);
	shadowCamera.m_mat4x4View = glm::lookAt(m_pSunLight->m_vec3Position, m_pSunLight->m_vec3Direction, glm::vec3(0, 1, 0));
	shadowCamera.BindShaderVariables(s_Program);


	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->UpdateTransform(nullptr);
		obj->Render(s_Program);
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);





	BindShaderVariables(s_Program);

	GLuint samplerULoc = glGetUniformLocation(s_Program, "u_IrradianceTexture");
	glUniform1i(samplerULoc, 5);
	m_tIrradianceTexture->BindShaderVariables(s_Program, GL_TEXTURE5);

	samplerULoc = glGetUniformLocation(s_Program, "u_BrdfLUT");
	glUniform1i(samplerULoc, 6);
	m_tPreCoumputedBRDFLUTexture->BindShaderVariables(s_Program, GL_TEXTURE6);

	samplerULoc = glGetUniformLocation(s_Program, "u_PreFilterMap");
	glUniform1i(samplerULoc, 7);
	m_tFilteringedEnvironmentTexture->BindShaderVariables(s_Program, GL_TEXTURE7);





	//m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program, false);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->UpdateTransform(nullptr);
		obj->Render(s_Program);
	}

	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	//m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program, false);


	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->UpdateTransform(nullptr);
	m_pSkyBoxObject->Render(s_Program);
}

bool CExamScene_22::CheckCollision()
{
	m_fFloor = 0.0f;

	for (int i = 0; i < m_obstacles.size(); ++i) {
		glm::vec3 obsPos = m_obstacles[i]->GetPosition();
		glm::vec4 xzRect{ obsPos.x - 1.5f,obsPos.z - 1.5f,obsPos.x + 1.5f,obsPos.z + 1.5f };
		float height = 1.0f;

		glm::vec3 robotPos = m_Robot->GetPosition();

		if (robotPos.y < height) {
			if (xzRect.x < robotPos.x && robotPos.x < xzRect.z && xzRect.y < robotPos.z && robotPos.z < xzRect.w) {
				return true;
			}
		}
		else {
			if (xzRect.x < robotPos.x && robotPos.x < xzRect.z && xzRect.y < robotPos.z && robotPos.z < xzRect.w) {
				m_fFloor = height;
			}
		}
	}
	return false;
}
