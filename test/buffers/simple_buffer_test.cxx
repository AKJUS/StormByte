#include <StormByte/buffers/simple.hxx>
#include <StormByte/test_handlers.h>

#include <iostream>
#include <thread>

using namespace StormByte;

// Test comparisons
int test_simple_buffer() {
    Buffers::Simple buffer { "Hello", 5 };
    auto buffer_data = buffer.Data();
    std::string original("Hello"), expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
    ASSERT_EQUAL("test_simple_buffer", original, expected);
    RETURN_TEST("test_simple_buffer", 0);
}

int test_buffer_copy() {
    Buffers::Simple buffer1 { "Hello", 5 };
    Buffers::Simple buffer2(buffer1);
    std::string original(reinterpret_cast<const char*>(buffer1.Data().data()), buffer1.Size()), copy(reinterpret_cast<const char*>(buffer2.Data().data()), buffer2.Size());
    ASSERT_EQUAL("test_buffer_copy", original, copy);
    RETURN_TEST("test_buffer_copy", 0);
}

int test_buffer_move() {
    Buffers::Simple buffer1 { "Hello", 5 };
    Buffers::Simple buffer2(std::move(buffer1));
    std::string original("Hello"), expected(reinterpret_cast<const char*>(buffer2.Data().data()), buffer2.Size());
    ASSERT_EQUAL("test_buffer_move", original, expected);
    ASSERT_EQUAL("test_buffer_move", 0, buffer1.Size());
    RETURN_TEST("test_buffer_move", 0);
}

int test_buffer_append() {
    Buffers::Simple buffer;
    buffer << "Hello" << " " << "World!";
    std::string original("Hello World!"), expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
    ASSERT_EQUAL("test_buffer_append", original, expected);
    RETURN_TEST("test_buffer_append", 0);
}

int test_append_string() {
    Buffers::Simple buffer;
    std::string str = "Hello, World!";
    buffer << str;
    std::string expected(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
    ASSERT_EQUAL("test_append_string", str, expected);
    RETURN_TEST("test_append_string", 0);
}

int test_peek_function() {
    Buffers::Simple buffer { "Hello", 5 };
    auto peeked = buffer.Peek();
    ASSERT_TRUE("test_peek_function", peeked.has_value());
    ASSERT_EQUAL("test_peek_function", static_cast<char>(peeked.value()), 'H');

    auto read = buffer.Read(1);
    ASSERT_TRUE("test_peek_function", read.has_value());
    ASSERT_EQUAL("test_peek_function", static_cast<char>(read.value()[0]), 'H');

    RETURN_TEST("test_peek_function", 0);
}

int test_discard_function() {
    Buffers::Simple buffer;

    // Add data to the buffer
    std::string initial_data = "Hello, World!";
    buffer << initial_data;

    // Verify initial size and content
    ASSERT_EQUAL("test_discard_function", buffer.Size(), initial_data.size());
    std::string expected_initial(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size());
    ASSERT_EQUAL("test_discard_function", initial_data, expected_initial);

    // Discard the first 7 bytes ("Hello, ")
    buffer.Discard(7, Buffers::Read::Position::Begin);

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
    Buffers::Simple buffer;

    // Add data to the buffer
    std::string initial_data = "Hello, World!";
    buffer << initial_data;

    // Test Discard from Begin
    buffer.Discard(7, Buffers::Read::Position::Begin);
    ASSERT_EQUAL("test_discard_modes (Begin)", 6, buffer.Size());
    ASSERT_EQUAL("test_discard_modes (Begin)", "World!", std::string(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size()));

    // Reset buffer
    buffer.Clear();
    buffer << initial_data;

    // Test Discard from End
    buffer.Discard(6, Buffers::Read::Position::End);
    ASSERT_EQUAL("test_discard_modes (End)", 7, buffer.Size());
    ASSERT_EQUAL("test_discard_modes (End)", "Hello, ", std::string(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size()));

    // Reset buffer
    buffer.Clear();
    buffer << initial_data;

    // Test Discard Relative
    buffer.Seek(7, Buffers::Read::Position::Begin); // Move position to 7
    buffer.Discard(2, Buffers::Read::Position::Relative);
    ASSERT_EQUAL("test_discard_modes (Relative)", 11, buffer.Size());
    ASSERT_EQUAL("test_discard_modes (Relative)", "Hello, rld!", std::string(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size()));

    // Reset buffer
    buffer.Clear();
    buffer << initial_data;

    // Test Discard Absolute
    buffer.Discard(5, Buffers::Read::Position::Absolute);
    ASSERT_EQUAL("test_discard_modes (Absolute)", 8, buffer.Size());
    ASSERT_EQUAL("test_discard_modes (Absolute)", ", World!", std::string(reinterpret_cast<const char*>(buffer.Data().data()), buffer.Size()));

    RETURN_TEST("test_discard_modes", 0);
}

int main() {
    int result = 0;
    //result += test_simple_buffer();
    //result += test_buffer_copy();
    //result += test_buffer_move();
    //result += test_buffer_append();
    //result += test_append_string();
    //result += test_peek_function();
    //result += test_discard_function();
    result += test_discard_modes();

    if (result == 0) {
        std::cout << "All tests passed!" << std::endl;
    } else {
        std::cout << result << " tests failed." << std::endl;
    }
    return result;
}