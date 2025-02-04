#include "MathUtil.h"
#include "EXUtil.h"
#include <cmath>

MathUtil mathUtil;

void MathUtil::LookAt(GLfloat& DestRotation, GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY, GLfloat RotationSpeed, float FrameTime) {
	GLfloat TargetAngle{}, ShortestAngle{};
	TargetAngle = Computedegree(FromX, FromY, ToX, ToY) - 90.0;
	TargetAngle = NormalizeDegree(TargetAngle);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(DestRotation, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(DestRotation, TargetAngle);

	DestRotation = NormalizeDegree(DestRotation + ShortestAngle);
}

void MathUtil::LookAt(GLfloat& DestRotation, glm::vec2& Position1, glm::vec2& Position2, GLfloat RotationSpeed, float FrameTime) {
	GLfloat TargetAngle{}, ShortestAngle{};
	TargetAngle = Computedegree(Position1, Position2) - 90.0;
	TargetAngle = NormalizeDegree(TargetAngle);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(DestRotation, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(DestRotation, TargetAngle);

	DestRotation = NormalizeDegree(DestRotation + ShortestAngle);
}

void MathUtil::LookAt(GLfloat& RotationDest, GLfloat Rotation, GLfloat RotationSpeed, float FrameTime) {
	GLfloat TargetAngle{}, ShortestAngle{};
	TargetAngle = NormalizeDegree(Rotation);

	if (RotationSpeed > 0)
		ShortestAngle = std::lerp(ShortestAngle, ComputeShortestRotation(RotationDest, TargetAngle), FrameTime * RotationSpeed);
	else
		ShortestAngle = ComputeShortestRotation(RotationDest, TargetAngle);

	RotationDest = NormalizeDegree(RotationDest + ShortestAngle);
}

void MathUtil::Lerp(GLfloat& DestValue, GLfloat Dest, GLfloat Speed, float FrameTime) {
	if (LERP_THRESHOLD > 0.0 && abs(DestValue - Dest) <= LERP_THRESHOLD) {
		DestValue = Dest;
		return;
	}

	GLfloat t = Speed * FrameTime;
	EX.ClampValue(t, 1.0, CLAMP_GREATER);
	DestValue = std::lerp(DestValue, Dest, t);
}

void MathUtil::Lerp(glm::vec2& DestValue, glm::vec2& Dest, GLfloat Speed, float FrameTime) {
	GLfloat t = Speed * FrameTime;
	EX.ClampValue(t, 1.0, CLAMP_GREATER);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.x - Dest.x) <= LERP_THRESHOLD) 
		DestValue.x = Dest.x;
	else 
		DestValue.x = std::lerp(DestValue.x, Dest.x, t);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.y - Dest.y) <= LERP_THRESHOLD)
		DestValue.y = Dest.y;
	else
		DestValue.y = std::lerp(DestValue.y, Dest.y, t);
}

void MathUtil::Lerp(glm::vec3& DestValue, glm::vec3& Dest, GLfloat Speed, float FrameTime) {
	GLfloat t = Speed * FrameTime;
	EX.ClampValue(t, 1.0, CLAMP_GREATER);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.x - Dest.x) <= LERP_THRESHOLD)
		DestValue.x = Dest.x;
	else
		DestValue.x = std::lerp(DestValue.x, Dest.x, t);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.y - Dest.y) <= LERP_THRESHOLD)
		DestValue.y = Dest.y;
	else
		DestValue.y = std::lerp(DestValue.y, Dest.y, t);

	if (LERP_THRESHOLD > 0.0 && abs(DestValue.z - Dest.z) <= LERP_THRESHOLD)
		DestValue.z = Dest.z;
	else
		DestValue.z = std::lerp(DestValue.z, Dest.z, t);
}

GLfloat MathUtil::ComputeDistance(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return  std::sqrt(std::pow(FromX - ToX, 2) + std::pow(FromY - ToY, 2));
}

GLfloat MathUtil::ComputeDistance(glm::vec2& Position1, glm::vec2& Position2) {
	return  std::sqrt(std::pow(Position1.x - Position2.x, 2) + std::pow(Position1.y - Position2.y, 2));
}

glm::vec2 MathUtil::ComputeMidPoint(glm::vec2& Position1, glm::vec2& Position2) {
	return glm::vec2((Position1.x + Position2.x) / 2.0, (Position1.y + Position2.y) / 2.0);
}

GLfloat MathUtil::Computedegree(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return atan2(ToY - FromY, ToX - FromX) * (180.0 / 3.1415);
}
 
GLfloat MathUtil::Computedegree(glm::vec2& Position1, glm::vec2& Position2) {
	return atan2(Position2.y - Position1.y, Position2.x - Position1.x) * (180.0 / 3.1415);
}

GLfloat MathUtil::ComputeRadians(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	return atan2(ToY - FromY, ToX - FromX);
}

GLfloat MathUtil::ComputeRadians(glm::vec2& Position1, glm::vec2& Position2) {
	return atan2(Position2.y - Position1.y, Position2.x - Position1.x);
}

RayVector MathUtil::ComputeRay(GLfloat FromX, GLfloat FromY, GLfloat ToX, GLfloat ToY) {
	XMVECTOR RayOrigin = XMVectorSet(FromX, FromY, 0.0, 0.0);
	float Angle = ComputeRadians(FromX, FromY, ToX, ToY);
	float DirectionX = cos(Angle);
	float DirectionY = sin(Angle);

	XMVECTOR RayDirection = XMVectorSet(DirectionX, DirectionY, 0.0f, 0.0f);
	RayDirection = XMVector2Normalize(RayDirection);
	GLfloat RayLength = ComputeDistance(FromX, FromY, ToX, ToY);
	GLfloat Distance = 0.0;

	return { RayOrigin, RayDirection, Distance, RayLength };
}

RayVector MathUtil::ComputeRay(glm::vec2 From, glm::vec2 To) {
	XMVECTOR RayOrigin = XMVectorSet(From.x, From.y, 0.0, 0.0);
	float Angle = ComputeRadians(From.x, From.y, To.x, To.y);
	float DirectionX = cos(Angle);
	float DirectionY = sin(Angle);

	XMVECTOR RayDirection = XMVectorSet(DirectionX, DirectionY, 0.0f, 0.0f);
	RayDirection = XMVector2Normalize(RayDirection);
	GLfloat RayLength = ComputeDistance(From.x, From.y, To.x, To.y);
	GLfloat Distance = 0.0;

	return { RayOrigin, RayDirection, Distance, RayLength };
}

RayVector MathUtil::ComputeRayWithDegree(GLfloat OriginX, GLfloat OriginY, GLfloat Degree, GLfloat LengthValue) {
	XMVECTOR RayOrigin = XMVectorSet(OriginX, OriginY, 0.0, 0.0);
	float Angle = glm::radians(Degree);
	float DirectionX = cos(Angle);
	float DirectionY = sin(Angle);

	XMVECTOR RayDirection = XMVectorSet(DirectionX, DirectionY, 0.0f, 0.0f);
	RayDirection = XMVector2Normalize(RayDirection);
	GLfloat RayLength = LengthValue;
	GLfloat Distance = 0.0;

	return { RayOrigin, RayDirection, Distance, RayLength };
}

RayVector MathUtil::ComputeRayWithDegree(glm::vec2 Origin, GLfloat Degree, GLfloat LengthValue) {
	XMVECTOR RayOrigin = XMVectorSet(Origin.x, Origin.y, 0.0, 0.0);
	float Angle = glm::radians(Degree);
	float DirectionX = cos(Angle);
	float DirectionY = sin(Angle);

	XMVECTOR RayDirection = XMVectorSet(DirectionX, DirectionY, 0.0f, 0.0f);
	RayDirection = XMVector2Normalize(RayDirection);
	GLfloat RayLength = LengthValue;
	GLfloat Distance = 0.0;

	return { RayOrigin, RayDirection, Distance, RayLength };
}

////////////////////////////////// private

GLfloat MathUtil::NormalizeDegree(GLfloat Degree) {
	while (Degree < 0) Degree += 360;
	while (Degree >= 360) Degree -= 360;
	return Degree;
}

GLfloat MathUtil::ComputeShortestRotation(GLfloat CurrentDegree, GLfloat DegreeDest) {
	GLfloat Diff = DegreeDest - CurrentDegree;

	if (Diff > 180)
		Diff -= 360;
	else if (Diff < -180)
		Diff += 360;

	return Diff;
}