#pragma once
#include "SDKHeader.h"

class MathUtil {
public:
	void LookAt(GLfloat& DestRotation, GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat RotationSpeed = 0.0, float FrameTime = 0.0);
	void LookAt(GLfloat& DestRotation, glm::vec2& Position1, glm::vec2& Position2, GLfloat Rotationpeed = 0.0, float FrameTime = 0.0);
	void LookAt(GLfloat& DestRotation, GLfloat Rotation, GLfloat RotationSpeed = 0.0, float FrameTime = 0.0);
	GLfloat ComputeDistance(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat ComputeDistance(glm::vec2& Position1, glm::vec2& Position2);
	glm::vec2 ComputeMidPoint(glm::vec2& Position1, glm::vec2& Position2);
	void Lerp(GLfloat& DestValue, GLfloat Dest, GLfloat Speed, float FrameTime);
	void Lerp(glm::vec2& DestValue, glm::vec2& Dest, GLfloat Speed, float FrameTime);
	void Lerp(glm::vec3& DestValue, glm::vec3& Dest, GLfloat Speed, float FrameTime);
	GLfloat Computedegree(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat Computedegree(glm::vec2& Position1, glm::vec2& Position2);
	GLfloat ComputeRadians(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	GLfloat ComputeRadians(glm::vec2& Position1, glm::vec2& Position2);
	RayVector ComputeRay(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY);
	RayVector ComputeRay(glm::vec2 From, glm::vec2 To);
	RayVector ComputeRayWithDegree(GLfloat OriginX, GLfloat OriginY, GLfloat Degree, GLfloat LengthValue = 0.0);
	RayVector ComputeRayWithDegree(glm::vec2 Origin, GLfloat Degree, GLfloat LengthValue = 0.0);
private:
	GLfloat NormalizeDegree(GLfloat Degree);
	GLfloat ComputeShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest);
};
extern MathUtil mathUtil;