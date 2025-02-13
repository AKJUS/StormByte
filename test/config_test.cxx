#include <StormByte/config/file.hxx>
#include <StormByte/config/item.hxx>
#include <StormByte/config/item/value/integer.hxx>
#include <StormByte/config/item/value/string.hxx>
#include <StormByte/config/item/group.hxx>
#include <StormByte/system/system.hxx>

#include <iostream>
#include <cassert>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <climits>
#include "test_handlers.h"

using namespace StormByte;

int test_add_and_lookup() {
	int result = 0;
    Config::File config;

    // Add Integer and String items
    auto int_item = config.Add("TestInt", StormByte::Config::Item::Type::Integer);
    int_item->SetInteger(42);
    auto str_item = config.Add("TestStr", StormByte::Config::Item::Type::String);
    str_item->SetString("Hello, World!");

	try {
		// Lookup and validate items
		auto lookup_int = config.LookUp("TestInt");
		ASSERT_EQUAL("test_add_and_lookup", 42, lookup_int->AsInteger());

		auto lookup_str = config.LookUp("TestStr");
		ASSERT_EQUAL("test_add_and_lookup", "Hello, World!", lookup_str->AsString());
	}
	catch(...) {
		result = 1;
	}

    RETURN_TEST("test_add_and_lookup", result);
}

int test_write_and_read() {
	int result = 0;
    std::filesystem::path temp_file = StormByte::System::TempFileName();
    std::string config_content = 
        "TestInt = 42;\n"
        "TestStr = \"Hello, World!\";\n";

    Config::File config;

	try {
		// Read from string
		config << config_content;

		// Validate items
		auto int_item = config.LookUp("TestInt");
		ASSERT_EQUAL("test_write_and_read", 42, int_item->AsInteger());

		auto str_item = config.LookUp("TestStr");
		ASSERT_EQUAL("test_write_and_read", "Hello, World!", str_item->AsString());

		// Write to file
		std::fstream file;
		file.open(temp_file, std::ios::out);
		file << config;
		file.close();

		// Read again from file to ensure correctness
		file.open(temp_file, std::ios::in);
		Config::File config2;
		file >> config2;
		file.close();

		auto int_item2 = config2.LookUp("TestInt");
		ASSERT_EQUAL("test_write_and_read", 42, int_item2->AsInteger());

		auto str_item2 = config2.LookUp("TestStr");
		ASSERT_EQUAL("test_write_and_read", "Hello, World!", str_item2->AsString());
	}
	catch(...) {
		result = 1;
	}

    std::remove(temp_file.string().c_str());
    RETURN_TEST("test_write_and_read", result);
}

int test_nested_groups() {
	int result = 0;
    Config::File config;

	try {
		// Create nested groups
		auto group1 = config.Add("Group1", StormByte::Config::Item::Type::Group);
		auto group2 = group1->AsGroup().Add("Group2", StormByte::Config::Item::Type::Group);

		// Add items to sub-group
		auto int_item = group2->AsGroup().Add("SubTestInt", StormByte::Config::Item::Type::Integer);
		int_item->SetInteger(99);
		auto str_item = group2->AsGroup().Add("SubTestStr", StormByte::Config::Item::Type::String);
		str_item->SetString("Sub Hello");

		// Lookup and validate items
		auto lookup_int = config.LookUp("Group1/Group2/SubTestInt");
		ASSERT_EQUAL("test_nested_groups", 99, lookup_int->AsInteger());

		auto lookup_str = config.LookUp("Group1/Group2/SubTestStr");
		ASSERT_EQUAL("test_nested_groups", "Sub Hello", lookup_str->AsString());
	}
	catch(...) {
		result = 1;
	}

    RETURN_TEST("test_nested_groups", result);
}

int test_add_remove_group() {
	int result = 0;
    Config::File config;

	try {
		// Add group and items
		auto group = config.Add("TestGroup", StormByte::Config::Item::Type::Group);
		auto int_item = group->AsGroup().Add("GroupInt", StormByte::Config::Item::Type::Integer);
		int_item->SetInteger(55);

		// Remove the item from the group
		group->AsGroup().Remove("GroupInt");

		// Validate removal
		config.LookUp("TestGroup/GroupInt");
		// Item not removed properly
		result = 1;
	}
	catch(...) {
		result = 0;
	}

    RETURN_TEST("test_add_remove_group", result);
}

int test_write_nested_groups() {
	int result = 0;
    std::filesystem::path temp_file = StormByte::System::TempFileName();
    std::string config_content = 
        "Group1 = {\n"
        "    Group2 = {\n"
        "        SubTestInt = 99;\n"
        "        SubTestStr = \"Sub Hello\";\n"
        "    };\n"
        "};\n";

    Config::File config;

	try {
		// Read from string
		config_content >> config;

		// Validate items
		auto lookup_int = config.LookUp("Group1/Group2/SubTestInt");
		ASSERT_EQUAL("test_write_nested_groups", 99, lookup_int->AsInteger());

		auto lookup_str = config.LookUp("Group1/Group2/SubTestStr");
		ASSERT_EQUAL("test_write_nested_groups", "Sub Hello", lookup_str->AsString());

		// Write to file
		std::fstream file;
		file.open(temp_file, std::ios::out);
		file << config;
		file.close();

		// Read again from file to ensure correctness
		file.open(temp_file, std::ios::in);
		Config::File config2;
		config2 << file;
		file.close();

		auto lookup_int2 = config2.LookUp("Group1/Group2/SubTestInt");
		ASSERT_EQUAL("test_write_nested_groups", 99, lookup_int2->AsInteger());

		auto lookup_str2 = config2.LookUp("Group1/Group2/SubTestStr");
		ASSERT_EQUAL("test_write_nested_groups", "Sub Hello", lookup_str2->AsString());
	}
	catch(...) {
		result = 1;
	}

    std::remove(temp_file.string().c_str());
    RETURN_TEST("test_write_nested_groups", result);
}

int test_complex_config_creation() {
	int result = 0;
    std::filesystem::path temp_file = StormByte::System::TempFileName();
    Config::File config;

	try {
		// Create a complex configuration
		auto group1 = config.Add("Group1", StormByte::Config::Item::Type::Group);
		auto group2 = group1->AsGroup().Add("Group2", StormByte::Config::Item::Type::Group);

		auto int_item1 = group2->AsGroup().Add("IntItem1", StormByte::Config::Item::Type::Integer);
		int_item1->SetInteger(123);
		auto str_item1 = group2->AsGroup().Add("StrItem1", StormByte::Config::Item::Type::String);
		str_item1->SetString("Nested String");

		auto group3 = config.Add("Group3", StormByte::Config::Item::Type::Group);
		auto int_item2 = group3->AsGroup().Add("IntItem2", StormByte::Config::Item::Type::Integer);
		int_item2->SetInteger(456);

		// Write to a temporary file
		std::fstream file;
		file.open(temp_file, std::ios::out);
		file << config;
		file.close();

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

		ASSERT_EQUAL("test_complex_config_creation", expected_content, buffer.str());
	}
	catch(...) {
		result = 1;
	}

    std::remove(temp_file.string().c_str());
    RETURN_TEST("test_write_nested_groups", result);
}

int bad_config1() {
	int result = 0;
	Config::File cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "bad_config1.conf", std::ios::in);
		cfg << file;
		file.close();
		result = 1;
	}
	catch(...) {
		//Expected
	}
	
	RETURN_TEST("bad_config1", result);
}

int bad_config2() {
	int result = 0;
	Config::File cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "bad_config2.conf", std::ios::in);
		cfg << file;
		file.close();
		result = 1;
	}
	catch(...) {
		//Expected
	}
	
	RETURN_TEST("bad_config2", result);
}

int bad_config3() {
	int result = 0;
	Config::File cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "bad_config3.conf", std::ios::in);
		cfg << file;
		file.close();
		result = 1;
	}
	catch(...) {
		//Expected
	}
	
	RETURN_TEST("bad_config3", result);
}

int good_double_conf1() {
	int result = 0;
	Config::File cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_double_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		auto lookup_double = cfg.LookUp("test_double");
		ASSERT_EQUAL("good_double_conf1", 666.666, lookup_double->AsDouble());
	}
	catch(...) {
		result = 1;
	}
	
	RETURN_TEST("bad_config3", result);
}

int good_double_conf2() {
	int result = 0;
	Config::File cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_double_conf2.conf", std::ios::in);
		cfg << file;
		file.close();
		auto lookup_test_double = cfg.LookUp("test_double");
		ASSERT_EQUAL("good_double_conf2", 19.89, lookup_test_double->AsDouble());
		auto lookup_test_exp = cfg.LookUp("test_exp");
		ASSERT_EQUAL("good_double_conf2", 1.87e-6, lookup_test_exp->AsDouble());
	}
	catch(...) {
		result = 1;
	}
	
	RETURN_TEST("good_double_conf2", result);
}

int commented_config() {
	int result = 0;
	const std::filesystem::path temp_file = StormByte::System::TempFileName();
	Config::File config;
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

	try {
		config << config_str;
		std::fstream file;
		file.open(temp_file, std::ios::out);
		config >> file;
		file.close();

		auto test_string = config.LookUp("test_group/test_string");
		ASSERT_EQUAL("commented_config", "# But this is not a comment", test_string->AsString());

		// Validate the written content
		std::ifstream temp_file_stream(temp_file);
		std::stringstream buffer;
		buffer << temp_file_stream.rdbuf();

		ASSERT_EQUAL("commented_config", expected_str, buffer.str());
	}
	catch(...) {
		result = 1;
	}

    std::remove(temp_file.string().c_str());

	RETURN_TEST("good_double_conf2", result);
}

int good_string_conf() {
	int result = 0;
	Config::File cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_string_conf.conf", std::ios::in);
		cfg << file;
		file.close();
		auto lookup_string = cfg.LookUp("test_string");
		ASSERT_EQUAL("good_string_conf", "This is a test string", lookup_string->AsString());

		auto lookup_quoted = cfg.LookUp("test_quoted");
		ASSERT_EQUAL("good_string_conf", "This \"quote\" allows more things", lookup_quoted->AsString());

		auto lookup_unfinished = cfg.LookUp("test_unfinished");
		ASSERT_EQUAL("good_string_conf", "When you see a \" you might have the start of a string", lookup_unfinished->AsString());
	}
	catch(...) {
		result = 1;
	}
	
	RETURN_TEST("good_string_conf", result);
}

int test_empty_string() {
	int result = 0;
    Config::File config;

    auto str_item = config.Add("EmptyString", StormByte::Config::Item::Type::String);
    str_item->SetString("");

	try {
    	auto lookup_str = config.LookUp("EmptyString");
    	ASSERT_EQUAL("test_empty_string", "", lookup_str->AsString());
	}
	catch(...) {
		result = 1;
	}

    RETURN_TEST("test_empty_string", result);
}

int test_integer_boundaries() {
	int result = 0;
    Config::File config;

    auto max_int_item = config.Add("MaxInt", StormByte::Config::Item::Type::Integer);
    max_int_item->SetInteger(INT_MAX);

    auto min_int_item = config.Add("MinInt", StormByte::Config::Item::Type::Integer);
    min_int_item->SetInteger(INT_MIN);

	try {
		auto lookup_max_int = config.LookUp("MaxInt");
		ASSERT_EQUAL("test_integer_boundaries", INT_MAX, lookup_max_int->AsInteger());

		auto lookup_min_int = config.LookUp("MinInt");
		ASSERT_EQUAL("test_integer_boundaries", INT_MIN, lookup_min_int->AsInteger());
	}
	catch(...) {
		result = 1;
	}

    RETURN_TEST("test_integer_boundaries", result);
}

int test_special_characters_in_string() {
	int result = 0;
    Config::File config;

    auto str_item = config.Add("SpecialChars", StormByte::Config::Item::Type::String);
    str_item->SetString("Line1\nLine2\tTabbed");

	try {
    	auto lookup_str = config.LookUp("SpecialChars");
    	ASSERT_EQUAL("test_special_characters_in_string", "Line1\nLine2\tTabbed", lookup_str->AsString());
	}
	catch(...) {
		result = 1;
	}

    RETURN_TEST("test_special_characters_in_string", result);
}

int test_deeply_nested_groups() {
	int result = 0;
    Config::File config;

	try {
		auto group1 = config.Add("Group1", StormByte::Config::Item::Type::Group);
		auto group2 = group1->AsGroup().Add("Group2", StormByte::Config::Item::Type::Group);
		auto group3 = group2->AsGroup().Add("Group3", StormByte::Config::Item::Type::Group);
		auto group4 = group3->AsGroup().Add("Group4", StormByte::Config::Item::Type::Group);

		auto int_item = group4->AsGroup().Add("DeepInt", StormByte::Config::Item::Type::Integer);
		int_item->SetInteger(1234);

		auto lookup_int = config.LookUp("Group1/Group2/Group3/Group4/DeepInt");
		ASSERT_EQUAL("test_deeply_nested_groups", 1234, lookup_int->AsInteger());
	}
	catch(...) {
		result = 1;
	}

    RETURN_TEST("test_deeply_nested_groups", result);
}

int test_invalid_syntax() {
	int result = 0;
    Config::File config;
    std::string invalid_config = "Invalid = { Unclosed }";

    try {
        config << invalid_config;
        result = 1;
    } catch (const StormByte::Config::ParseError&) {
        // Expected exception
    }

    RETURN_TEST("test_invalid_syntax", result);
}

int test_special_characters_string() {
	int result = 0;
    Config::File cfg;
    try {
        std::fstream file;
		file.open(CurrentFileDirectory / "files" / "special_characters_conf.conf", std::ios::in);
		cfg << file;
		file.close();
        auto lookup_special = cfg.LookUp("special_string");
        ASSERT_EQUAL("test_special_characters_string", "This is a test string with special characters: \n, \t, \\", lookup_special->AsString());
    } catch (const StormByte::Config::Exception& ex) {
        result = 1;
    }

	RETURN_TEST("test_special_characters_string", result);
}

int test_long_string() {
	int result = 0;
    Config::File cfg;
    try {
        std::fstream file;
		file.open(CurrentFileDirectory / "files" / "long_string_conf.conf", std::ios::in);
		cfg << file;
		file.close();
        auto lookup_long = cfg.LookUp("long_string");
        ASSERT_EQUAL("test_long_string", std::string(1000, 'a'), lookup_long->AsString());
    } catch (...) {
        result = 1;
    }

	RETURN_TEST("test_long_string", result);
}

int test_missing_semicolon() {
	int result = 0;
    Config::File cfg;
    try {
        std::fstream file;
		file.open(CurrentFileDirectory / "files" / "missing_semicolon.conf", std::ios::in);
		cfg << file;
		file.close();
        result = 1;
    } catch (...) {
        // Expected
    }
    
	RETURN_TEST("test_missing_semicolon", result);
}

int test_unmatched_braces() {
	int result = 0;
    Config::File cfg;
    try {
        std::fstream file;
		file.open(CurrentFileDirectory / "files" / "unmatched_braces.conf", std::ios::in);
		cfg << file;
		file.close();
        result = 1;
    } catch (...) {
        // Expected
    }
    
	RETURN_TEST("test_unmatched_braces", result);
}

int good_boolean_config1() {
	int result = 0;
	Config::File cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_boolean_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		auto lookup_enable_feature = cfg.LookUp("settings/enable_feature");
        ASSERT_EQUAL("good_boolean_config1", true, lookup_enable_feature->AsBool());

		auto lookup_enable_extra = cfg.LookUp("settings/enable_extra");
        ASSERT_EQUAL("good_boolean_config1", false, lookup_enable_extra->AsBool());
	}
	catch (...) {
		result = 1;
	}
	
	RETURN_TEST("good_boolean_config1", result);
}

int bad_boolean_config1() {
	int result = 0;
	Config::File cfg;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "bad_boolean_conf1.conf", std::ios::in);
		cfg << file;
		file.close();
		result = 1;
	}
	catch (...) {
		// Expected exception
	}
	
	RETURN_TEST("bad_boolean_config1", result);
}

int copy_configuration() {
	int result = 0;
	Config::File cfg1, cfg2;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_boolean_conf1.conf", std::ios::in);
		cfg1 << file;
		file.close();
		cfg2 = cfg1;
	}
	catch(...) {
		RETURN_TEST("copy_configuration", 1);
	}

	try {
		/* Both should be found */
		auto lookup_enable_feature_1 = cfg1.LookUp("settings/enable_feature");
		auto lookup_enable_feature_2 = cfg2.LookUp("settings/enable_feature");
        ASSERT_EQUAL("good_boolean_config1", lookup_enable_feature_1->AsBool(), lookup_enable_feature_2->AsBool());
	}
	catch(...) {
		result = 1;
	}

	RETURN_TEST("copy_configuration", result);
}

int move_configuration() {
	int result = 0;
	Config::File cfg1, cfg2;
	try {
		std::fstream file;
		file.open(CurrentFileDirectory / "files" / "good_boolean_conf1.conf", std::ios::in);
		cfg1 << file;
		file.close();
		cfg2 = std::move(cfg1);
	}
	catch(...) {
		RETURN_TEST("copy_configuration", 1);
	}

	/* First should fail, second should be found */
	try {
		auto lookup_enable_feature = cfg1.LookUp("settings/enable_feature");
		RETURN_TEST("move_configuration", 1);
	}
	catch(...) {
		// Expected
	}
	try {
		auto lookup_enable_feature = cfg2.LookUp("settings/enable_feature");
        ASSERT_EQUAL("move_configuration", true, lookup_enable_feature->AsBool());
	}
	catch(...) {
		RETURN_TEST("move_and_copy_operators", 1);
	}

	RETURN_TEST("move_and_copy_operators", result);
}

int duplicated_insertion() {
	int result = 0;
	Config::File cfg;
	try {
		cfg.Add("testInt", Config::Item::Type::Integer);
		cfg.Add("testInt", Config::Item::Type::Integer);
		result = 1;
	}
	catch(...) {
		// Expected
	}

	return result;
}

int on_name_clash_ignore() {
	int result = 0;
	Config::File cfg;
	cfg.SetOnNameClashAction([](std::shared_ptr<Config::Item> existing, std::shared_ptr<Config::Item>){ return existing; });
	cfg.Add("testItem", Config::Item::Type::Bool);
	try {
		cfg.Add("testItem", Config::Item::Type::Integer)->SetInteger(66);
		// Should throw because action was set to "ignore" and item is bool
		result = 1;
		auto item = cfg.LookUp("testItem");
		ASSERT_EQUAL("on_name_clash_ignore", 66, item->AsInteger());
	}
	catch(...) {
		// Expected
	}

	RETURN_TEST("on_name_clash_ignore", result);
}

int on_name_clash_replace() {
	int result = 0;
	Config::File cfg;
	cfg.SetOnNameClashAction([](std::shared_ptr<Config::Item>, std::shared_ptr<Config::Item> new_item){ return new_item; });
	cfg.Add("testItem", Config::Item::Type::Bool);
	try {
		cfg.Add("testItem", Config::Item::Type::Integer)->SetInteger(66);
		auto item = cfg.LookUp("testItem");
		ASSERT_EQUAL("on_name_clash_ignore", 66, item->AsInteger());
	}
	catch(...) {
		result = 1;
	}

	RETURN_TEST("on_name_clash_replace", result);
}

int config_to_config_output() {
	int result = 0;
	Config::File cfg1, cfg2;
	cfg1.Add("testInt", Config::Item::Type::Integer)->SetInteger(66);
	cfg2.Add("testString", Config::Item::Type::String)->SetString("Hello!");
	try {
		cfg1 << cfg2;
		auto testString = cfg1.LookUp("testString");
		ASSERT_EQUAL("config_to_config_output", "Hello!", testString->AsString());
	}
	catch(...) {
		result = 1;
	}

	RETURN_TEST("config_to_config_output", result);
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
		result += copy_configuration();
		result += move_configuration();
		result += duplicated_insertion();
		result += on_name_clash_ignore();
		result += on_name_clash_replace();
		result += config_to_config_output();
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
