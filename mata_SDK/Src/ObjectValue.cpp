#include "ObjectValue.h"

glm::mat4 MoveMatrix, RotateMatrix, ScaleMatrix, ResultMatrix;
glm::mat4 ImageAspectMatrix, FlipMatrix;

glm::mat4 UnitMoveMatrix, UnitRotateMatrix, UnitScaleMatrix;
glm::mat4 UnitFlipMatrix;

glm::mat4 ViewportPositionMatrix;

glm::vec3 ObjectColor;

GLfloat UnitBlurValue;
GLfloat ObjectBlurValue;

GLfloat ObjectOpacityValue;
GLfloat UnitOpacityValue;


glm::mat4 TextMatrix;


glm::mat4 ShapeMatrix;