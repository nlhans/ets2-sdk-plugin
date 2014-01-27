#include "SharedMemory.hpp"

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

SharedMemory::SharedMemory(char *namePtr, unsigned int size)
{
    this->mapsize = size;
    this->namePtr = namePtr;

    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE, // use paging file
            NULL, // default security
            PAGE_READWRITE, // read/write access
            0, // maximum object size (high-order DWORD)
            mapsize, // maximum object size (low-order DWORD)
            (LPCWSTR)namePtr); // name of mapping object

    if (hMapFile == NULL)
    {
            if(GetLastError() == (DWORD)183) // already exists
            {
                    hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, (LPCWSTR)namePtr);
                    if (hMapFile == NULL)
                    {
                            LogError("Could not open file mapping object although it already exists.");
                            return;
                    }
            }
            else
            {
                    LogError("Could not create file mapping object");
                    LogError(namePtr);
                    return;
            }
    }

    this->pBufferPtr = (void*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, mapsize);

    if (this->pBufferPtr == NULL)
    {
        LogError("Could not reserve buffer for shared memory");
        CloseHandle(hMapFile);
    }
	else
	{
		isSharedMemoryHooked = true;
		LogError("Opened MMF");
	}
		
}


SharedMemory::~SharedMemory(void)
{
#ifdef SHAREDMEM_LOGGING
		if (logFilePtr != NULL)
		{
			fclose(logFilePtr);
			// TODO: Is this closed properly?
		}
#endif
        if (isSharedMemoryHooked)
        {
                UnmapViewOfFile(pBufferPtr);
                CloseHandle(hMapFile);
                
        }

        isSharedMemoryHooked = false;

}