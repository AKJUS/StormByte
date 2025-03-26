# StormByte

StormByte is a comprehensive, cross-platform C++ library aimed at easing system programming, configuration management, logging, and database handling tasks. This library provides a unified API that abstracts away the complexities and inconsistencies of different platforms (Windows, Linux).

## Features

- **Configuration Management**: Provides an intuitive API for reading and writing configuration files.
- **Database Handling**: Includes SQLite support for embedded database management while hiding SQLite3 internals conveniently.
- **Logging**: Offers various logging levels, customizable formats, and supports outputs to files, streams, or other destinations.
- **Multimedia**: Includes a set of classes to work with multimedia files.
- **Network**: Contains everything needed to handle network communication portable to Linux and Windows.
- **System Operations**: Manages pipes, processes, and system variables seamlessly across different platforms.
- **Buffers**: Provides a variety of buffer types for managing byte data in both single-threaded and multi-threaded environments. This includes lightweight non-thread-safe buffers, thread-safe shared buffers, and robust producer/consumer models that track buffer status.

## Table of Contents

- [Repository](#repository)
- [Installation](#installation)
- [Modules](#modules)
  - **Base**
  - [Config](https://dev.stormbyte.org/StormByte-Config)
  - [Database](https://dev.stormbyte.org/StormByte-Database)
  - [Multimedia](https://dev.stormbyte.org/StormByte-Multimedia)
  - [Network](https://dev.stormbyte.org/StormByte-Network)
  - [System](https://dev.stormbyte.org/StormByte-System)
- [Contributing](#contributing)
- [License](#license)

## Repository

You can visit the code repository at [GitHub](https://github.com/StormBytePP/StormByte)

## Installation

### Prerequisites

Ensure you have the following installed:

- C++23 compatible compiler
- CMake 3.12 or higher

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

StormByte Library is composed of several modules:

### Base

The Base component is the core of the library containing only templates, string helpers, and the base exception framework.

### Logger

#### Overview

The `Logger` module provides robust logging functionalities with support for multiple log levels, customizable formats, and output redirection to various targets. Whether you need basic console logging or advanced logging to files, the `Logger` module is designed for flexibility and ease of use.

#### Features
- **Log Levels**: Includes levels such as `Info`, `Debug`, `Warning`, `Error`, and `Fatal`.
- **Custom Formats**: Allows customization of log headers with placeholders like `%T` for timestamps and `%L` for log levels.
- **Output Targets**: Supports logging to `std::ostream` or any custom output stream.
- **Thread Safety**: Ensures logs are safely written even in multithreaded applications.

#### Example Usage

##### Basic Logging
```cpp
#include <StormByte/logger/log.hxx>
#include <iostream>

using namespace StormByte::Logger;

int main() {
    Log logger(std::cout, Level::Info, "%T %L: ");
    logger << Level::Info << "This is an info message." << std::endl;
    logger << Level::Error << "An error occurred." << std::endl;
    return 0;
}
```

##### Redirecting Logs to a File
```cpp
#include <StormByte/logger/log.hxx>
#include <fstream>

using namespace StormByte::Logger;

int main() {
    std::ofstream file("log.txt");
    Log logger(file, Level::Debug, "%L %T: ");
    logger << Level::Debug << "Debugging information." << std::endl;
    logger << Level::Fatal << "Critical error!" << std::endl;
    return 0;
}
```

### Buffers

The Buffers module provides a robust set of classes for managing byte data. With support for both thread-safe and non-thread-safe buffers—as well as for producer/consumer models—you can choose the right buffer to suit your use case.

#### Simple Buffer

The `Simple` buffer is designed for performance-critical, single-threaded scenarios. It offers lightweight operations for appending, reading, and seeking within the buffer.

**Example:**
```cpp
#include <StormByte/buffers/simple.hxx>
#include <iostream>

int main() {
    // Create a simple buffer with initial data.
    StormByte::Buffers::Simple simpleBuffer("Initial data");
    simpleBuffer << " appended text";
    
    // Retrieve the buffer content.
    auto data = simpleBuffer.Data();
    std::string output(data.begin(), data.end());
    std::cout << "Simple Buffer contents: " << output << std::endl;
    return 0;
}
```

#### Shared Buffer

The `Shared` buffer extends `Simple` by adding thread safety through a `std::shared_mutex`. It allows concurrent reads and exclusive writes, making it ideal for multi-threaded contexts.

**Example:**
```cpp
#include <StormByte/buffers/shared.hxx>
#include <iostream>
#include <thread>

void writeData(StormByte::Buffers::Shared& sharedBuffer) {
    sharedBuffer << " Thread data";
}

int main() {
    StormByte::Buffers::Shared sharedBuffer("Start");
    
    // Launch two threads that write data concurrently.
    std::thread t1(writeData, std::ref(sharedBuffer));
    std::thread t2(writeData, std::ref(sharedBuffer));
    
    t1.join();
    t2.join();
    
    // Display the hexadecimal representation of the shared buffer.
    std::cout << "Shared Buffer Hex: " << sharedBuffer.HexData() << std::endl;
    return 0;
}
```

#### Async Buffer

The `Async` class provides the foundation for a multi-threaded producer/consumer model. It safely handles concurrent reads and writes by offering helper methods to obtain `Producer` and `Consumer` instances.

**Example:**
```cpp
#include <StormByte/buffers/async.hxx>
#include <StormByte/buffers/producer.hxx>
#include <StormByte/buffers/consumer.hxx>
#include <iostream>
#include <memory>
#include <thread>

int main() {
    // Create a shared Async buffer and obtain Producer and Consumer instances.
    auto asyncBuffer = std::make_shared<StormByte::Buffers::Async>();
    auto producer = asyncBuffer->Producer();
    auto consumer = asyncBuffer->Consumer();
    
    // Producer thread: Append some data and signal the End of File.
    std::thread prod([&]() {
        *producer << "Async data: Hello from producer.";
        *producer << StormByte::Buffers::Status::EoF;
    });
    
    // Consumer thread: Extract data from the buffer.
    std::thread cons([&]() {
        auto data = consumer->Extract(25);
        if (data) {
            std::string output(data.value().begin(), data.value().end());
            std::cout << "Async Buffer extracted: " << output << std::endl;
        }
    });
    
    prod.join();
    cons.join();
    
    return 0;
}
```

#### Producer Buffer

The `Producer` is a write-only buffer designed for use in producer/consumer models. It supports appending data using overloaded `operator<<` calls, seamlessly integrating various data sources.

**Example:**
```cpp
#include <StormByte/buffers/producer.hxx>
#include <iostream>

int main() {
    // Create an Async buffer and initialize a Producer instance with it.
    StormByte::Buffers::Async asyncBuffer;
    StormByte::Buffers::Producer producer(asyncBuffer);
    
    // Append data to the buffer.
    producer << "Producer writes data.";
    producer << StormByte::Buffers::Status::EoF;
    
    std::cout << "Data appended by Producer." << std::endl;
    return 0;
}
```

#### Consumer Buffer

The `Consumer` buffer supports destructive read operations in a thread-safe manner—once data is extracted it is removed from the buffer. This is ideal for scenarios where data should be processed only once.

**Example:**
```cpp
#include <StormByte/buffers/consumer.hxx>
#include <iostream>

int main() {
    // Create an Async buffer and initialize a Consumer instance with it.
    StormByte::Buffers::Async asyncBuffer;
    StormByte::Buffers::Consumer consumer(asyncBuffer);
    
    // In a real-world scenario, a Producer would be adding data.
    // Here we simulate data already being available.
    auto result = consumer.Extract(10);
    if (result) {
        std::string readString(result.value().begin(), result.value().end());
        std::cout << "Consumer extracted data: " << readString << std::endl;
    } else {
        std::cerr << "Extraction error." << std::endl;
    }
    
    return 0;
}
```

### Serializable

The `Serializable` template class provides a way to serialize and deserialize data. It works with trivially copyable types, standard library types, containers, and can be specialized for custom objects.

#### Example Usage with a Trivially Copyable Type

```cpp
#include <StormByte/util/serializable.hxx>
#include <iostream>
#include <vector>

using namespace StormByte::Util;

struct MyStruct {
    int a;
    float b;
};

template<>
class Serializable<MyStruct> {
public:
    Serializable(const MyStruct& data) : m_data(data) {}

    Buffer Serialize() const noexcept {
        Buffer buffer;
        buffer << Serializable<int>(m_data.a).Serialize();
        buffer << Serializable<float>(m_data.b).Serialize();
        return buffer;
    }

    static StormByte::Expected<MyStruct, BufferException> Deserialize(const Buffer& buffer) noexcept {
        auto a = Serializable<int>::Deserialize(buffer);
        if (!a) return StormByte::Unexpected<BufferException>(a.error());

        auto b = Serializable<float>::Deserialize(buffer);
        if (!b) return StormByte::Unexpected<BufferException>(b.error());

        return MyStruct{a.value(), b.value()};
    }

private:
    const MyStruct& m_data;
};

int main() {
    MyStruct myStruct = {42, 3.14f};
    Serializable<MyStruct> serializable(myStruct);

    // Serialize
    Buffer buffer = serializable.Serialize();
    std::cout << "Serialized data: " << buffer.HexData() << std::endl;

    // Deserialize
    auto deserialized = Serializable<MyStruct>::Deserialize(buffer);
    if (deserialized) {
        MyStruct deserializedStruct = deserialized.value();
        std::cout << "Deserialized data: a = " << deserializedStruct.a << ", b = " << deserializedStruct.b << std::endl;
    } else {
        std::cerr << "Deserialization failed: " << deserialized.error().what() << std::endl;
    }

    return 0;
}
```

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under GPL v3 License - see the [LICENSE](LICENSE) file for details.
```
