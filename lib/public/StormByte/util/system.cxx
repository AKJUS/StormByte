#include <StormByte/util/system.hxx>
#include <StormByte/util/string.hxx>

#ifdef WINDOWS
#include <windows.h> // For MAX_PATH
#else
#include <cstdlib> // For mkstemp
#include <cstring> // For strncpy
#include <unistd.h> // For close
#define MAX_PATH 256
#endif

using namespace StormByte::Util;

std::filesystem::path System::TempFileName(const std::string& prefix) {
#ifdef WINDOWS
    wchar_t tempPath[MAX_PATH];
    wchar_t tempFile[MAX_PATH];

    // Get the path to the temporary file directory
    if (GetTempPathW(MAX_PATH, tempPath) == 0) {
        throw std::runtime_error("Error getting temp path");
    }

    // Create a unique temporary filename
    if (GetTempFileNameW(tempPath, String::UTF8Decode(prefix).c_str(), 0, tempFile) == 0) {
        throw std::runtime_error("Error getting temp file name");
    }

    return String::UTF8Encode(std::wstring(tempFile));
#else
	const std::string temp_filename_with_prefix = "/tmp/" + prefix + "XXXXXX";
	char temp_filename[MAX_PATH];
	strncpy(temp_filename, temp_filename_with_prefix.c_str(), MAX_PATH);
	temp_filename[MAX_PATH - 1] = '\0';
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        throw std::runtime_error("Failed to create temporary file");
    }
    close(fd);
    return std::string(temp_filename);
#endif
}

std::filesystem::path System::CurrentPath() {
	char path[MAX_PATH];
#ifdef WINDOWS
	if (GetModuleFileNameA(nullptr, path, MAX_PATH)) {
		return std::filesystem::path(path).remove_filename();
	}
#else
	ssize_t count = readlink("/proc/self/exe", path, sizeof(path) - 1);
	if (count != -1) {
		path[count] = '\0';
		return std::filesystem::path(path);
	}
#endif
	return "NOPATH";
}