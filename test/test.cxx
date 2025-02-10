#include <StormByte/system/process.hxx>

using namespace StormByte::System;

int main() {
	Process echo("/bin/echo", {"hello world"});
	Process tr("/usr/bin/tr", {"a-z", "A-Z"});
	Process wc("/usr/bin/wc", {"-c"});

	echo >> tr >> wc;

	wc.wait();

	std::string result;
	wc >> result;

	std::cout << "Result is: " << result << std::endl;
	return 0;
}