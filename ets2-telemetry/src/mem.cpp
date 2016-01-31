#include "mem.h"

void SharedMemory::LogError(const char *logPtr)
{
#ifdef SHAREDMEM_LOGGING
	if (this->logFilePtr == NULL)
	{
		this->logFilePtr = fopen(SHAREDMEM_FILENAME, "a");
	}
	if (this->logFilePtr != NULL)
	{
		fprintf(this->logFilePtr, "%s\r\n", logPtr);
		fprintf(this->logFilePtr, "Windows Error code: %d\r\n\r\n", GetLastError());
	}
#endif
}
