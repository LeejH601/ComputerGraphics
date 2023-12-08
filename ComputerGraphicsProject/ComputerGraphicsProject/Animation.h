#pragma once
#include "stdafx.h"
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//

#define ANIMATION_TYPE_ONCE				0
#define ANIMATION_TYPE_LOOP				1
#define ANIMATION_TYPE_PINGPONG			2
#define ANIMATION_CALLBACK_EPSILON		0.0165f

struct CALLBACKKEY
{
	float m_fTime = 0.0f;
	void* m_pCallbackData = nullptr;
};

#define _WITH_ANIMATION_INTERPOLATION

class CObject;
class CSkinnedMesh;
class CLoadedModelInfo;
class CAnimationCallbackHandler
{
public:
	CAnimationCallbackHandler() { }
	~CAnimationCallbackHandler() { }

public:
	virtual void HandleCallback(void* pCallbackData, float fTrackPosition) { }
};

class CAnimationSet
{
public:
	CAnimationSet(float fLength, int nFramesPerSecond, int nKeyFrameTransforms, int nSkinningBones, char* pstrName);
	~CAnimationSet();

public:
	char							m_pstrAnimationSetName[64];

	float							m_fLength = 0.0f;
	int								m_nFramesPerSecond = 0; //m_fTicksPerSecond

	int								m_nKeyFrames = 0;
	std::vector<float>				m_pfKeyFrameTimes;
	std::vector<std::vector<glm::mat4x4>>		m_ppxmf4x4KeyFrameTransforms;

#ifdef _WITH_ANIMATION_SRT
	int								m_nKeyFrameScales = 0;
	float* m_pfKeyFrameScaleTimes = NULL;
	XMFLOAT3** m_ppxmf3KeyFrameScales = NULL;
	int								m_nKeyFrameRotations = 0;
	float* m_pfKeyFrameRotationTimes = NULL;
	XMFLOAT4** m_ppxmf4KeyFrameRotations = NULL;
	int								m_nKeyFrameTranslations = 0;
	float* m_pfKeyFrameTranslationTimes = NULL;
	XMFLOAT3** m_ppxmf3KeyFrameTranslations = NULL;
#endif
public:
	glm::mat4x4 GetSRT(int nBone, float fPosition);
};

class CAnimationSets
{
public:
	CAnimationSets(int nAnimationSets);
	~CAnimationSets();

public:
	int								m_nAnimationSets = 0;
	std::vector<CAnimationSet*>		m_pAnimationSets;

	int								m_nAnimatedBoneFrames = 0;
	std::vector<glm::mat4x4>		m_ppAnimatedBoneFrameCaches; //[m_nAnimatedBoneFrames]
};

class CAnimationTrack
{
public:
	CAnimationTrack() { }
	~CAnimationTrack() { }

public:
	BOOL 							m_bEnable = true;
	bool							m_bUpdate = true;
	float 							m_fSpeed = 1.0f;
	float 							m_fPosition = 0.0f;
	float 							m_fWeight = 0.0f;
	float							m_fSequenceWeight = 0.0f;

	int 							m_nAnimationSet = 0;

	int 							m_nType = ANIMATION_TYPE_LOOP; //Once, Loop, PingPong

	int 							m_nCallbackKeys = 0;
	std::vector<CALLBACKKEY>		m_pCallbackKeys;

	CAnimationCallbackHandler*		m_pAnimationCallbackHandler = NULL;
public:
	void SetAnimationSet(int nAnimationSet) { m_nAnimationSet = nAnimationSet; }

	void SetEnable(bool bEnable) { m_bEnable = bEnable; }
	void SetSpeed(float fSpeed) { m_fSpeed = fSpeed; }
	void SetWeight(float fWeight) { m_fWeight = fWeight; }
	void SetPosition(float fPosition) { m_fPosition = fPosition; }
	void SetUpdateEnable(bool bEnable) { m_bUpdate = bEnable; };
	float UpdatePosition(float fTrackPosition, float fElapsedTime, float fAnimationLength);
	float UpdateSequence(float fSequence, float fElapsedTime, float fAnimationLength);

	void SetCallbackKeys(int nCallbackKeys);
	void SetCallbackKey(int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(CAnimationCallbackHandler* pCallbackHandler);
	float ConvertSequenceToPosition(float fAnimationLength) { return m_fSequenceWeight * fAnimationLength; };
	float ConvertPositionToSequence(float fAnimationLength) { return m_fPosition / fAnimationLength; };

	void HandleCallback();
};

typedef std::pair<std::pair<int,int>, CObject*> Obj_Socket; // <int, int>, CGameObject* : <SkinMeshIndex, BoneTransformIndex>, obj

class CAnimationController
{
public:
	CAnimationController();
	CAnimationController(int nAnimationTracks, CLoadedModelInfo* pModel);
	virtual ~CAnimationController();

public:
	float 							m_fTime = 0.0f;

	int m_nAnimationTracks = 0;
	std::vector<CAnimationTrack> m_pAnimationTracks;

	int m_nSubAnimationTracks = 0;
	std::vector<CAnimationTrack> m_pSubAnimationTracks;

	std::vector<float> m_fLayerBlendWeights;
	std::vector<int> m_nLayerBlendBaseBoneIndex;
	std::vector<int> m_nLayerBlendRange;

	CAnimationSets* m_pAnimationSets = NULL;

	int m_nSkinnedMeshes = 0;
	std::vector<CSkinnedMesh*> m_ppSkinnedMeshes ; //[SkinnedMeshes], Skinned Mesh Cache

	std::vector<std::vector<glm::mat4>> m_ppMat4x4SkinningBoneTransforms; //[SkinnedMeshes]

	std::vector<Obj_Socket> m_pSockets;

	static GLuint m_UBOBoneTransforms;

public:
	void UpdateShaderVariables();

	void SetTrackAnimationSet(int nAnimationTrack, int nAnimationSet);

	void SetTrackEnable(int nAnimationTrack, bool bEnable);
	void SetTrackPosition(int nAnimationTrack, float fPosition);
	void SetTrackSpeed(int nAnimationTrack, float fSpeed);
	void SetTrackWeight(int nAnimationTrack, float fWeight);
	void SetTrackUpdateEnable(int nAnimationTrack, bool bEnable);

	float GetTrackPosition(int nAnimationTrack) { return m_pAnimationTracks[nAnimationTrack].m_fPosition; };
	float GetTrackWeight(int nAnimationTrack) { return m_pAnimationTracks[nAnimationTrack].m_fWeight; };

	void SetCallbackKeys(int nAnimationTrack, int nCallbackKeys);
	void SetCallbackKey(int nAnimationTrack, int nKeyIndex, float fTime, void* pData);
	void SetAnimationCallbackHandler(int nAnimationTrack, CAnimationCallbackHandler* pCallbackHandler);

	void SetSocket(int SkinMeshIndex, std::string& FrameName, CObject* DestObject);

	void UpdateSocketsTransform();
	void UpdateBoneTransform();

	virtual void BindShaderVariables(GLuint s_Program, int skinmeshIndex = 0);

	void AdvanceTime(float fElapsedTime, CObject* pRootGameObject, int skinmeshIndex = 0);

	


public:
	bool							m_bRootMotion = false;
	CObject* m_pModelRootObject = NULL;

	CObject* m_pRootMotionObject = NULL;
	glm::vec3						m_xmf3FirstRootMotionPosition = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3						m_xmf3RootObjectScale = glm::vec3(1.0f, 1.0f, 1.0f);


	void SetRootMotion(bool bRootMotion) { m_bRootMotion = bRootMotion; }

	virtual void OnRootMotion(CObject* pRootGameObject, float fTimeElapsed) { }
	virtual void OnAnimationIK(CObject* pRootGameObject) { }
};