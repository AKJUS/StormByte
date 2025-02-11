#include <StormByte/config/file.hxx>
#include <StormByte/config/item.hxx>
#include <StormByte/config/item/value/integer.hxx>
#include <StormByte/config/item/value/string.hxx>
#include <StormByte/config/item/group.hxx>

#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <cstdlib>  // For mkstemp
#include <unistd.h> // For close

#define ASSERT_EQ(expected, actual) if ((expected) != (actual)) { \
    std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << ": expected " << (expected) << ", got " << (actual) << std::endl; \
    return 1; \
}

class ConfigFile : public StormByte::Config::File {
public:
    using StormByte::Config::File::File;
    void PostRead() noexcept override {}
};

std::string get_temp_filename() {
    char temp_filename[] = "/tmp/config_testXXXXXX";
    int fd = mkstemp(temp_filename);
    if (fd == -1) {
        throw std::runtime_error("Failed to create temporary file");
    }
    close(fd);
    return std::string(temp_filename);
}

std::filesystem::path get_current_path() {
	return std::filesystem::path(__FILE__).parent_path();
}

int test_add_and_lookup() {
    std::string temp_file = get_temp_filename();
    ConfigFile file(temp_file);

    // Add Integer and String items
    auto int_item = file.Add("TestInt", StormByte::Config::Item::Type::Integer);
    int_item->SetInteger(42);
    auto str_item = file.Add("TestStr", StormByte::Config::Item::Type::String);
    str_item->SetString("Hello, World!");

    // Lookup and validate items
    auto lookup_int = file.LookUp("TestInt");
    ASSERT_EQ(42, lookup_int->AsInteger());

    auto lookup_str = file.LookUp("TestStr");
    ASSERT_EQ("Hello, World!", lookup_str->AsString());

    std::remove(temp_file.c_str());
    return 0;
}

int test_write_and_read() {
    std::string temp_file = get_temp_filename();
    std::string config_content = 
        "TestInt = 42;\n"
        "TestStr = \"Hello, World!\";\n";

    ConfigFile file(temp_file);

    // Read from string
    file.ReadFromString(config_content);

    // Validate items
    auto int_item = file.LookUp("TestInt");
    ASSERT_EQ(42, int_item->AsInteger());

    auto str_item = file.LookUp("TestStr");
    ASSERT_EQ("Hello, World!", str_item->AsString());

    // Write to file
    file.Write();

    // Read again from file to ensure correctness
    ConfigFile file2(temp_file);
    file2.Read();

    auto int_item2 = file2.LookUp("TestInt");
    ASSERT_EQ(42, int_item2->AsInteger());

    auto str_item2 = file2.LookUp("TestStr");
    ASSERT_EQ("Hello, World!", str_item2->AsString());

    std::remove(temp_file.c_str());
    return 0;
}

int test_nested_groups() {
    std::string temp_file = get_temp_filename();
    ConfigFile file(temp_file);

    // Create nested groups
    auto group1 = file.Add("Group1", StormByte::Config::Item::Type::Group);
    auto group2 = group1->AsGroup().Add("Group2", StormByte::Config::Item::Type::Group);

    // Add items to sub-group
    auto int_item = group2->AsGroup().Add("SubTestInt", StormByte::Config::Item::Type::Integer);
    int_item->SetInteger(99);
    auto str_item = group2->AsGroup().Add("SubTestStr", StormByte::Config::Item::Type::String);
    str_item->SetString("Sub Hello");

    // Lookup and validate items
    auto lookup_int = file.LookUp("Group1/Group2/SubTestInt");
    ASSERT_EQ(99, lookup_int->AsInteger());

    auto lookup_str = file.LookUp("Group1/Group2/SubTestStr");
    ASSERT_EQ("Sub Hello", lookup_str->AsString());

    std::remove(temp_file.c_str());
    return 0;
}

int test_add_remove_group() {
    std::string temp_file = get_temp_filename();
    ConfigFile file(temp_file);

    // Add group and items
    auto group = file.Add("TestGroup", StormByte::Config::Item::Type::Group);
    auto int_item = group->AsGroup().Add("GroupInt", StormByte::Config::Item::Type::Integer);
    int_item->SetInteger(55);

    // Remove the item from the group
    group->AsGroup().Remove("GroupInt");

    // Validate removal
    try {
        file.LookUp("TestGroup/GroupInt");
        std::cerr << "Item not removed properly" << std::endl;
        return 1;
    } catch (const StormByte::Config::ItemNotFound&) {
        // Expected outcome
    }

    std::remove(temp_file.c_str());
    return 0;
}

int test_write_nested_groups() {
    std::string temp_file = get_temp_filename();
    std::string config_content = 
        "Group1 = {\n"
        "    Group2 = {\n"
        "        SubTestInt = 99;\n"
        "        SubTestStr = \"Sub Hello\";\n"
        "    };\n"
        "};\n";

    ConfigFile file(temp_file);

    // Read from string
    file.ReadFromString(config_content);

    // Validate items
    auto lookup_int = file.LookUp("Group1/Group2/SubTestInt");
    ASSERT_EQ(99, lookup_int->AsInteger());

    auto lookup_str = file.LookUp("Group1/Group2/SubTestStr");
    ASSERT_EQ("Sub Hello", lookup_str->AsString());

    // Write to file
    file.Write();

    // Read again from file to ensure correctness
    ConfigFile file2(temp_file);
    file2.Read();

    auto lookup_int2 = file2.LookUp("Group1/Group2/SubTestInt");
    ASSERT_EQ(99, lookup_int2->AsInteger());

    auto lookup_str2 = file2.LookUp("Group1/Group2/SubTestStr");
    ASSERT_EQ("Sub Hello", lookup_str2->AsString());

    std::remove(temp_file.c_str());
    return 0;
}

int test_complex_config_creation() {
    std::string temp_file = get_temp_filename();
    ConfigFile file(temp_file);

    // Create a complex configuration
    auto group1 = file.Add("Group1", StormByte::Config::Item::Type::Group);
    auto group2 = group1->AsGroup().Add("Group2", StormByte::Config::Item::Type::Group);

    auto int_item1 = group2->AsGroup().Add("IntItem1", StormByte::Config::Item::Type::Integer);
    int_item1->SetInteger(123);
    auto str_item1 = group2->AsGroup().Add("StrItem1", StormByte::Config::Item::Type::String);
    str_item1->SetString("Nested String");

    auto group3 = file.Add("Group3", StormByte::Config::Item::Type::Group);
    auto int_item2 = group3->AsGroup().Add("IntItem2", StormByte::Config::Item::Type::Integer);
    int_item2->SetInteger(456);

    // Write to a temporary file
    file.Write();

    // Validate the written content
    std::ifstream temp_file_stream(temp_file);
    std::stringstream buffer;
    buffer << temp_file_stream.rdbuf();

    std::string expected_content = 
        "Group1 = {\n"
        "\tGroup2 = {\n"
        "\t\tIntItem1 = 123;\n"
        "\t\tStrItem1 = \"Nested String\";\n"
        "\t};\n"
        "};\n"
        "Group3 = {\n"
        "\tIntItem2 = 456;\n"
        "};\n";

    ASSERT_EQ(expected_content, buffer.str());

    std::remove(temp_file.c_str());
    return 0;
}

int bad_config1() {
	ConfigFile cfg(get_current_path() / "bad_config1.conf");
	try {
		cfg.Read();
		std::cerr << "Config read ok when it should not!";
		return 1;
	}
	catch(...) {
		//Expected
	}
	return 0;
}

int bad_config2() {
	ConfigFile cfg(get_current_path() / "bad_config2.conf");
	try {
		cfg.Read();
		std::cerr << "Config read ok when it should not!";
		return 1;
	}
	catch(...) {
		//Expected
	}
	return 0;
}

int bad_config3() {
	ConfigFile cfg(get_current_path() / "bad_config3.conf");
	try {
		cfg.Read();
		std::cerr << "Config read ok when it should not!";
		return 1;
	}
	catch(...) {
		//Expected
	}
	return 0;
}

int good_double_conf1() {
	ConfigFile cfg(get_current_path() / "good_double_conf1.conf");
	try {
		cfg.Read();
		auto lookup_double = cfg.LookUp("test_double");
		ASSERT_EQ(666.666, lookup_double->AsDouble());
		return 0;
	}
	catch(...) {
		std::cerr << "Got exception when we should not" << std::endl;
		return 1;
	}
	return 0;
}

int commented_config() {
	const std::string temp_file = get_temp_filename();
	ConfigFile file(temp_file);
	const std::string config_str = "# The following is a test integer\n"
		"test_integer = 666;\n"
		"\n"
		"# Now a group\n"
		"test_group = { # We can have a comment here!\n"
		"\t# And also here\n"
		"\ttest_string = \"# But this is not a comment\";\n"
		"};\n"
		"# Ending comment no newline";
	const std::string expected_str = "# The following is a test integer\n"
		"test_integer = 666;\n"
		"# Now a group\n"
		"test_group = {\n"
		"\t# We can have a comment here!\n"
		"\t# And also here\n"
		"\ttest_string = \"# But this is not a comment\";\n"
		"};\n"
		"# Ending comment no newline\n";
	file.ReadFromString(config_str);
	file.Write();

	auto test_string = file.LookUp("test_group/test_string");
	ASSERT_EQ("# But this is not a comment", test_string->AsString());

	// Validate the written content
    std::ifstream temp_file_stream(temp_file);
    std::stringstream buffer;
    buffer << temp_file_stream.rdbuf();

	ASSERT_EQ(expected_str, buffer.str());

    std::remove(temp_file.c_str());

	return 0;
}

int main() {
    int result = 0;
	try {
		result += test_add_and_lookup();
		result += test_write_and_read();
		result += test_nested_groups();
		result += test_add_remove_group();
		result += test_write_nested_groups();
    	result += test_complex_config_creation();
		result += bad_config1();
		result += bad_config2();
		result += bad_config3();
		result += good_double_conf1();
		result += commented_config();
	}
	catch (const StormByte::Config::Exception& ex) {
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
