#pragma once

namespace bey {
	class IGameTimer {
	public :		

		// return elapsed time from the last tick
		virtual float DeltaTime() const = 0;

		virtual void Reset() = 0; // reset and start timer
		
		virtual void Start() = 0; // start measuring time
		
		virtual void Stop() = 0; // pause the timer
		
		virtual void Tick() = 0; // call every frame to update timer
	};
}