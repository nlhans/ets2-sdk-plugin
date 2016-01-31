#ifndef SHAREDMEMORY_HPP
#define SHAREDMEMORY_HPP

#include <stdio.h>
#include <stdlib.h>
#include "ets2-telemetry-common.h"

class SharedMemory
{
protected:

        // MMF specifics
        void* hMapFile;
        void* pBufferPtr;

        // Status about hook
        bool hooked;

        void LogError(const char* logPtr);

public:
        bool Hooked(void) { return hooked; }
        void* GetBuffer(void) { return pBufferPtr; }

	virtual void Open(const char* name, unsigned int size);
	virtual void Close(void);

        void* getPtrAt(int offset) { return (void*) &(((unsigned char*)pBufferPtr)[offset]); }
};

class SharedMemoryWin : public SharedMemory
{
public:
        void Open(const char* newNamePtr, unsigned int size);
        void Close(void);
};
class SharedMemoryGnu : public SharedMemory
{
public:
	void Open(const char* namePtr, unsigned int size);
        void Close(void);
};


#endif
