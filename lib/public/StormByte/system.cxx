#include <StormByte/system.hxx>
#include <StormByte/string.hxx>

#ifdef WINDOWS
#include <windows.h> // For MAX_PATH
#else
#include <cstdlib> // For mkstemp
#include <cstring> // For strncpy
#include <unistd.h> // For close
#define MAX_PATH 256
#endif

#include <thread>

namespace StormByte::System {
	std::filesystem::path TempFileName(const std::string& prefix) {
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

	std::filesystem::path CurrentPath() {
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

	template <typename Rep, typename Period>
    void Sleep(const std::chrono::duration<Rep, Period>& duration) {
        std::this_thread::sleep_for(duration);
    }

	// Explicit template instantiations
	//template STORMBYTE_PUBLIC void Sleep(const std::chrono::microseconds& duration); // Microsecond sleep is not working properly
	template STORMBYTE_PUBLIC void Sleep(const std::chrono::milliseconds& duration);
	template STORMBYTE_PUBLIC void Sleep(const std::chrono::seconds& duration);
	template STORMBYTE_PUBLIC void Sleep(const std::chrono::minutes& duration);
	template STORMBYTE_PUBLIC void Sleep(const std::chrono::hours& duration);
}