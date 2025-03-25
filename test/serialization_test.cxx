#include <StormByte/serializable.hxx>
#include <StormByte/test_handlers.h>

#include <format>
#include <map>
#include <optional>
#include <string>
#include <vector>

using namespace StormByte;

int test_serialize_int() {
	int data = 42;
	Serializable<int> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_int", 1);

	auto expected_data = Serializable<int>::Deserialize(buffer);
	if (!expected_data)
		RETURN_TEST("test_serialize_int", 1);
	
	ASSERT_EQUAL("test_serialize_int", data, expected_data.value());
	RETURN_TEST("test_serialize_int", 0);
}

int test_serialize_double() {
	double data = 777.777;
	Serializable<double> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_double", 1);
	
	auto expected_data = Serializable<double>::Deserialize(buffer);
	if (!expected_data)
		RETURN_TEST("test_serialize_double", 1);
	
	ASSERT_EQUAL("test_serialize_double", data, expected_data.value());
	RETURN_TEST("test_serialize_double", 0);
}

int test_serialize_string() {
	std::string data = "Hello, World!";
	Serializable<std::string> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_string", 1);
	
	auto expected_data = Serializable<std::string>::Deserialize(buffer);
	if (!expected_data)
		RETURN_TEST("test_serialize_string", 1);
	
	ASSERT_EQUAL("test_serialize_string", data, expected_data.value());
	RETURN_TEST("test_serialize_string", 0);
}

int test_serialize_size_t() {
	std::string data = "Hello, World!";
	std::size_t size = data.size();
	Serializable<std::size_t> serialization(size);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_size_t", 1);
	
	auto expected_data = Serializable<std::size_t>::Deserialize(buffer);
	if (!expected_data)
		RETURN_TEST("test_serialize_size_t", 1);

	ASSERT_EQUAL("test_serialize_size_t", data.size(), expected_data.value());
	RETURN_TEST("test_serialize_size_t", 0);
}

int test_serialize_string_vector() {
	std::vector<std::string> data = { "Hello", "World!" };
	Serializable<std::vector<std::string>> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_string_vector", 1);
	
	auto expected_data = Serializable<std::vector<std::string>>::Deserialize(buffer);
	if (!expected_data) {
		std::cerr << expected_data.error()->what() << std::endl;
		RETURN_TEST("test_serialize_string_vector", 1);
	}

	ASSERT_TRUE("test_serialize_string_vector", data == expected_data.value());
	RETURN_TEST("test_serialize_string_vector", 0);
}

int test_serialize_pair() {
	std::pair<int, double> data = { 42, 777.777 };
	Serializable<std::pair<int, double>> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_pair", 1);
	
	auto expected_data = Serializable<std::pair<int, double>>::Deserialize(buffer);
	if (!expected_data) {
		std::cerr << expected_data.error()->what() << std::endl;
		RETURN_TEST("test_serialize_pair", 1);
	}

	ASSERT_TRUE("test_serialize_pair", data == expected_data.value());
	RETURN_TEST("test_serialize_pair", 0);
}

int test_serialize_map() {
	std::map<int, std::string> data = { { 1, "Hello" }, { 2, "World!" } };
	Serializable<std::map<int, std::string>> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_map", 1);
	
	auto expected_data = Serializable<std::map<int, std::string>>::Deserialize(buffer);
	if (!expected_data) {
		std::cerr << expected_data.error()->what() << std::endl;
		RETURN_TEST("test_serialize_map", 1);
	}

	ASSERT_TRUE("test_serialize_map", data == expected_data.value());
	RETURN_TEST("test_serialize_map", 0);
}

int test_serialize_int_truncated() {
    int data = 42;
    Serializable<int> serialization(data);
    Buffer buffer = serialization.Serialize();
    if (buffer.Size() == 0)
        RETURN_TEST("test_serialize_int_truncated", 1);

    // Truncamos el buffer a la mitad de su tamaño esperado
    auto expected_truncated_buffer = buffer.Read(sizeof(int) / 2);
	if (!expected_truncated_buffer) {
		std::cerr << expected_truncated_buffer.error()->what() << std::endl;
		RETURN_TEST("test_serialize_int_truncated", 1);
	}
    auto expected_data = Serializable<int>::Deserialize(expected_truncated_buffer.value());
    if (expected_data) {
        std::cerr << "Expected failure, but got value: " << expected_data.value() << std::endl;
        RETURN_TEST("test_serialize_int_truncated", 1);
    }

    RETURN_TEST("test_serialize_int_truncated", 0);
}

int test_serialize_string_vector_truncated() {
    std::vector<std::string> data = { "Hello", "World!" };
    Serializable<std::vector<std::string>> serialization(data);
    Buffer buffer = serialization.Serialize();
    if (buffer.Size() == 0)
        RETURN_TEST("test_serialize_string_vector_truncated", 1);

    // Truncamos el buffer para que solo incluya el tamaño y parte del primer elemento
    std::size_t truncated_length = sizeof(std::size_t) + 2; // Tamaño + 2 bytes del primer string
	auto expected_truncated_buffer = buffer.Read(truncated_length);
	if (!expected_truncated_buffer) {
		std::cerr << expected_truncated_buffer.error()->what() << std::endl;
		RETURN_TEST("test_serialize_string_vector_truncated", 1);
	}
    auto expected_data = Serializable<std::vector<std::string>>::Deserialize(expected_truncated_buffer.value());
    if (expected_data) {
        std::cerr << "Expected failure, but got value" << std::endl;
        RETURN_TEST("test_serialize_string_vector_truncated", 1);
    }

    RETURN_TEST("test_serialize_string_vector_truncated", 0);
}

int test_serialize_pair_truncated() {
    std::pair<int, double> data = { 42, 777.777 };
    Serializable<std::pair<int, double>> serialization(data);
    Buffer buffer = serialization.Serialize();
    if (buffer.Size() == 0)
        RETURN_TEST("test_serialize_pair_truncated", 1);

    // Truncamos el buffer para que solo incluya el int (primer elemento)
    std::size_t truncated_length = sizeof(int);
	auto expected_truncated_buffer = buffer.Read(truncated_length);
	if (!expected_truncated_buffer) {
		std::cerr << expected_truncated_buffer.error()->what() << std::endl;
		RETURN_TEST("test_serialize_pair_truncated", 1);
	}
    auto expected_data = Serializable<std::pair<int, double>>::Deserialize(expected_truncated_buffer.value());
    if (expected_data) {
        std::cerr << "Expected failure, but got value: (" << expected_data.value().first << ", " << expected_data.value().second << ")" << std::endl;
        RETURN_TEST("test_serialize_pair_truncated", 1);
    }

    RETURN_TEST("test_serialize_pair_truncated", 0);
}

int test_serialize_optional_notempty() {
	std::optional<int> data = 42;
	Serializable<std::optional<int>> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_optional_notempty", 1);

	auto expected_data = Serializable<std::optional<int>>::Deserialize(buffer);
	if (!expected_data) {
		std::cerr << expected_data.error()->what() << std::endl;
		RETURN_TEST("test_serialize_optional_notempty", 1);
	}

	ASSERT_EQUAL("test_serialize_optional_notempty", data.value(), expected_data.value().value());
	RETURN_TEST("test_serialize_optional_notempty", 0);
}

int test_serialize_optional_empty() {
	std::optional<int> data;
	Serializable<std::optional<int>> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_optional_empty", 1);

	auto expected_data = Serializable<std::optional<int>>::Deserialize(buffer);
	if (!expected_data) {
		std::cerr << expected_data.error()->what() << std::endl;
		RETURN_TEST("test_serialize_optional_empty", 1);
	}

	ASSERT_FALSE("test_serialize_optional_empty", expected_data.value().has_value());
	RETURN_TEST("test_serialize_optional_empty", 0);
}

int test_serialize_optional_string() {
	std::optional<std::string> data = "Hello, World!";
	Serializable<std::optional<std::string>> serialization(data);
	Buffer buffer = serialization.Serialize();
	if (buffer.Size() == 0)
		RETURN_TEST("test_serialize_optional_string", 1);

	auto expected_data = Serializable<std::optional<std::string>>::Deserialize(buffer);
	if (!expected_data) {
		std::cerr << expected_data.error()->what() << std::endl;
		RETURN_TEST("test_serialize_optional_string", 1);
	}

	ASSERT_EQUAL("test_serialize_optional_string", data.value(), expected_data.value().value());
	RETURN_TEST("test_serialize_optional_string", 0);
}

int main() {
    int result = 0;
	result += test_serialize_int();
	result += test_serialize_double();
	result += test_serialize_string();
	result += test_serialize_size_t();
	result += test_serialize_string_vector();
	result += test_serialize_pair();
	result += test_serialize_map();
	result += test_serialize_int_truncated();
	result += test_serialize_string_vector_truncated();
	result += test_serialize_pair_truncated();
	result += test_serialize_optional_notempty();
	result += test_serialize_optional_empty();
	result += test_serialize_optional_string();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}
