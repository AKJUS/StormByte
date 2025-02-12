#include <StormByte/config/file.hxx>
#include <StormByte/config/item.hxx>
#include <StormByte/config/item/value/integer.hxx>
#include <StormByte/config/item/value/string.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/system/functions.hxx>

#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <climits>

#define ASSERT_EQ(expected, actual) if ((expected) != (actual)) { \
    std::cerr << "Assertion failed at " << __FILE__ << ":" << __LINE__ << ": expected " << (expected) << ", got " << (actual) << std::endl; \
    return 1; \
}

class ConfigFile : public StormByte::Config::File {
public:
    using StormByte::Config::File::File;
    void PostRead() noexcept override {}
};

int non_existant_file_parse() {
	ConfigFile file("nonexistant.conf");
	try {
		file.Read();
		std::cerr << "File was somehow read and this should not happen" << std::endl;
		return 1;
	}
	catch (...) {
		// Expected exception
		return 0;
	}
	return 0;
}

int test_add_and_lookup() {
    std::string temp_file = StormByte::System::Functions::TempFileName();
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
    std::string temp_file = StormByte::System::Functions::TempFileName();
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
    std::string temp_file = StormByte::System::Functions::TempFileName();
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
    std::string temp_file = StormByte::System::Functions::TempFileName();
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
    std::string temp_file = StormByte::System::Functions::TempFileName();
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
    std::string temp_file = StormByte::System::Functions::TempFileName();
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
	ConfigFile cfg(CurrentFileDirectory / "files" / "bad_config1.conf");
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
	ConfigFile cfg(CurrentFileDirectory / "files" / "bad_config2.conf");
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
	ConfigFile cfg(CurrentFileDirectory / "files" / "bad_config3.conf");
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
	ConfigFile cfg(CurrentFileDirectory / "files" / "good_double_conf1.conf");
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

int good_double_conf2() {
	ConfigFile cfg(CurrentFileDirectory / "files" / "good_double_conf2.conf");
	try {
		cfg.Read();
		auto lookup_test_double = cfg.LookUp("test_double");
		ASSERT_EQ(19.89, lookup_test_double->AsDouble());
		auto lookup_test_exp = cfg.LookUp("test_exp");
		ASSERT_EQ(1.87e-6, lookup_test_exp->AsDouble());
		return 0;
	}
	catch(...) {
		std::cerr << "Got exception when we should not" << std::endl;
		return 1;
	}
	return 0;
}

int commented_config() {
	const std::string temp_file = StormByte::System::Functions::TempFileName();
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

int good_string_conf() {
	ConfigFile cfg(CurrentFileDirectory / "files" / "good_string_conf.conf");
	try {
		cfg.Read();
		auto lookup_string = cfg.LookUp("test_string");
		ASSERT_EQ("This is a test string", lookup_string->AsString());

		auto lookup_quoted = cfg.LookUp("test_quoted");
		ASSERT_EQ("This \"quote\" allows more things", lookup_quoted->AsString());

		auto lookup_unfinished = cfg.LookUp("test_unfinished");
		ASSERT_EQ("When you see a \" you might have the start of a string", lookup_unfinished->AsString());

		return 0;
	}
	catch(...) {
		std::cerr << "Got exception when we should not" << std::endl;
		return 1;
	}
	return 0;
}

int test_empty_string() {
	std::string temp_file = StormByte::System::Functions::TempFileName();
    ConfigFile file(temp_file);

    auto str_item = file.Add("EmptyString", StormByte::Config::Item::Type::String);
    str_item->SetString("");

    auto lookup_str = file.LookUp("EmptyString");
    ASSERT_EQ("", lookup_str->AsString());

	std::remove(temp_file.c_str());

    return 0;
}

int test_integer_boundaries() {
	std::string temp_file = StormByte::System::Functions::TempFileName();
    ConfigFile file(temp_file);

    auto max_int_item = file.Add("MaxInt", StormByte::Config::Item::Type::Integer);
    max_int_item->SetInteger(INT_MAX);

    auto min_int_item = file.Add("MinInt", StormByte::Config::Item::Type::Integer);
    min_int_item->SetInteger(INT_MIN);

    auto lookup_max_int = file.LookUp("MaxInt");
    ASSERT_EQ(INT_MAX, lookup_max_int->AsInteger());

    auto lookup_min_int = file.LookUp("MinInt");
    ASSERT_EQ(INT_MIN, lookup_min_int->AsInteger());

	std::remove(temp_file.c_str());

    return 0;
}

int test_special_characters_in_string() {
	std::string temp_file = StormByte::System::Functions::TempFileName();
    ConfigFile file(temp_file);

    auto str_item = file.Add("SpecialChars", StormByte::Config::Item::Type::String);
    str_item->SetString("Line1\nLine2\tTabbed");

    auto lookup_str = file.LookUp("SpecialChars");
    ASSERT_EQ("Line1\nLine2\tTabbed", lookup_str->AsString());

	std::remove(temp_file.c_str());

    return 0;
}

int test_deeply_nested_groups() {
	std::string temp_file = StormByte::System::Functions::TempFileName();
    ConfigFile file(temp_file);

    auto group1 = file.Add("Group1", StormByte::Config::Item::Type::Group);
    auto group2 = group1->AsGroup().Add("Group2", StormByte::Config::Item::Type::Group);
    auto group3 = group2->AsGroup().Add("Group3", StormByte::Config::Item::Type::Group);
    auto group4 = group3->AsGroup().Add("Group4", StormByte::Config::Item::Type::Group);

    auto int_item = group4->AsGroup().Add("DeepInt", StormByte::Config::Item::Type::Integer);
    int_item->SetInteger(1234);

    auto lookup_int = file.LookUp("Group1/Group2/Group3/Group4/DeepInt");
    ASSERT_EQ(1234, lookup_int->AsInteger());

	std::remove(temp_file.c_str());

    return 0;
}

int test_invalid_syntax() {
	std::string temp_file = StormByte::System::Functions::TempFileName();
    ConfigFile file(temp_file);
    std::string invalid_config = "Invalid = { Unclosed }";

    try {
        file.ReadFromString(invalid_config);
        std::cerr << "Expected exception for invalid syntax but none was thrown.\n";
        return 1;
    } catch (const StormByte::Config::ParseError&) {
        // Expected exception
		std::remove(temp_file.c_str());
    }

    return 0;
}

int test_special_characters_string() {
    ConfigFile cfg(CurrentFileDirectory / "files" / "special_characters_conf.conf");
    try {
        cfg.Read();
        auto lookup_special = cfg.LookUp("special_string");
        ASSERT_EQ("This is a test string with special characters: \n, \t, \\", lookup_special->AsString());
        return 0;
    } catch (const StormByte::Config::Exception& ex) {
        std::cerr << "Got exception when we should not: " << ex.what() << std::endl;
        return 1;
    }
}

int test_long_string() {
    ConfigFile cfg(CurrentFileDirectory / "files" / "long_string_conf.conf");
    try {
        cfg.Read();
        auto lookup_long = cfg.LookUp("long_string");
        ASSERT_EQ(std::string(1000, 'a'), lookup_long->AsString());
        return 0;
    } catch (...) {
        std::cerr << "Got exception when we should not" << std::endl;
        return 1;
    }
}

int test_missing_semicolon() {
    ConfigFile cfg(CurrentFileDirectory / "files" / "missing_semicolon.conf");
    try {
        cfg.Read();
        std::cerr << "Config read ok when it should not!";
        return 1;
    } catch (...) {
        // Expected
    }
    return 0;
}

int test_unmatched_braces() {
    ConfigFile cfg(CurrentFileDirectory / "files" / "unmatched_braces.conf");
    try {
        cfg.Read();
        std::cerr << "Config read ok when it should not!";
        return 1;
    } catch (...) {
        // Expected
    }
    return 0;
}

int good_boolean_config1() {
	ConfigFile cfg(CurrentFileDirectory / "files" / "good_boolean_conf1.conf");
	try {
		cfg.Read();
		auto lookup_enable_feature = cfg.LookUp("settings/enable_feature");
        ASSERT_EQ(true, lookup_enable_feature->AsBool());

		auto lookup_enable_extra = cfg.LookUp("settings/enable_extra");
        ASSERT_EQ(false, lookup_enable_extra->AsBool());
		return 0;
	}
	catch (...) {
		std::cerr << "Got exception when not expecting it" << std::endl;
		return 1;
	}
	return 0;
}

int bad_boolean_config1() {
	ConfigFile cfg(CurrentFileDirectory / "files" / "bad_boolean_conf1.conf");
	try {
		cfg.Read();
		std::cerr << "File was read ok but it should have failed" << std::endl;
		return 1;
	}
	catch (...) {
		return 0;
	}
	return 0;
}

int main() {
    int result = 0;
    try {
		result += non_existant_file_parse();
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
		result += good_double_conf2();
        result += commented_config();
        result += good_string_conf();
        result += test_empty_string();
        result += test_integer_boundaries();
        result += test_special_characters_in_string();
        result += test_deeply_nested_groups();
        result += test_invalid_syntax();
        result += test_special_characters_string();
        result += test_long_string();
        result += test_missing_semicolon();
        result += test_unmatched_braces();
		result += good_boolean_config1();
		result += bad_boolean_config1();
    } catch (const StormByte::Config::Exception& ex) {
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
