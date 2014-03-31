#pragma once

#include "IGameTimer.h"

namespace bey {
	class WinGameTimer : public IGameTimer {
	public : 
		//override
		virtual float DeltaTime() const;
		virtual void Reset();
		virtual void Start();		
		virtual void Stop();
		virtual void Tick();						

		WinGameTimer();
	private :
		bool m_IsStop;

		double m_SecondsPerCount;
		double m_DeltaTime;

		__int64 m_BaseTime; // in count
		__int64 m_PrevTickTime; // in count
		__int64 m_CurrTickTime; // in count
	};
}