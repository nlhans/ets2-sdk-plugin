#include "ets2-telemetry-common.h"

#ifdef WINVER
// Telemetry api
BOOL APIENTRY DllMain(
	HMODULE module,
	DWORD  reason_for_call,
	LPVOID reseved
)
{
	if (reason_for_call == DLL_PROCESS_DETACH) {

		// Close MemoryMap
		if (telemMem != NULL)
		{
			telemMem->Close();
		}

	}
	return TRUE;
}
#endif
