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
  - [Config](#Config)
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

// Example usage
int main() {
    std::vector<std::string> args = {"-l", "-a"};
    StormByte::System::Process ls("/bin/ls", args);
    StormByte::System::Process grep("/bin/grep", {"main.cpp"});
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

// Example usage
int main() {
    std::string path = StormByte::System::Variable::Expand("~");
    std::cout << "Home path: " << path << std::endl;
    return 0;
}
```

### Config

The `Config` module provides a flexible and easy-to-use API for configuration management. It supports reading and writing configuration files with various types including groups, strings, integers, and doubles.

#### Example: Config

```cpp
#include <StormByte/config/file.hxx>

// Example usage
int main() {
    StormByte::Config::File config("/path/to/config.file");
    config.Read();
    if (config.Exists("settings/username")) {
        std::cout << "Username: " << config.LookUp("settings/username")->AsString() << std::endl;
    }
    return 0;
}
```

Example `example.cfg`:

```plaintext
# Configuration file

settings = {
    username = "example_user";
    timeout = 30;
    enable_feature = true;
};
```

### Log

The `Log` module provides a comprehensive logging framework with support for different logging levels and outputs.

#### Example: Log

```cpp
#include <StormByte/log/file.hxx>

// Example usage
int main() {
    StormByte::Log::File logger(StormByte::Log::Level::Debug, "logfile.txt");
    logger << StormByte::Log::Level::Info << "This is an info message" << StormByte::Log::endl;
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
