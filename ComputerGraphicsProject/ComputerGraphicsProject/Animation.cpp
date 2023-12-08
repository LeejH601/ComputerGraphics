#include "Animation.h"
#include "Object.h"
#include "Mesh.h"

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationSet::CAnimationSet(float fLength, int nFramesPerSecond, int nKeyFrames, int nAnimatedBones, char* pstrName)
{
	m_fLength = fLength;
	m_nFramesPerSecond = nFramesPerSecond;
	m_nKeyFrames = nKeyFrames;

	strcpy_s(m_pstrAnimationSetName, 64, pstrName);

	m_pfKeyFrameTimes.resize(nKeyFrames);
	m_ppxmf4x4KeyFrameTransforms.resize(nKeyFrames);
	for (int i = 0; i < nKeyFrames; i++)
		m_ppxmf4x4KeyFrameTransforms[i].resize(nAnimatedBones);
}
CAnimationSet::~CAnimationSet()
{
}
glm::mat4x4 CAnimationSet::GetSRT(int nBone, float fPosition)
{
	glm::mat4x4 xmf4x4Transform = glm::identity< glm::mat4x4>();
	for (int i = 0; i < (m_nKeyFrames - 1); i++)
	{
		if ((m_pfKeyFrameTimes[i] <= fPosition) && (fPosition < m_pfKeyFrameTimes[i + 1]))
		{
			float t = (fPosition - m_pfKeyFrameTimes[i]) / (m_pfKeyFrameTimes[i + 1] - m_pfKeyFrameTimes[i]);
			xmf4x4Transform = m_ppxmf4x4KeyFrameTransforms[i][nBone] * (1.0f - t) + m_ppxmf4x4KeyFrameTransforms[i + 1][nBone] * t;
			//xmf4x4Transform = glm::mix<glm::mat4x4, float>(m_ppxmf4x4KeyFrameTransforms[i][nBone], m_ppxmf4x4KeyFrameTransforms[i + 1][nBone], t);
			break;
		}
	}
	if (fPosition >= m_pfKeyFrameTimes[m_nKeyFrames - 1]) xmf4x4Transform = m_ppxmf4x4KeyFrameTransforms[m_nKeyFrames - 1][nBone];
	return(xmf4x4Transform);
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationSets::CAnimationSets(int nAnimationSets)
{
	m_nAnimationSets = nAnimationSets;
	m_pAnimationSets.resize(nAnimationSets);
}
CAnimationSets::~CAnimationSets()
{
}

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
void CAnimationTrack::SetCallbackKeys(int nCallbackKeys)
{
	m_nCallbackKeys = nCallbackKeys;
	m_pCallbackKeys.resize(nCallbackKeys);
}
void CAnimationTrack::SetCallbackKey(int nKeyIndex, float fKeyTime, void* pData)
{
	m_pCallbackKeys[nKeyIndex].m_fTime = fKeyTime;
	m_pCallbackKeys[nKeyIndex].m_pCallbackData = pData;
}
void CAnimationTrack::SetAnimationCallbackHandler(CAnimationCallbackHandler* pCallbackHandler)
{
	m_pAnimationCallbackHandler = pCallbackHandler;
}
void CAnimationTrack::HandleCallback()
{
	if (m_pAnimationCallbackHandler)
	{
		for (int i = 0; i < m_nCallbackKeys; i++)
		{
			if (abs(m_pCallbackKeys[i].m_fTime - m_fPosition) < ANIMATION_CALLBACK_EPSILON)
			{
				if (m_pCallbackKeys[i].m_pCallbackData) m_pAnimationCallbackHandler->HandleCallback(m_pCallbackKeys[i].m_pCallbackData, m_fPosition);
				break;
			}
		}
	}
}
float CAnimationTrack::UpdatePosition(float fTrackPosition, float fElapsedTime, float fAnimationLength)
{
	float fTrackElapsedTime = fElapsedTime * m_fSpeed;
	switch (m_nType)
	{
	case ANIMATION_TYPE_LOOP:
	{
		if (m_fPosition < 0.0f) m_fPosition = 0.0f;
		else
		{
			m_fPosition = fTrackPosition + fTrackElapsedTime;
			if (m_fPosition > fAnimationLength)
			{
				m_fPosition = -ANIMATION_CALLBACK_EPSILON;
				return(fAnimationLength);
			}
		}
		//			m_fPosition = fmod(fTrackPosition, m_pfKeyFrameTimes[m_nKeyFrames-1]); // m_fPosition = fTrackPosition - int(fTrackPosition / m_pfKeyFrameTimes[m_nKeyFrames-1]) * m_pfKeyFrameTimes[m_nKeyFrames-1];
		//			m_fPosition = fmod(fTrackPosition, m_fLength); //if (m_fPosition < 0) m_fPosition += m_fLength;
		//			m_fPosition = fTrackPosition - int(fTrackPosition / m_fLength) * m_fLength;
		break;
	}
	case ANIMATION_TYPE_ONCE:
		m_fPosition = fTrackPosition + fTrackElapsedTime;
		if (m_fPosition > fAnimationLength) m_fPosition = fAnimationLength;
		break;
	case ANIMATION_TYPE_PINGPONG:
		break;
	}

	m_fSequenceWeight = m_fPosition / fAnimationLength;

	return(m_fPosition);
}
float CAnimationTrack::UpdateSequence(float fSequence, float fElapsedTime, float fAnimationLength)
{
	float fTrackElapsedTime = fElapsedTime * m_fSpeed;
	float SequenceElapsedTime = (1.0f * fTrackElapsedTime) / fAnimationLength;

	switch (m_nType)
	{
	case ANIMATION_TYPE_LOOP:
	{
		if (m_fSequenceWeight < 0.0f) m_fSequenceWeight = 0.0f;
		else
		{
			m_fSequenceWeight = m_fSequenceWeight + SequenceElapsedTime;
			if (m_fSequenceWeight > 1.0f)
			{
				m_fSequenceWeight = -ANIMATION_CALLBACK_EPSILON;
				return(1.0f);
			}
		}
		//			m_fPosition = fmod(fTrackPosition, m_pfKeyFrameTimes[m_nKeyFrames-1]); // m_fPosition = fTrackPosition - int(fTrackPosition / m_pfKeyFrameTimes[m_nKeyFrames-1]) * m_pfKeyFrameTimes[m_nKeyFrames-1];
		//			m_fPosition = fmod(fTrackPosition, m_fLength); //if (m_fPosition < 0) m_fPosition += m_fLength;
		//			m_fPosition = fTrackPosition - int(fTrackPosition / m_fLength) * m_fLength;
		break;
	}
	case ANIMATION_TYPE_ONCE:
		m_fSequenceWeight = m_fSequenceWeight + fTrackElapsedTime;
		if (m_fSequenceWeight > 1.0f) m_fSequenceWeight = 1.0f;
		break;
	case ANIMATION_TYPE_PINGPONG:
		break;
	}

	return(m_fSequenceWeight);
}
GLuint CAnimationController::m_UBOBoneTransforms = -1;
CAnimationController::CAnimationController()
{
	m_nAnimationTracks = 0;

	m_nSkinnedMeshes = 1;
	m_ppSkinnedMeshes.resize(m_nSkinnedMeshes);
	for (int i = 0; i < m_nSkinnedMeshes; i++) m_ppSkinnedMeshes[i] = nullptr;

	m_ppMat4x4SkinningBoneTransforms.resize(m_nSkinnedMeshes);

	// 데이터 세팅
	for (int i = 0; i < m_nSkinnedMeshes; i++)
	{
		m_ppMat4x4SkinningBoneTransforms[i].resize(SKINNED_ANIMATION_BONES);
	}

	if (m_UBOBoneTransforms == -1) {
		glGetError();
		int size = SKINNED_ANIMATION_BONES * sizeof(glm::mat4x4);
		glGenBuffers(1, &m_UBOBoneTransforms);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOBoneTransforms);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_UBOBoneTransforms);
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOBoneTransforms);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4x4) * m_ppMat4x4SkinningBoneTransforms.size(), m_ppMat4x4SkinningBoneTransforms.data());
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
CAnimationController::CAnimationController(int nAnimationTracks, CLoadedModelInfo* pModel)
{
	m_nAnimationTracks = nAnimationTracks;
	m_pAnimationTracks.resize(nAnimationTracks);
	m_pAnimationTracks[0].m_fWeight = 1.0f;
	for (int i = 1; i < m_pAnimationTracks.size(); ++i) {
		m_pAnimationTracks[i].m_fWeight = 0.0f;
	}

	m_pAnimationSets = pModel->m_pAnimationSets.get();

	m_nSkinnedMeshes = pModel->m_nSkinnedMeshes;
	m_ppSkinnedMeshes.resize(m_nSkinnedMeshes);
	for (int i = 0; i < m_nSkinnedMeshes; i++) m_ppSkinnedMeshes[i] = pModel->m_ppSkinnedMeshes[i];

	m_ppMat4x4SkinningBoneTransforms.resize(m_nSkinnedMeshes);

	// 데이터 세팅
	for (int i = 0; i < m_nSkinnedMeshes; i++)
	{
		m_ppMat4x4SkinningBoneTransforms[i].resize(SKINNED_ANIMATION_BONES);
	}

	if (m_UBOBoneTransforms == -1) {
		glGetError();
		int size = SKINNED_ANIMATION_BONES * sizeof(glm::mat4x4);
		glGenBuffers(1, &m_UBOBoneTransforms);
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOBoneTransforms);
		glBufferData(GL_UNIFORM_BUFFER, size, NULL, GL_STATIC_DRAW);
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_UBOBoneTransforms);
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBuffer(GL_UNIFORM_BUFFER, m_UBOBoneTransforms);
		glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4x4) * m_ppMat4x4SkinningBoneTransforms.size(), m_ppMat4x4SkinningBoneTransforms.data());
		if (glGetError())
			std::cout << "error BufferData" << std::endl;

		glBindBuffer(GL_UNIFORM_BUFFER, 0);
	}
}
CAnimationController::~CAnimationController()
{
}
void CAnimationController::SetCallbackKeys(int nAnimationTrack, int nCallbackKeys)
{
	if (m_pAnimationTracks.data()) m_pAnimationTracks[nAnimationTrack].SetCallbackKeys(nCallbackKeys);
}
void CAnimationController::SetCallbackKey(int nAnimationTrack, int nKeyIndex, float fKeyTime, void* pData)
{
	if (m_pAnimationTracks.data()) m_pAnimationTracks[nAnimationTrack].SetCallbackKey(nKeyIndex, fKeyTime, pData);
}
void CAnimationController::SetAnimationCallbackHandler(int nAnimationTrack, CAnimationCallbackHandler* pCallbackHandler)
{
	if (m_pAnimationTracks.data()) m_pAnimationTracks[nAnimationTrack].SetAnimationCallbackHandler(pCallbackHandler);
}
void CAnimationController::SetSocket(int SkinMeshIndex, std::string& FrameName, CObject* DestObject)
{
	if (SkinMeshIndex >= m_nSkinnedMeshes)
		return;

	CSkinnedMesh* mesh = m_ppSkinnedMeshes[SkinMeshIndex];
	for (int i = 0; i < mesh->m_nSkinningBones; ++i) {
		if (strcmp(mesh->m_ppstrSkinningBoneNames[i].data(), FrameName.data()) == 0)
			m_pSockets.push_back(Obj_Socket(std::pair<int, int>(SkinMeshIndex, i), DestObject));
	}


}
void CAnimationController::UpdateSocketsTransform()
{
	/*for (int i = 0; i < m_pSockets.size(); ++i) {
		Obj_Socket& Socket = m_pSockets[i];
		int skinMeshIndex = Socket.first.first;
		int BondIndex = Socket.first.second;
		CGameObject* obj = Socket.second;

		glm::mat4x4 xmf4x4World; XMStoreFloat4x4(&xmf4x4World, XMMatrixTranspose(XMLoadFloat4x4(&m_ppcbxmf4x4MappedSkinningBoneTransforms[skinMeshIndex][BondIndex])));
		obj->m_pChild->UpdateTransform(&xmf4x4World);
	}*/
}
void CAnimationController::UpdateBoneTransform()
{
	/*for (int i = 0; i < m_nSkinnedMeshes; ++i) {
		glm::mat4x4 p = m_ppcbxmf4x4MappedSkinningBoneTransforms[i];

		for (int j = 0; j < m_ppSkinnedMeshes[i]->m_nSkinningBones; j++) {
			p[j] = m_ppSkinnedMeshes[i]->m_ppSkinningBoneFrameCaches[j]->m_xmf4x4World)));
		}
	}*/
}
void CAnimationController::BindShaderVariables(GLuint s_Program, int skinmeshIndex)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBOBoneTransforms);
	GLuint offset = 0u;
	glBufferSubData(GL_UNIFORM_BUFFER, offset, sizeof(glm::mat4x4) * m_ppMat4x4SkinningBoneTransforms.size(), m_ppMat4x4SkinningBoneTransforms[skinmeshIndex].data());
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}
void CAnimationController::SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet)
{
	if (m_pAnimationTracks.data())
	{
		m_pAnimationTracks[nAnimationTrack].m_nAnimationSet = nAnimationSet;
		m_pAnimationTracks[nAnimationTrack].SetPosition(0.0f);
		m_pAnimationTracks[nAnimationTrack].SetUpdateEnable(true);
	}
}
void CAnimationController::SetTrackEnable(int nAnimationTrack, bool bEnable)
{
	if (m_pAnimationTracks.data()) m_pAnimationTracks[nAnimationTrack].SetEnable(bEnable);
}
void CAnimationController::SetTrackPosition(int nAnimationTrack, float fPosition)
{
	if (m_pAnimationTracks.data()) m_pAnimationTracks[nAnimationTrack].SetPosition(fPosition);
}
void CAnimationController::SetTrackSpeed(int nAnimationTrack, float fSpeed)
{
	if (m_pAnimationTracks.data()) m_pAnimationTracks[nAnimationTrack].SetSpeed(fSpeed);
}
void CAnimationController::SetTrackWeight(int nAnimationTrack, float fWeight)
{
	if (m_pAnimationTracks.data()) m_pAnimationTracks[nAnimationTrack].SetWeight(fWeight);
}
void CAnimationController::SetTrackUpdateEnable(int nAnimationTrack, bool bEnable)
{
	if (m_pAnimationTracks.data()) m_pAnimationTracks[nAnimationTrack].SetUpdateEnable(bEnable);
}
void CAnimationController::UpdateShaderVariables()
{
	for (int i = 0; i < m_nSkinnedMeshes; i++)
	{
		/*m_ppSkinnedMeshes[i]->m_pd3dcbSkinningBoneTransforms = m_ppd3dcbSkinningBoneTransforms[i].Get();
		m_ppSkinnedMeshes[i]->m_pcbxmf4x4MappedSkinningBoneTransforms = m_ppcbxmf4x4MappedSkinningBoneTransforms[i];*/
	}
	UpdateSocketsTransform();
}
void CAnimationController::AdvanceTime(float fTimeElapsed, CObject* pRootGameObject, int skinmeshIndex)
{
	m_fTime += fTimeElapsed;

	if (m_pAnimationTracks.data())
	{
		for (int j = 0; j < m_pAnimationSets->m_nAnimatedBoneFrames; j++) {
			m_pAnimationSets->m_ppAnimatedBoneFrameCaches[j] = glm::mat4x4(0);
			m_ppMat4x4SkinningBoneTransforms[skinmeshIndex][j] = glm::mat4x4(0);
		}

		for (int k = 0; k < m_nAnimationTracks; k++)
		{
			if (m_pAnimationTracks[k].m_bEnable)
			{
				CAnimationSet* pAnimationSet = m_pAnimationSets->m_pAnimationSets[m_pAnimationTracks[k].m_nAnimationSet];
				float fPosition;

				if (m_pAnimationTracks[k].m_bUpdate)
					m_pAnimationTracks[k].UpdatePosition(m_pAnimationTracks[k].m_fPosition, fTimeElapsed, pAnimationSet->m_fLength);

				fPosition = std::max(m_pAnimationTracks[k].m_fPosition, 0.0f);

				for (int j = 0; j < m_pAnimationSets->m_nAnimatedBoneFrames; j++)
				{
					glm::mat4x4 xmf4x4Transform = m_ppMat4x4SkinningBoneTransforms[skinmeshIndex][j];
					glm::mat4x4 xmf4x4TrackTransform = pAnimationSet->GetSRT(j, fPosition);
					glm::quat q = glm::quat_cast(xmf4x4TrackTransform);


					glm::mat4x4 xmf4x4ScaledTransform = xmf4x4TrackTransform * m_pAnimationTracks[k].m_fWeight;
					xmf4x4Transform = xmf4x4Transform + xmf4x4ScaledTransform;
					m_ppMat4x4SkinningBoneTransforms[skinmeshIndex][j] = xmf4x4Transform;
				}
			}
		}

		/*if (m_pAnimationTracks.data())
		{
			if (m_nAnimationTracks == 1) {
				for (int j = 0; j < m_pAnimationSets->m_nAnimatedBoneFrames; j++) m_pAnimationSets->m_ppAnimatedBoneFrameCaches[j]->m_mat4x4Transform = glm::mat4x4(0);

				for (int k = 0; k < m_nAnimationTracks; k++)
				{
					if (m_pAnimationTracks[k].m_bEnable)
					{
						CAnimationSet* pAnimationSet = m_pAnimationSets->m_pAnimationSets[m_pAnimationTracks[k].m_nAnimationSet];
						float fPosition;
						if (m_pAnimationTracks[k].m_bUpdate)
							m_pAnimationTracks[k].UpdatePosition(m_pAnimationTracks[k].m_fPosition, fTimeElapsed, pAnimationSet->m_fLength);

						fPosition = std::max(m_pAnimationTracks[k].m_fPosition, 0.0f);

						for (int j = 0; j < m_pAnimationSets->m_nAnimatedBoneFrames; j++)
						{
							glm::mat4x4 xmf4x4Transform = m_pAnimationSets->m_ppAnimatedBoneFrameCaches[j]->m_mat4x4Transform;
							glm::mat4x4 xmf4x4TrackTransform = pAnimationSet->GetSRT(j, fPosition);
							glm::quat q = glm::quat_cast(xmf4x4TrackTransform);


							glm::mat4x4 xmf4x4ScaledTransform = xmf4x4TrackTransform * m_pAnimationTracks[k].m_fWeight;
							xmf4x4Transform = xmf4x4Transform + xmf4x4ScaledTransform;
							m_pAnimationSets->m_ppAnimatedBoneFrameCaches[j]->m_mat4x4Transform = xmf4x4Transform;
						}
						m_pAnimationTracks[k].HandleCallback();
					}
				}
			}*/


		OnRootMotion(pRootGameObject, fTimeElapsed);
		OnAnimationIK(pRootGameObject);

		//pRootGameObject->UpdateTransform(NULL);

		UpdateBoneTransform();

	}
}
