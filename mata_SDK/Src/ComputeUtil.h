#pragma once
#include "SDKHeader.h"

class ComputeUtil {
public:
	void ComputeMatrix(glm::mat4& Dest, glm::mat4& Matrix1, glm::mat4& Matrix2 = glm::mat4(1.0f), glm::mat4& Matrix3 = glm::mat4(1.0f), glm::mat4& Matrix4 = glm::mat4(1.0f), glm::mat4& Matrix5 = glm::mat4(1.0f));
};
extern ComputeUtil computeUtil;