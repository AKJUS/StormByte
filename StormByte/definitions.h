#pragma once

// Since only Windows and Linux OS is supported...
#if defined _WIN32 || defined __CYGWIN__
	#define WINDOWS
#else
	#define LINUX
#endif

#if defined(__clang__)
#define CLANG
#elif defined(__GNUC__) || defined(__GNUG__)
#define GCC
#elif defined(_MSC_VER)
#define MSVC
#endif

// Aliases which would make the code very large
#define expected_value(value)					value
#define expected_value_ref(value)				std::ref(value)
#define expected_value_cref(value)				std::cref(value)
#define expected_type(type, exception) 			std::expected<type, exception>
#define expected_type_ref(type, exception) 		std::expected<std::reference_wrapper<type>, exception>
#define expected_type_cref(type, exception) 	std::expected<std::reference_wrapper<const type>, exception>
#define unexpected_value(exception) 			std::unexpected(exception)