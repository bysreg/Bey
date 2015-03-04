#pragma once



namespace bey {

	//TODO : check the alignment
	struct AllocBuffers
	{
		void* pAddress;
		void* oriAddress;
		unsigned int length;
		char* fileName;
		unsigned long line;
	};

	// TODO : prevent from copyable
	class MemoryManager
	{
	private:
		MemoryManager();
		~MemoryManager();

		static const int MAX_ALLOC = 1000;
		
		AllocBuffers m_dataBuffers[MAX_ALLOC];
		unsigned int m_noBuffers;

	public:
		static MemoryManager * GetInstance();

		// allocate memory with size noBytes with information filename and line number of where it was allocated
		// will allocate more memory than necessary to store debugging information, and sentinel value in the end of array to detect memory corruption
		void* Alloc(unsigned int noBytes, char* fileName, unsigned int line);
		void Free(void * pAddress);
		void Dump();
		void SanityCheck(bool bShowStats = false);
		void Error(char* szMessage);
	};
}