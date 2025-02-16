#include "ObjectValue.h"

glm::mat4 ImageAspectMatrix = glm::mat4(1.0f);
glm::mat4 ResultMatrix = glm::mat4(1.0f);

glm::mat4 MoveMatrix = glm::mat4(1.0f), RotateMatrix = glm::mat4(1.0f), ScaleMatrix = glm::mat4(1.0f), FlipMatrix = glm::mat4(1.0f);

glm::mat4 UnitMoveMatrix = glm::mat4(1.0f), UnitRotateMatrix = glm::mat4(1.0f), UnitScaleMatrix = glm::mat4(1.0f);
glm::mat4 UnitFlipMatrix = glm::mat4(1.0f);

glm::mat4 ViewportPositionMatrix = glm::mat4(1.0f);

glm::vec3 ObjectColor;

GLfloat UnitBlurValue;
GLfloat ObjectBlurValue;

GLfloat ObjectOpacityValue;
GLfloat UnitOpacityValue;


glm::mat4 TextMatrix = glm::mat4(1.0f);


glm::mat4 ShapeMatrix = glm::mat4(1.0f);