#pragma once
#include "SDKHeader.h"
#include "ObjectValue.h"

class Transform {
public:
	void Identity(glm::mat4& Matrix);
	void Move(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Move(glm::mat4& Matrix, glm::vec2& Position);
	void Rotate(glm::mat4& Matrix, GLfloat Degree);
	void RotateRadians(glm::mat4& Matrix, GLfloat Radians);
	void Scale(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Scale(glm::mat4& Matrix, glm::vec2& Size);
	void Shear(glm::mat4& Matrix, GLfloat X, GLfloat Y);
	void Shear(glm::mat4& Matrix, glm::vec2& Value);
	void ImageScale(glm::mat4& Matrix, GLfloat Width, GLfloat Height);
	bool CheckIdentity(glm::mat4& Matrix);
	void RotateV(glm::mat4& Matrix, GLfloat Degree);
	void RotateH(glm::mat4& Matrix, GLfloat Degree);
};
extern Transform transform;