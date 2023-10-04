#include "ExamScene.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Timer.h"
#include <cmath>

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

	m_sptrMainCamera = std::make_unique<CCamera>();

	m_sptrMainCamera->RegenarationViewMatrix();
	m_sptrMainCamera->GenerateProjectionMatrix(glm::radians(90.0f), (float)g_WindowSizeX / (float)g_WindowSizeY, 0.1f, 50.0f);
	m_pMainCamera = m_sptrMainCamera.get();

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


	m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->Render();
	}

	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program);

	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->Render();
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
		m_AimbientMoveDir = glm::vec3(0.0f, 0.0f, -1.0f);
		break;
	case 'a':
		m_AimbientMoveDir = glm::vec3(-1.0f, 0.0f, 0.0f);
		break;
	case 's':
		m_AimbientMoveDir = glm::vec3(0.0f, 0.0f, 1.0f);
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


	m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->Render();
	}


	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program);

	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->Render();
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


	m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program);



	for (std::shared_ptr<CObject>& obj : m_pObjects) {
		obj->BindShaderVariables(s_Program);

		obj->Render();
	}


	s_Program = g_Renderer->SkyBoxShader;
	glUseProgram(s_Program);

	m_pSunLight->BindShaderVariables(s_Program);
	m_pMainCamera->BindShaderVariables(s_Program);

	glDepthFunc(GL_LEQUAL);

	m_pSkyBoxObject->BindShaderVariables(s_Program);
	m_pSkyBoxObject->Render();
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
	static glm::vec4 rectColide{- 0.5, -1.0f, 0.5f, 1.0f  };
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
