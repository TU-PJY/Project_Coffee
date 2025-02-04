#pragma once
#include "SDKHeader.h"

extern glm::mat4 MoveMatrix, RotateMatrix, ScaleMatrix, ResultMatrix;
extern glm::mat4 ImageAspectMatrix, FlipMatrix;

extern glm::mat4 UnitMoveMatrix, UnitRotateMatrix, UnitScaleMatrix;
extern glm::mat4 UnitFlipMatrix;

extern glm::mat4 ViewportPositionMatrix;

extern glm::vec3 ObjectColor;

extern GLfloat UnitBlurValue;
extern GLfloat ObjectBlurValue;

extern GLfloat ObjectOpacityValue;
extern GLfloat UnitOpacityValue;


extern glm::mat4 TextMatrix;


extern glm::mat4 ShapeMatrix;