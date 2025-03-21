# StormByte

StormByte is a comprehensive, cross-platform C++ library aimed at easing system programming, configuration management, logging, and database handling tasks. This library provides a unified API that abstracts away the complexities and inconsistencies of different platforms (Windows, Linux).

## Features

- **Configuration Management**: Provides an intuitive API for reading and writing configuration files.
- **Database Handling**: Includes SQLite support for embedded database management while hiding SQLite3 internals conveniently.
- **Logging**: Supports various logging levels and outputs, including file-based logging.
- **Multimedia**: Includes a set of classes to work with multimedia files
- **Network**: All the needed classes to handle network communication portable to Linux and Windows
- **System Operations**: Handles pipes, processes, and system variables seamlessly across different platforms.

## Table of Contents

- [Repository](#Repository)
- [Installation](#Installation)
- [Modules](#Modules)
	- **Base**
	- [Config](https://dev.stormbyte.org/StormByte-Config)
	- [Database](https://dev.stormbyte.org/StormByte-Database)
	- [Logger](https://dev.stormbyte.org/StormByte-Logger)
	- [Multimedia](https://dev.stormbyte.org/StormByte-Multimedia)
	- [Network](https://dev.stormbyte.org/StormByte-Network)
	- [System](https://dev.stormbyte.org/StormByte-System)
- [Contributing](#Contributing)
- [License](#License)

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

StormByte Library is composed by several modules:

### Base

Base component is the most basic part of the library containing only templates, string helpers and base exception framework

### Serializable

The `Serializable` template class provides a way to serialize and deserialize data. It works with trivially copyable types, standard library types, containers, and can be specialized to include custom objects with the `SerializeComplex` and `DeserializeComplex` functions.

#### Example Usage with Trivially Copyable Type

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

#### Example Usage with Standard Library Type

```cpp
#include <StormByte/util/serializable.hxx>
#include <iostream>
#include <string>

using namespace StormByte::Util;

int main() {
	std::string myString = "Hello, StormByte!";
	Serializable<std::string> serializable(myString);

	// Serialize
	Buffer buffer = serializable.Serialize();
	std::cout << "Serialized data: " << buffer.HexData() << std::endl;

	// Deserialize
	auto deserialized = Serializable<std::string>::Deserialize(buffer);
	if (deserialized) {
		std::string deserializedString = deserialized.value();
		std::cout << "Deserialized data: " << deserializedString << std::endl;
	} else {
		std::cerr << "Deserialization failed: " << deserialized.error().what() << std::endl;
	}

	return 0;
}
```

#### Example Usage with Container

```cpp
#include <StormByte/util/serializable.hxx>
#include <iostream>
#include <vector>
#include <string>

using namespace StormByte::Util;

int main() {
	std::vector<std::string> myVector = {"Hello", "StormByte", "Library"};
	Serializable<std::vector<std::string>> serializable(myVector);

	// Serialize
	Buffer buffer = serializable.Serialize();
	std::cout << "Serialized data: " << buffer.HexData() << std::endl;

	// Deserialize
	auto deserialized = Serializable<std::vector<std::string>>::Deserialize(buffer);
	if (deserialized) {
		std::vector<std::string> deserializedVector = deserialized.value();
		std::cout << "Deserialized data: ";
		for (const auto& str : deserializedVector) {
			std::cout << str << " ";
		}
		std::cout << std::endl;
	} else {
		std::cerr << "Deserialization failed: " << deserialized.error().what() << std::endl;
	}

	return 0;
}
```

### Buffer

The `Buffer` class provides a powerful and memory-safe way to store and manipulate byte buffers. It supports various operations such as appending data, reading data, and converting data to a hexadecimal string representation.

#### Example Usage

```cpp
#include <StormByte/util/buffer.hxx>
#include <iostream>

using namespace StormByte::Util;

int main() {
	// Create a buffer from a string
	Buffer buffer("Hello, World!", 13);

	// Append more data to the buffer
	buffer << " More data";

	// Print the buffer contents as a hexadecimal string
	std::cout << "Buffer contents (hex):" << std::endl;
	std::cout << buffer.HexData() << std::endl;

	// Read data from the buffer
	auto readData = buffer.Read(5);
	if (readData) {
		std::string readString(reinterpret_cast<const char*>(readData.value().data()), readData.value().size());
		std::cout << "Read data: " << readString << std::endl;
	} else {
		std::cerr << "Read failed: " << readData.error().what() << std::endl;
	}

	return 0;
}
```

## Contributing

Contributions are welcome! Please fork the repository and submit pull requests for any enhancements or bug fixes.

## License

This project is licensed under GPL v3 License - see the [LICENSE](LICENSE) file for details.