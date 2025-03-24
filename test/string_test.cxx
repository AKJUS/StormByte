#include <StormByte/exception.hxx>
#include <StormByte/util/string.hxx>
#include <StormByte/util/system.hxx>
#include <StormByte/test_handlers.h>

int test_simple_explode() {
	int result = 0;
	try {
		std::string str = "Hello, World!";
		std::queue<std::string> parts = StormByte::Util::String::Explode(str, ',');
		ASSERT_EQUAL("test_simple_explode", 2, parts.size());
		ASSERT_EQUAL("test_simple_explode", "Hello", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_simple_explode", " World!", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_simple_explode", true, parts.empty());
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_simple_explode", result);
}

int test_path_explode() {
	int result = 0;
	try {
		std::string str = "path/to/items";
		std::queue<std::string> parts = StormByte::Util::String::Explode(str, '/');
		ASSERT_EQUAL("test_path_explode", 3, parts.size());
		ASSERT_EQUAL("test_path_explode", "path", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_path_explode", "to", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_path_explode", "items", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_path_explode", true, parts.empty());
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_path_explode", result);
}

int test_explode_one_item() {
	int result = 0;
	try {
		std::string str = "Hello";
		std::queue<std::string> parts = StormByte::Util::String::Explode(str, '/');
		ASSERT_EQUAL("test_explode_one_item", 1, parts.size());
		ASSERT_EQUAL("test_explode_one_item", "Hello", parts.front());
		parts.pop();
		ASSERT_EQUAL("test_explode_one_item", true, parts.empty());
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_explode_one_item", result);
}

int test_temp_path() {
	int result = 0;
	try {
		const std::filesystem::path path = StormByte::Util::System::TempFileName("something");
		const bool exists = std::filesystem::exists(path);
		std::remove(path.string().c_str());
		ASSERT_TRUE("test_temp_path", exists);
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_temp_path", result);
}

int test_human_readable_byte_size() {
    int result = 0;
    constexpr const std::string locale = "en_US.UTF-8";
    try {
        // Explicitly specify the type of T
        std::string size = StormByte::Util::String::HumanReadable<uint64_t>(1024, StormByte::Util::String::Format::HumanReadableBytes, locale);
        ASSERT_EQUAL("test_human_readable_byte_size", "1 KiB", size);

        size = StormByte::Util::String::HumanReadable<uint64_t>(1024ULL * 1024, StormByte::Util::String::Format::HumanReadableBytes, locale);
        ASSERT_EQUAL("test_human_readable_byte_size", "1 MiB", size);

        size = StormByte::Util::String::HumanReadable<uint64_t>(1024ULL * 1024 * 1024, StormByte::Util::String::Format::HumanReadableBytes, locale);
        ASSERT_EQUAL("test_human_readable_byte_size", "1 GiB", size);

        size = StormByte::Util::String::HumanReadable<uint64_t>(1024ULL * 1024 * 1024 * 1024, StormByte::Util::String::Format::HumanReadableBytes, locale);
        ASSERT_EQUAL("test_human_readable_byte_size", "1 TiB", size);

        size = StormByte::Util::String::HumanReadable<uint64_t>(1024ULL * 1024 * 1024 * 1024 * 1024, StormByte::Util::String::Format::HumanReadableBytes, locale);
        ASSERT_EQUAL("test_human_readable_byte_size", "1 PiB", size);

		size = StormByte::Util::String::HumanReadable<double>(1027.65, StormByte::Util::String::Format::HumanReadableBytes, locale);
		ASSERT_EQUAL("test_human_readable_byte_size", "1 KiB", size);

		size = StormByte::Util::String::HumanReadable<double>(1154.65, StormByte::Util::String::Format::HumanReadableBytes, locale);
		ASSERT_EQUAL("test_human_readable_byte_size", "1.13 KiB", size);
    } catch (const StormByte::Exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    RETURN_TEST("test_human_readable_byte_size", result);
}

int test_human_readable_number() {
	int result = 0;
	try {
		std::string number = StormByte::Util::String::HumanReadable<int>(1024, StormByte::Util::String::Format::HumanReadableNumber, "en_US.UTF-8");
		ASSERT_EQUAL("test_human_readable_number", "1,024", number);

		number = StormByte::Util::String::HumanReadable<int>(1024 * 1024, StormByte::Util::String::Format::HumanReadableNumber, "en_US.UTF-8");
		ASSERT_EQUAL("test_human_readable_number", "1,048,576", number);
	} catch (const StormByte::Exception& ex) {
		std::cerr << ex.what() << std::endl;
		result++;
	}
	RETURN_TEST("test_human_readable_number", result);
}

int main() {
    int result = 0;
    try {
		result += test_simple_explode();
		result += test_path_explode();
		result += test_explode_one_item();
		result += test_temp_path();
		result += test_human_readable_byte_size();
		result += test_human_readable_number();

    } catch (const StormByte::Exception& ex) {
        std::cerr << ex.what() << std::endl;
        result++;
    }
    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}
