#pragma once
#include <iostream>
#include <random>
template<typename Type>
class Randomer {
	// random seed by default
	std::mt19937 gen_;
	std::uniform_real_distribution<Type> dist_;  // uniform_int_distribution<float/double> for pointed number

public:
	/*  ... some convenient ctors ... */

	Randomer(Type min, Type max, Type seed = std::random_device{}())
		: gen_{ seed }, dist_{ min, max } {
	}

	// if you want predictable numbers
	void SetSeed(Type seed) {
		gen_.seed(seed);
	}

	Type operator()() {
		return dist_(gen_);
	}
};