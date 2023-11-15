#pragma once
#include "Utils/OS.h"

#ifdef __linux__
#include <dlfcn.h>
#endif

namespace GALAXY 
{

	template<typename T>
	inline T Utils::OS::GetDLLMethod(void* dll, const char* name)
	{
#ifdef _WIN32
		return (T)(GetProcAddress((HMODULE)dll, name));
#elif defined(__linux__)
		return (T)dlsym(dll, name);
#endif
		return T();
	}


}
