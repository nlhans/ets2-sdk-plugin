#include "mem.h"
#if WINVER

const wchar_t* ets2MmfName = ETS2_PLUGIN_MMF_NAME;

SharedMemoryWin::SharedMemoryWin(const char* namePtr, unsigned int size)
{
	wchat_t winStr[256];
	// TODO: needs fixing:
	mbstowcs_s(winStr, namePtr, strlen(namePtr)+1);
	
	this->isSharedMemoryHooked = false;
#ifdef SHAREDMEM_LOGGING
	this->logFilePtr = NULL;
#endif

    hMapFile = CreateFileMapping(
            INVALID_HANDLE_VALUE, // use paging file
            NULL, // default security
            PAGE_READWRITE, // read/write access
            0, // maximum object size (high-order DWORD)
            size, // maximum object size (low-order DWORD)
            this->namePtr); // name of mapping object
	LogError("Created file map");
    if (hMapFile == NULL)
    {
		LogError("but it's NULL!");
        if(GetLastError() == (DWORD)183) // already exists
        {
                hMapFile = OpenFileMapping(FILE_MAP_ALL_ACCESS, false, this->namePtr);
                if (hMapFile == NULL)
                {
                        LogError("Could not open existing file mapping");
                        return;
                }
        }
        else
        {
                LogError("Could not create file mapping object");
                return;
        }
    }
	else
	{
		LogError("and it's not NULL!");
	}

    this->pBufferPtr = (void*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, size);
    if (this->pBufferPtr == NULL)
    {
        LogError("Could not reserve buffer for shared memory");
        CloseHandle(hMapFile);
    }
	else
	{
		memset(this->pBufferPtr, 0, size);
		this->hooked = true;
		LogError("Opened MMF");
	}

}

void SharedMemoryWin::Close(void)
{
#ifdef SHAREDMEM_LOGGING
    if (logFilePtr != NULL)
    {
        fclose(logFilePtr);
        // TODO: Is this closed properly?
    }
#endif
    if (this->hooked)
    {
        if (pBufferPtr != NULL) UnmapViewOfFile(pBufferPtr);
        if (hMapFile != NULL) CloseHandle((HANDLE)hMapFile);
    }
    
    this->hooked = false;
}
#endif
