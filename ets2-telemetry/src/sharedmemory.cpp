#include "SharedMemory.hpp"
#include <stdio.h>

void SharedMemory::LogError(const char *logPtr)
{
#ifdef LOG_ERRORS
        FILE *fo;
        fo = fopen( "sharedmemory-debug.txt", "a" );
        fprintf(fo, "%s\r\n", logPtr);
        fprintf(fo, "Error code: %d\r\n", GetLastError());
        fclose(fo);
#endif
}

SharedMemory::SharedMemory(char *namePtr, unsigned int size)
{
        this->mapsize = mapsize;
        this->name = name;

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
                        LogError(name);
                        return;
                }
        }

        pBuffer = (void*) MapViewOfFile(hMapFile, FILE_MAP_ALL_ACCESS, 0, 0, mapsize);

        if (pBuffer == NULL)
        {
                LogError("Could not reserve buffer for shared memory");
                CloseHandle(hMapFile);
                return;
        }

        isSharedMemoryHooked = true;
        LogError("Opened MMF");
        return;

}


SharedMemory::~SharedMemory(void)
{
        if (isSharedMemoryHooked)
        {
                UnmapViewOfFile(pBuffer);
                CloseHandle(hMapFile);
                
        }

        isSharedMemoryHooked = false;

}