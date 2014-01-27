#ifndef SHAREDMEMORY_HPP
#define SHAREDMEMORY_HPP
#include <windows.h>
#include <stdio.h>
#include <stdlib.h>
#include "ets2-telemetry-common.hpp"

#undef SHAREDMEM_LOGGING
#if ETS2_PLUGIN_LOGGING_ON == 1
	#if ETS2_PLUGIN_LOGGING_SHAREDMEMORY == 1
		#define SHAREDMEM_FILENAME ETS2_PLUGIN_FILENAME_PREFIX "sharedmem.txt"
		#define SHAREDMEM_LOGGING 1
	#endif
#endif

class SharedMemory
{
protected:

        const char *namePtr;
        int mapsize;

		// MMF specifics
        HANDLE hMapFile;
        void* pBufferPtr;

		// Status about hook
        bool isSharedMemoryHooked;

		// Logging
#ifdef SHAREDMEM_LOGGING
		FILE *logFilePtr;
#endif

        void LogError(const char* logPtr);

public:
        bool Hooked() { return isSharedMemoryHooked; }
        void* GetBuffer() { return pBufferPtr; }

        SharedMemory(char *mapPtr, unsigned int size);
        ~SharedMemory();


};

#endif