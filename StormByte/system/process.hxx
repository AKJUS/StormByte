#pragma once

#include <StormByte/system/pipe.hxx>

#include <filesystem>
#include <iostream>
#include <thread>
#ifdef LINUX
#include <unistd.h>
#else
#include <windows.h>
#endif
#include <vector>

namespace StormByte::System {
	struct {} typedef _EoF;
	static constexpr _EoF EoF = {};
	class STORMBYTE_PUBLIC Process {
		public:
			Process(const std::filesystem::path& prog, const std::vector<std::string>& args = std::vector<std::string>());
			Process(std::filesystem::path&&, std::vector<std::string>&&);
			Process(const Process&)				= delete;
			Process(Process&&)					= default;
			Process& operator=(const Process&)	= delete;
			Process& operator=(Process&&)		= default;
			virtual ~Process() noexcept;
			#ifdef LINUX
			int wait() noexcept;
			pid_t get_pid() noexcept;
			#else
			DWORD wait() noexcept;
			PROCESS_INFORMATION get_pid();
			#endif
			void suspend();
			void resume();

			Process& operator>>(Process&);
			std::string& operator>>(std::string&);
			friend std::ostream& operator<<(std::ostream&, const Process&);
			Process& operator<<(const std::string&);
			void operator<<(const System::_EoF&);

			enum class Status { RUNNING, SUSPENDED, TERMINATED };

		protected:
			Status m_status;
			#ifdef LINUX
			pid_t m_pid;
			#else
			STARTUPINFO m_siStartInfo;
			PROCESS_INFORMATION m_piProcInfo;
			#endif
			System::Pipe m_pstdout, m_pstdin, m_pstderr;
			
		private:
			void send(const std::string&);
			void run();
			void consume_and_forward(Process&);
			#ifdef WINDOWS
			std::wstring full_command() const;
			#endif

			std::filesystem::path m_program;
			std::vector<std::string> m_arguments;
			std::unique_ptr<std::thread> m_forwarder;
	};
	std::ostream& operator<<(std::ostream&, const Process&);
}