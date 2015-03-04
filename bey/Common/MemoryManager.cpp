#include "MemoryManager.h"
#include "Log.h"
#include <cstdlib>
#include <cstdio>
#include <windows.h>

using namespace bey;
using namespace std;

const unsigned long CHECK_CODE = 0X12345678;

MemoryManager* MemoryManager::GetInstance()
{
	static MemoryManager ms_Instance;
	return &ms_Instance;
}

MemoryManager::MemoryManager()
{
	m_noBuffers = 0;
}

MemoryManager::~MemoryManager()
{
}

void * MemoryManager::Alloc(unsigned int noBytes, char * fileName, unsigned int line)
{
#ifdef _DEBUG
	SanityCheck();
#endif

	//allocate memory
	size_t offset = sizeof(m_noBuffers);
	void* ori_mem = malloc(noBytes + offset + 4);
	void* ret_mem = (void*) (((size_t)ori_mem) + offset);

	//put the index of m_dataBuffers inside the memory
	memcpy(ori_mem, &m_noBuffers, sizeof(m_noBuffers));

	m_dataBuffers[m_noBuffers].pAddress = ret_mem;
	m_dataBuffers[m_noBuffers].oriAddress = ori_mem;
	m_dataBuffers[m_noBuffers].fileName = fileName;
	m_dataBuffers[m_noBuffers].line = line;
	m_dataBuffers[m_noBuffers].length = noBytes;

	char* temp = (char*)m_dataBuffers[m_noBuffers].pAddress;
	temp += noBytes;
	memcpy(temp, &CHECK_CODE, 4); // put sentinel value to detect memory overflow/corruption

	++m_noBuffers;
	return ret_mem;
}

void MemoryManager::Free(void * pAddress)
{
#ifdef _DEBUG
	//memory overrun checking
	SanityCheck();
	// deletion of Null pointer
	if (pAddress == nullptr)
	{
		Error("Null pointer deletion\n");
		return;
	}
#endif

	unsigned int bufferIdx = 0;
	void* ori_mem = (void*)(((size_t)pAddress) - sizeof(m_noBuffers));
	memcpy(&bufferIdx, ori_mem, sizeof(m_noBuffers));

	if (bufferIdx >= m_noBuffers)
	{
		// wrong pointer deletion
#ifdef _DEBUG
		Error("Bad pointer deletion\n");
		return;
#endif
	}

	free(ori_mem);
	if (bufferIdx != m_noBuffers - 1)
	{		
		memcpy(m_dataBuffers + bufferIdx, m_dataBuffers + m_noBuffers - 1, sizeof(AllocBuffers));
		memcpy(m_dataBuffers[bufferIdx].oriAddress, &bufferIdx, sizeof(m_noBuffers)); // update the buffer index
	}
	m_noBuffers--;
}

void MemoryManager::Dump()
{
	unsigned long noTotalBytes = 0;

	BEY_LOG("\nStarting memory dump\n");	

	for (unsigned int i = 0; i < m_noBuffers; i++)
	{
		BEY_LOGF("%4d. 0x%08X: %d bytes(%s: %d)\n", i + 1, (unsigned long)m_dataBuffers[i].pAddress, m_dataBuffers[i].length,
			m_dataBuffers[i].fileName, m_dataBuffers[i].line);
		noTotalBytes += m_dataBuffers[i].length;
	}
	BEY_LOG("---------------------------\n");
	BEY_LOGF("Total: %d buffers, %d bytes\n", m_noBuffers, noTotalBytes);
}

void MemoryManager::SanityCheck(bool bShowStats)
{
	if (bShowStats)
		BEY_LOG("Sanity check start...\n");

	int count = 0;
	for (unsigned int i = 0; i < m_noBuffers; i++)
	{
		char * temp = (char *)m_dataBuffers[i].pAddress;
		temp += m_dataBuffers[i].length;
		if (memcmp(temp, &CHECK_CODE, 4) != 0)
		{
			BEY_LOGF("memory corruption at 0x%08X: %d bytes(%s: %d)\n", (unsigned long)m_dataBuffers[i].pAddress, m_dataBuffers[i].length,
				m_dataBuffers[i].fileName, m_dataBuffers[i].line);
			count++;
		}
	}

	if (bShowStats || count > 0)
	{
		BEY_LOG("---------------------------\n");
		BEY_LOGF("Total: %d corrupted buffers\n", count);
	}

	if (count > 0)
	{
		Error("Memory corruption detected");
	}
}

void MemoryManager::Error(char * szMessage)
{
	BEY_LOG(szMessage);
	switch (MessageBoxA(NULL, szMessage, "Memory Error", MB_ABORTRETRYIGNORE | MB_ICONERROR))
	{
	case IDABORT:
		exit(-1);
	case IDIGNORE:
		return;
	}
	throw;
}