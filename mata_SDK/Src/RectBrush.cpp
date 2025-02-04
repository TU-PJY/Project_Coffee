#include "RectBrush.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include "TransformUtil.h"
#include "SystemResource.h"
#include "ComputeUtil.h"
#include <cmath>

LineRectBrush::LineRectBrush(bool Flag) {
	CamInheritanceCommand = Flag;
}

void LineRectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void LineRectBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void LineRectBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void LineRectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void LineRectBrush::Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat Width, GLfloat RotationValue, GLfloat OpacityValue) {
	Opacity = OpacityValue;
	DrawLine(X, Y, 0.0, SizeY / 2.0, SizeX + Width, Width, RotationValue);
	DrawLine(X, Y, 0.0, -SizeY / 2.0, SizeX + Width, Width, RotationValue);
	DrawLine(X, Y, -SizeX / 2.0, 0.0, Width, SizeY + Width, RotationValue);
	DrawLine(X, Y, SizeX / 2.0, 0.0, Width, SizeY + Width, RotationValue);
}

void LineRectBrush::DrawLine(GLfloat X, GLfloat Y, GLfloat OffsetX, GLfloat OffsetY, GLfloat Width, GLfloat Height, GLfloat RotationValue) {
	transform.Identity(ShapeMatrix);

	transform.Move(ShapeMatrix, X, Y);
	transform.Rotate(ShapeMatrix, RotationValue);
	transform.Move(ShapeMatrix, OffsetX, OffsetY);
	transform.Scale(ShapeMatrix, Width, Height);
	Render();
}

void LineRectBrush::Render() {
	if (!CamInheritanceCommand)
		camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	imageUtil.RenderRaw();
}



RectBrush::RectBrush(bool Flag) {
	CamInheritanceCommand = Flag;
}

void RectBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void RectBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void RectBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void RectBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void RectBrush::Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat RotationValue, GLfloat OpacityValue) {
	transform.Identity(ShapeMatrix);
	Opacity = OpacityValue;

	transform.Move(ShapeMatrix, X, Y);
	transform.Rotate(ShapeMatrix, RotationValue);
	transform.Scale(ShapeMatrix, SizeX, SizeY);

	Render();
}

void RectBrush::Render() {
	if(!CamInheritanceCommand)
		camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	imageUtil.RenderRaw();
}
