#pragma once

namespace bey {
	class App {
	public:			
		virtual void Init() = 0;
		virtual void Clean() = 0;
		virtual void Update(float dt) = 0;
		virtual void Render() = 0;
	};
}