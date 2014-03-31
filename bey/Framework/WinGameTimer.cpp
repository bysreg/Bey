#include "WinGameTimer.h"
#include <windows.h>

using namespace bey;

WinGameTimer::WinGameTimer() :
m_IsStop(false),
m_SecondsPerCount(0),
m_DeltaTime(0), 
m_BaseTime(0),
m_PrevTickTime(0), 
m_CurrTickTime(0)
{
	__int64 countsPerSec;
	QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
	m_SecondsPerCount = 1.0 / (double) countsPerSec;
}

float WinGameTimer::DeltaTime() const
{
	return (float) m_DeltaTime;
}

void WinGameTimer::Reset()
{
	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_BaseTime = currTime;
	m_PrevTickTime = currTime;

	m_IsStop = false;
}

void WinGameTimer::Start()
{
	__int64 startTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&startTime);

	if (m_IsStop) 
	{
		m_PrevTickTime = startTime;
		m_IsStop = false;
	}
}

void WinGameTimer::Stop()
{
	if (!m_IsStop)
	{
		m_IsStop = true;
	}
}

void WinGameTimer::Tick()
{
	if (m_IsStop) 
	{
		m_DeltaTime = 0;
		return;
	}

	__int64 currTime;
	QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

	m_CurrTickTime = currTime;
	m_DeltaTime = (m_CurrTickTime - m_PrevTickTime) * m_SecondsPerCount;
	m_PrevTickTime = m_CurrTickTime;	
}