#pragma once
#include "SDKHeader.h"
#include <random>

class RandomUtil {
private:
	std::random_device RD{};

public:
	GLfloat Gen(int DistType, GLfloat Min, GLfloat Max);
	bool Probability(int Percent);
};
extern RandomUtil randomUtil;