#pragma once
#include "stdafx.h"

const ULONG MAX_SAMPLE_COUNT = 50;

class CGameTimer
{
public:
	CGameTimer();
	virtual ~CGameTimer();

	void Tick(float fLockFPS = 0.0f);
	void Start();
	void Stop();
	void Reset();

	unsigned long GetFrameRate(LPTSTR lpszString = NULL, int nCharacters = 0);
	float GetFrameTimeElapsed();
	float GetTotalTime();
	long long GetNowTimeAfter(float second);
	long long GetNowTime();
	__int64 GetCurrentCounter() {
		return m_nCurrentPerformanceCounter;
	};
private:
	double m_fTimeScale;
	double m_fTotalTimeElasped;

	float m_fDynamicTimeScale;
	float m_fTimeElapsed;

	__int64							m_nBasePerformanceCounter;
	__int64							m_nPausedPerformanceCounter;
	__int64							m_nStopPerformanceCounter;
	__int64							m_nCurrentPerformanceCounter;
	__int64							m_nLastPerformanceCounter;

	__int64							m_nPerformanceFrequencyPerSec;

	float							m_fFrameTime[MAX_SAMPLE_COUNT];
	ULONG							m_nSampleCount;

	unsigned long					m_nCurrentFrameRate;
	unsigned long					m_nFramesPerSecond;
	float							m_fFPSTimeElapsed;

	bool							m_bStopped;
};

extern CGameTimer g_Timer;