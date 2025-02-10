#include "RandomUtil.h"
#include <sstream>

RandomUtil randomUtil;

GLfloat RandomUtil::Gen(int DistType, GLfloat Min, GLfloat Max) {
	if (DistType == RANDOM_TYPE_REAL) {
		std::uniform_real_distribution URD{ Min, Max };
		return URD(RD);
	}

	else if (DistType == RANDOM_TYPE_INT) {
		std::uniform_int_distribution UID{ (int)Min, (int)Max };
		return UID(RD);
	}

	return 0.0;
}

bool RandomUtil::Probability(int Percent) {
	int RandNum = Gen(RANDOM_TYPE_INT, 1, 100);
	if (1 <= RandNum && RandNum <= Percent)
		return true;
	else
		return false;
}