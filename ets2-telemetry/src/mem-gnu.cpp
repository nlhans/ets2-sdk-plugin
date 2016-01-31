#include "mem.h"

void SharedMemoryGnu::Open(const char* namePtr, unsigned int size)
{
	this->hooked = true;
}

void SharedMemory::Close(void)
{
	this->hooked = false;
}
