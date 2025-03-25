#include <StormByte/arithmetic_mutexed.hxx>

#include <string>

namespace StormByte {
	template class ArithmeticMutexed<int>;
	template class ArithmeticMutexed<unsigned int>;
	template class ArithmeticMutexed<long>;
	template class ArithmeticMutexed<unsigned long>;
	template class ArithmeticMutexed<long long>;
	template class ArithmeticMutexed<unsigned long long>;
	template class ArithmeticMutexed<float>;
	template class ArithmeticMutexed<double>;
	template class ArithmeticMutexed<long double>;
	template class ArithmeticMutexed<std::string>;
}