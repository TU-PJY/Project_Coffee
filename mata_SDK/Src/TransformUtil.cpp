#include "TransformUtil.h"

Transform transform;

void Transform::Identity(glm::mat4& Matrix) {
	Matrix = glm::mat4(1.0f);
}

void Transform::Move(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = translate(Matrix, glm::vec3(X, Y, 0.0));
}

void Transform::Move(glm::mat4& Matrix, glm::vec2& Position) {
	Matrix = translate(Matrix, glm::vec3(Position.x, Position.y, 0.0));
}

void Transform::Rotate(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(-Degree), glm::vec3(0.0, 0.0, 1.0));
}

void Transform::RotateRadians(glm::mat4& Matrix, GLfloat Radians) {
	Matrix = rotate(Matrix, -Radians, glm::vec3(0.0, 0.0, 1.0));
}

void Transform::RotateV(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(1.0, 0.0, 0.0));
}

void Transform::RotateH(glm::mat4& Matrix, GLfloat Degree) {
	Matrix = rotate(Matrix, glm::radians(Degree), glm::vec3(0.0, 1.0, 0.0));
}

void Transform::Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix = scale(Matrix, glm::vec3(X, Y, 1.0));
}

void Transform::Scale(glm::mat4& Matrix, glm::vec2& Size) {
	Matrix = scale(Matrix, glm::vec3(Size.x, Size.y, 1.0));
}

void Transform::Shear(glm::mat4& Matrix, GLfloat X, GLfloat Y) {
	Matrix[1][0] += X;
	Matrix[0][1] += Y;
}

void Transform::Shear(glm::mat4& Matrix, glm::vec2& Value) {
	Matrix[1][0] += Value.x;
	Matrix[0][1] += Value.y;
}

void Transform::ImageScale(glm::mat4& Matrix, GLfloat Width, GLfloat Height) {
	if (Width > Height)
		Matrix = glm::scale(Matrix, glm::vec3(1.0, Height / Width, 1.0));
	else if (Width < Height)
		Matrix = glm::scale(Matrix, glm::vec3(Width / Height, 1.0, 1.0));
}

bool Transform::CheckIdentity(glm::mat4& Matrix) {
	if (std::memcmp(&Matrix, &IdentityMatrix, sizeof(glm::mat4)) != 0)
		return false;
	
	return true;
}