#include "ComputeUtil.h"

ComputeUtil computeUtil;

void ComputeUtil::ComputeMatrix(glm::mat4& Dest, glm::mat4& Matrix1, glm::mat4& Matrix2, glm::mat4& Matrix3, glm::mat4& Matrix4, glm::mat4& Matrix5) {
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_MATRIX_INPUT);
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Matrix1));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(Matrix2));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * 2, sizeof(glm::mat4), glm::value_ptr(Matrix3));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * 3, sizeof(glm::mat4), glm::value_ptr(Matrix4));
	glBufferSubData(GL_SHADER_STORAGE_BUFFER, sizeof(glm::mat4) * 4, sizeof(glm::mat4), glm::value_ptr(Matrix5));

	glUseProgram(MATRIX_COMPT_SHADER);
	glDispatchCompute(1, 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, SSBO_MATRIX_OUTPUT);
	void* Result = glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, sizeof(glm::mat4), GL_MAP_READ_BIT);
	memcpy(glm::value_ptr(Dest), Result, sizeof(glm::mat4));
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}