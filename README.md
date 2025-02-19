# StormByte

StormByte is a comprehensive, cross-platform C++ library aimed at easing system programming, configuration management, logging, and database handling tasks. This library provides a unified API that abstracts away the complexities and inconsistencies of different platforms (Windows, Linux).

## Features

- **System Operations**: Handles pipes, processes, and system variables seamlessly across different platforms.
- **Configuration Management**: Provides an intuitive API for reading and writing configuration files.
- **Logging**: Supports various logging levels and outputs, including file-based logging.
- **Database Handling**: Includes SQLite support for embedded database management while hiding SQLite3 internals conveniently.

## Table of Contents

- [Installation](#Installation)
- [Modules](#Modules)
	- [System](#System)
	- [Config](#Configuration)
	- [Log](#Log)
	- [Database](#Database)
- [Contributing](#Contributing)
- [License](#License)

## Installation

### Prerequisites

Ensure you have the following installed:

- C++20 compatible compiler
- CMake 3.12 or higher
- SQLite3 (for database support)

### Building

To build the library, follow these steps:

```sh
git clone https://github.com/StormBytePP/StormByte.git
cd StormByte
mkdir build
cd build
cmake ..
make
```

## Modules

### System

The `System` module provides a set of classes and functions to handle system-level operations like pipes, processes, and system variables.

#### Example: Process

Include the necessary headers in your project and link against the `StormByte` library:

```cpp
#include <StormByte/system/process.hxx>
#include <iostream>

using namespace StormByte::System;

// Example usage
int main() {
	std::vector<std::string> args = {"-l", "-a"};
	Process ls("/bin/ls", args);
	Process grep("/bin/grep", {"main.cpp"});
	ls >> grep;
	grep.wait();
	std::string output;
	grep >> output;
	std::cout << output << std::endl;
	return 0;
}
```

#### Example: Variable

```cpp
#include <StormByte/system/variable.hxx>
#include <iostream>

// Example usage
int main() {
	std::string path = StormByte::System::Variable::Expand("~");
	std::cout << "Home path: " << path << std::endl;
	return 0;
}
```

### Configuration

#### Overview

The StormByte configuration format is designed to be flexible and human-readable. It supports various data types, including integers, floating-point numbers, strings, booleans, groups (nested configurations), and lists. The format also allows for comments to enhance readability and documentation.

#### Basic Structure

Configuration files consist of key-value pairs. Keys are strings that follow a specific naming convention, while values can be of different data types.

##### Example

```plaintext
username = "example_user"
timeout = 30
enable_feature = true
```

#### Naming Convention

- Keys and group names must consist of alphanumeric characters, underscores (`_`), and hyphens (`-`).
- Strings must be enclosed in double quotes (`"`).
- Special characters like newline (`\n`), tab (`\t`), and backslash (`\\`) are supported within strings.
- Boolean values are represented as `true` or `false`.

#### Groups

Groups are nested configurations that can contain other key-value pairs, groups, or lists. Groups are denoted by curly braces `{}`.

##### Example

```plaintext
settings = {
    username = "example_user"
    timeout = 30
    enable_feature = true
}
```

#### Lists

Lists are sequences of values enclosed in square brackets `[]`. Lists can contain comments, values, and even nested groups.

##### Example

```plaintext
favorite_numbers = [
    3
    14
    42
]
```

#### Comments

Comments can be added using the `#` symbol. Comments can appear on their own line or after a value.

##### Example

```plaintext
# This is a comment
username = "example_user"  # This is an inline comment
```

#### Advanced Examples

##### Group with Nested List and Comments

```plaintext
settings = {
    username = "example_user"  # This is a comment
    timeout = 30
    enable_feature = true
    favorite_numbers = [  # List of favorite numbers
        3
        14
        42
    ]
}
```

##### Complex Nested Structure

```plaintext
Group1 = {
    Group2 = {
        SubTestInt = 99
        SubTestStr = "Sub Hello"
    }
}
```

##### Special Characters in Strings

```plaintext
special_string = "This is a test string with special characters: \n, \t, \\"
```

##### Floating Point Precision

```plaintext
float_precise = 1.123456789012345
```

##### Large Numbers

```plaintext
large_number = 9223372036854775807  # INT64_MAX
small_number = -9223372036854775807  # INT64_MIN
```

#### Real Usage Example

Consider a real usage example where a main file reads a configuration file (`config.cfg`) and parses its values.

##### Configuration File (`config.cfg`)

```plaintext
settings = {
    username = "example_user"
    timeout = 30
    enable_feature = true
}

database = {
    host = "localhost"
    port = 5432
    user = "db_user"
    password = "secret"
}

log_levels = [
    "info"
    "debug"
    "error"
]

features = {
    enable_new_feature = true
    feature_timeout = 60.5
}
```

##### Main File (`main.cpp`)

```cpp
#include <StormByte/config/config.hxx>
#include <StormByte/system/system.hxx>
#include <iostream>
#include <fstream>

using namespace StormByte::Config;

int main() {
    Config config;

    // Read configuration from file
    std::ifstream config_file("config.cfg");
    if (!config_file.is_open()) {
        std::cerr << "Failed to open config file." << std::endl;
        return 1;
    }

    config_file >> config;
    config_file.close();

    // Access configuration values
    try {
        const Item& username = config["settings/username"];
        const Item& timeout = config["settings/timeout"];
        const Item& enable_feature = config["settings/enable_feature"];
        const Item& db_host = config["database/host"];
        const Item& db_port = config["database/port"];
        const Item& db_user = config["database/user"];
        const Item& db_password = config["database/password"];
        const List& log_levels = config["log_levels"].Value<List>();
        const Item& enable_new_feature = config["features/enable_new_feature"];
        const Item& feature_timeout = config["features/feature_timeout"];

        // Print configuration values
        std::cout << "Username: " << username.Value<std::string>() << std::endl;
        std::cout << "Timeout: " << timeout.Value<int>() << std::endl;
        std::cout << "Enable Feature: " << enable_feature.Value<bool>() << std::endl;
        std::cout << "Database Host: " << db_host.Value<std::string>() << std::endl;
        std::cout << "Database Port: " << db_port.Value<int>() << std::endl;
        std::cout << "Database User: " << db_user.Value<std::string>() << std::endl;
        std::cout << "Database Password: " << db_password.Value<std::string>() << std::endl;
        std::cout << "Log Levels: ";
        for (const auto& level : log_levels) {
            std::cout << level.Value<std::string>() << " ";
        }
        std::cout << std::endl;
        std::cout << "Enable New Feature: " << enable_new_feature.Value<bool>() << std::endl;
        std::cout << "Feature Timeout: " << feature_timeout.Value<double>() << std::endl;

    } catch (const StormByte::Config::Exception& e) {
        std::cerr << "Error accessing configuration: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
```

#### Summary

The StormByte configuration format is designed to be flexible and easy to use. It supports various data types, including nested structures, lists, and comments. By following the provided examples and guidelines, you can create comprehensive and readable configuration files for your StormByte projects. The real usage example demonstrates how to read and parse a configuration file in a main program, providing a practical application of the configuration format.

### Log

The `Log` module provides a comprehensive logging framework with support for different logging levels and outputs.

#### Example: Log

```cpp
#include <StormByte/log/logger.hxx>
#include <iostream>

using namespace StormByte::Log;

// Example usage
int main() {
	// Simple logger outputing only errors to stdout
	Logger logger(std::cout, StormByte::Log::Level::Error);
	logger << Level::Info << "This is an info message"; // Will not be displayed
	logger << Level::Error << "This is an error message"; // Will be displayed
	return 0;
}
```

### Database

The `Database` module provides support for SQLite, an embedded SQL database engine. It includes classes for managing database connections, prepared statements, and result rows.

#### Example: Database

```cpp
#include <StormByte/database/sqlite/sqlite3.hxx>
#include <memory>
#include <iostream>

class MyDatabase : public StormByte::Database::SQLite::SQLite3 {
public:
	MyDatabase(const std::filesystem::path& dbfile) : SQLite3(dbfile) {
		init_database();
	}

	void print_all_users() {
		auto stmt = prepare_select_all_users();
		while (auto row = stmt->Step()) {
			std::cout << "ID: " << row->At(0)->Value<int>() << " Name: " << row->At(1)->Value<std::string>() << std::endl;
		}
	}

protected:
	void post_init_action() noexcept override {
		try {
			silent_query("CREATE TABLE IF NOT EXISTS users (id INTEGER PRIMARY KEY, name TEXT NOT NULL)");
		} catch (const StormByte::Database::SQLite::Exception& e) {
			std::cerr << "Database initialization error: " << e.what() << std::endl;
		}
	}

	std::shared_ptr<StormByte::Database::SQLite::PreparedSTMT> prepare_select_all_users() {
		return prepare_sentence("select_all_users", "SELECT * FROM users");
	}
};

// Example usage
int main() {
	MyDatabase db("/path/to/database.db");
	db.print_all_users();
	return 0;
}
```

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under GPL v3 License - see the [LICENSE](LICENSE) file for details.
