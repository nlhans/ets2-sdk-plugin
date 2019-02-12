
#if LOGGING
#include <windows.h>
#include <string> 
#include <ctime>
#include "log.hpp" 
// Function: Logger
// used to log to file, usage: add LOGGING to preamble to activate it globally in cpp, but use it with care not full tested yet and not many features 
// also it rise dll size and may have impacted on the system (loading time, etc.)
namespace logger
{
	void flush() { out.flush(); }

	namespace // detail
	{
		std::wstring s2ws(const std::string& s)
		{
			int len;
			int slength = (int)s.length() + 1;
			len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0);
			wchar_t* buf = new wchar_t[len];
			MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
			std::wstring r(buf);
			delete[] buf;
			return r;
		}
		std::string time_stamp()
		{
			const auto now = std::time(nullptr);
			char cstr[256]{};
			return std::strftime(cstr, sizeof(cstr), "%Y%m%d_%H%M%S", std::localtime(&now)) ? cstr : "";
		}
		std::string ExePath() {
			char buffer[248];
			GetModuleFileNameA(NULL, buffer, 248);
			std::string::size_type pos = std::string(buffer).find_last_of("\\/");
			return std::string(buffer).substr(0, pos);
		}
		std::string path_to_session_log_file() {
            if(CreateDirectoryA((ExePath()+"/tmp/").c_str(),NULL)||ERROR_ALREADY_EXISTS==GetLastError()) {
				if (CreateDirectoryA((ExePath() + "/tmp/log/").c_str(), NULL) || ERROR_ALREADY_EXISTS == GetLastError()) {
					static const std::string log_dir = ExePath() + "/tmp/log/";
					static const std::string log_file_name = "log.txt";
					return log_dir + time_stamp() + '_' + log_file_name;
				}else {
					return "";
				}
            }else {
				return "";
            }
			
		}
		BOOL DirectoryExists(LPCTSTR szPath)
		{
			DWORD dwAttrib = GetFileAttributes(szPath);

			return (dwAttrib != INVALID_FILE_ATTRIBUTES &&
				(dwAttrib & FILE_ATTRIBUTE_DIRECTORY));
		}

       
	}

	const std::string path = path_to_session_log_file();    
	std::ofstream out = std::ofstream(path);
}

#endif 