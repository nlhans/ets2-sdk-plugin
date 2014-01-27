#ifndef SHAREDMEMORY_HPP
#define SHAREDMEMORY_HPP
#include <windows.h>

class SharedMemory
{
protected:

        char* name;
        int mapsize;

        HANDLE hMapFile;
        void* pBuffer;
        bool isSharedMemoryHooked;

        void LogError(const char* logPtr);

public:
        bool Hooked() { return isSharedMemoryHooked; }
        void* GetBuffer() { return pBuffer; }

        SharedMemory(char *mapPtr, unsigned int size);
        ~SharedMemory();


};

#endif