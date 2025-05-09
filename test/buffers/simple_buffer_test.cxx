#include <StormByte/buffer/simple.hxx>
#include <StormByte/test_handlers.h>

#include <iostream>
#include <thread>

using namespace StormByte;

// Test comparisons
int test_simple_buffer() {
	Buffer::Simple buffer { "Hello", 5 };
	auto buffer_data = buffer.Data();
	std::string original("Hello"), expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_simple_buffer", original, expected);
	RETURN_TEST("test_simple_buffer", 0);
}

int test_buffer_copy() {
	Buffer::Simple buffer1 { "Hello", 5 };
	Buffer::Simple buffer2(buffer1);
	std::string original(reinterpret_cast<const char*>(buffer1.Data().data()), buffer1.Size()), copy(reinterpret_cast<const char*>(buffer2.Data().data()), buffer2.Size());
	ASSERT_EQUAL("test_buffer_copy", original, copy);
	RETURN_TEST("test_buffer_copy", 0);
}

int test_buffer_move() {
	Buffer::Simple buffer1 { "Hello", 5 };
	Buffer::Simple buffer2(std::move(buffer1));
	std::string original("Hello"), expected(reinterpret_cast<const char*>(buffer2.Data().data()), buffer2.Size());
	ASSERT_EQUAL("test_buffer_move", original, expected);
	ASSERT_EQUAL("test_buffer_move", 0, buffer1.Size());
	RETURN_TEST("test_buffer_move", 0);
}

int test_buffer_append() {
	Buffer::Simple buffer;
	buffer << "Hello" << " " << "World!";
	std::string original("Hello World!"), expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_buffer_append", original, expected);
	RETURN_TEST("test_buffer_append", 0);
}

int test_append_string() {
	Buffer::Simple buffer;
	std::string str = "Hello, World!";
	buffer << str;
	std::string expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_append_string", str, expected);
	RETURN_TEST("test_append_string", 0);
}

int test_peek_function() {
	Buffer::Simple buffer { "Hello", 5 };
	auto peeked = buffer.Peek();
	ASSERT_TRUE("test_peek_function", peeked.has_value());
	ASSERT_EQUAL("test_peek_function", static_cast<char>(peeked.value()), 'H');

	auto read = buffer.Read(1);
	ASSERT_TRUE("test_peek_function", read.has_value());
	ASSERT_EQUAL("test_peek_function", static_cast<char>(read.value()[0]), 'H');

	RETURN_TEST("test_peek_function", 0);
}

int test_discard_function() {
	Buffer::Simple buffer;

	// Add data to the buffer
	std::string initial_data = "Hello, World!";
	buffer << initial_data;

	// Verify initial size and content
	ASSERT_EQUAL("test_discard_function", buffer.Size(), initial_data.size());
	std::string expected_initial(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_discard_function", initial_data, expected_initial);

	// Discard the first 7 bytes ("Hello, ")
	buffer.Discard(7, Buffer::Read::Position::Begin);

	// Verify the new size and content
	std::string expected_remaining = "World!";
	ASSERT_EQUAL("test_discard_function", buffer.Size(), expected_remaining.size());
	std::string actual_remaining(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
	ASSERT_EQUAL("test_discard_function", expected_remaining, actual_remaining);

	// Read the remaining data and verify it matches
	auto read_data = buffer.Read(buffer.Size());
	ASSERT_TRUE("test_discard_function", read_data.has_value());
	std::string read_remaining(reinterpret_cast<const char*>(read_data->data()), read_data->size());
	ASSERT_EQUAL("test_discard_function", expected_remaining, read_remaining);

	RETURN_TEST("test_discard_function", 0);
}

int test_discard_modes() {
	Buffer::Simple buffer;

	// Add data to the buffer
	std::string initial_data = "Hello, World!";
	buffer << initial_data;

	// Test Discard from Begin
	buffer.Discard(7, Buffer::Read::Position::Begin);
	ASSERT_EQUAL("test_discard_modes (Begin)", 6, buffer.Size());
	ASSERT_EQUAL("test_discard_modes (Begin)", "World!", std::string(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size()));

	// Reset buffer
	buffer.Clear();
	buffer << initial_data;

	// Test Discard from End
	buffer.Discard(6, Buffer::Read::Position::End);
	ASSERT_EQUAL("test_discard_modes (End)", 7, buffer.Size());
	ASSERT_EQUAL("test_discard_modes (End)", "Hello, ", std::string(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size()));

	// Reset buffer
	buffer.Clear();
	buffer << initial_data;

	// Test Discard Relative
	buffer.Seek(7, Buffer::Read::Position::Begin); // Move position to 7
	buffer.Discard(2, Buffer::Read::Position::Relative);
	ASSERT_EQUAL("test_discard_modes (Relative)", 11, buffer.Size());
	ASSERT_EQUAL("test_discard_modes (Relative)", "Hello, rld!", std::string(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size()));

	// Reset buffer
	buffer.Clear();
	buffer << initial_data;

	// Test Discard Absolute
	buffer.Discard(5, Buffer::Read::Position::Absolute);
	ASSERT_EQUAL("test_discard_modes (Absolute)", 8, buffer.Size());
	ASSERT_EQUAL("test_discard_modes (Absolute)", ", World!", std::string(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size()));

	RETURN_TEST("test_discard_modes", 0);
}

int test_process_function() {
	Buffer::Simple input_buffer;
	Buffer::Simple output_buffer;

	// Add data to the input buffer
	std::string initial_data = "Hello, World!";
	input_buffer << initial_data;

	// Define a processing function that converts all characters to uppercase
	auto to_uppercase = [](const Buffer::Simple& buffer) -> std::shared_ptr<Buffer::Simple> {
		auto data = buffer.Data();
		auto result = std::make_shared<Buffer::Simple>();
		for (auto byte : data) {
			char c = static_cast<char>(byte);
			result->Write(std::string(1, std::toupper(c)));
		}
		return result;
	};

	// Process the data
	auto status = input_buffer.Process(initial_data.size(), to_uppercase, output_buffer);

	// Verify the status
	ASSERT_TRUE("test_process_function", Buffer::Read::Status::Success == status);

	// Verify the output buffer content
	std::string expected_output = "HELLO, WORLD!";
	std::string actual_output(reinterpret_cast<const char*>(output_buffer.Data().data()), output_buffer.Size());
	ASSERT_EQUAL("test_process_function", expected_output, actual_output);

	// Verify that the input buffer is empty after processing
	ASSERT_TRUE("test_process_function", input_buffer.Empty());

	RETURN_TEST("test_process_function", 0);
}

int test_extract_into() {
	Buffer::Simple source_buffer;
	Buffer::Simple target_buffer;

	// Add data to the source buffer
	std::string initial_data = "Hello, World!";
	source_buffer << initial_data;

	// Extract 5 bytes ("Hello") into the target buffer
	auto status = source_buffer.ExtractInto(5, target_buffer);

	// Verify the status
	ASSERT_TRUE("test_extract_into", Buffer::Read::Status::Success == status);

	// Verify the target buffer content
	std::string expected_target = "Hello";
	std::string actual_target(reinterpret_cast<const char*>(target_buffer.Data().data()), target_buffer.Size());
	ASSERT_EQUAL("test_extract_into", expected_target, actual_target);

	// Verify the source buffer content after extraction
	std::string expected_source = ", World!";
	std::string actual_source(reinterpret_cast<const char*>(source_buffer.Data().data()), source_buffer.Size());
	ASSERT_EQUAL("test_extract_into", expected_source, actual_source);

	// Verify the source buffer's position is updated correctly
	ASSERT_EQUAL("test_extract_into", source_buffer.Size(), expected_source.size());

	RETURN_TEST("test_extract_into", 0);
}

int test_simple_available_bytes() {
    Buffer::Simple buffer;

    // Add data to the buffer
    std::string data = "Hello, World!";
    buffer << data;

    // Verify the initial available bytes
    ASSERT_EQUAL("test_simple_available_bytes (initial)", data.size(), buffer.AvailableBytes());

    // Read 7 bytes ("Hello, ")
    buffer.Read(7);

    // Verify the available bytes after reading
    ASSERT_EQUAL("test_simple_available_bytes (after read)", data.size() - 7, buffer.AvailableBytes());

    // Read the remaining bytes
    buffer.Read(buffer.AvailableBytes());

    // Verify the available bytes after reading everything
    ASSERT_EQUAL("test_simple_available_bytes (after full read)", 0, buffer.AvailableBytes());

    RETURN_TEST("test_simple_available_bytes", 0);
}

int test_copy_empty_buffer() {
	Buffer::Simple buffer1;
	Buffer::Simple buffer2(buffer1);
	ASSERT_TRUE("test_copy_empty_buffer", buffer1.Empty());
	ASSERT_TRUE("test_copy_empty_buffer", buffer1.Empty());
	RETURN_TEST("test_copy_empty_buffer", 0);
}

int test_move_empty_buffer() {
	Buffer::Simple buffer1;
	Buffer::Simple buffer2(std::move(buffer1));
	ASSERT_TRUE("test_move_empty_buffer", buffer1.Empty());
	ASSERT_TRUE("test_move_empty_buffer", buffer2.Empty());
	RETURN_TEST("test_move_empty_buffer", 0);
}

int main() {
	int result = 0;
	result += test_simple_buffer();
	result += test_buffer_copy();
	result += test_buffer_move();
	result += test_buffer_append();
	result += test_append_string();
	result += test_peek_function();
	result += test_discard_function();
	result += test_discard_modes();
	result += test_process_function();
	result += test_extract_into();
	result += test_simple_available_bytes();
	result += test_copy_empty_buffer();
	result += test_move_empty_buffer();

	if (result == 0) {
		std::cout << "All tests passed!" << std::endl;
	} else {
		std::cout << result << " tests failed." << std::endl;
	}
	return result;
}