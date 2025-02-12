#include <StormByte/system/system.hxx>
#ifdef WINDOWS
#include <windows.h> // For MAX_PATH
#include <tchar.h>
#include <direct.h> // For _getcwd
#else
#include <cstdlib> // For mkstemp
#include <unistd.h> // For close
#endif

using namespace StormByte;

#ifdef WINDOWS
std::string System::UTF8Encode(const std::wstring& wstr) {
	if (wstr.empty()) return std::string();
	int size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
	std::string strTo(size_needed, 0);
	WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
	return strTo;
}

std::wstring System::UTF8Decode(const std::string& str) {
	if (str.empty()) return std::wstring();
	int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
	std::wstring wstrTo(size_needed, 0);
	MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
	return wstrTo;
}
#endif

std::filesystem::path System::TempFileName() {
#ifdef WINDOWS
    wchar_t tempPath[MAX_PATH];
    wchar_t tempFile[MAX_PATH];

    // Get the path to the temporary file directory
    if (GetTempPathW(MAX_PATH, tempPath) == 0) {
        throw std::runtime_error("Error getting temp path");
    }

    // Create a unique temporary filename
    if (GetTempFileNameW(tempPath, L"TMP", 0, tempFile) == 0) {
        throw std::runtime_error("Error getting temp file name");
    }

    return StormByte::System::System::UTF8Encode(std::wstring(tempFile));
#else
    char temp_filename[] = "/tmp/config_testXXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        throw std::runtime_error("Failed to create temporary file");
    }
    close(fd);
    return std::string(temp_filename);
#endif
}
