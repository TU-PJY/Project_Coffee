#include "LineBrush.h"
#include "TransformUtil.h"
#include "MathUtil.h"
#include "ImageUtil.h"
#include "ShaderUtil.h"
#include "CameraUtil.h"
#include "SystemResource.h"
#include "ComputeUtil.h"

void LineBrush::SetRenderType(int Opt) {
	RenderType = Opt;
}

void LineBrush::SetColor(GLfloat R, GLfloat G, GLfloat B) {
	Color.r = R;
	Color.g = G;
	Color.b = B;
}

void LineBrush::SetColor(glm::vec3& ColorValue) {
	Color.r = ColorValue.r;
	Color.g = ColorValue.g;
	Color.b = ColorValue.b;
}

void LineBrush::SetColorRGB(int R, int G, int B) {
	Color.r = (1.0f / 255.0f) * (GLfloat)R;
	Color.g = (1.0f / 255.0f) * (GLfloat)G;
	Color.b = (1.0f / 255.0f) * (GLfloat)B;
}

void LineBrush::SetLineType(int LineTypeOpt) {
	if (LineTypeOpt > LINE_TYPE_ROUND)
		return;

	LineType = LineTypeOpt;
}

void LineBrush::Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat OpacityValue) {
	transform.Identity(ShapeMatrix);
	Opacity = OpacityValue;

	Length = mathUtil.ComputeDistance(X1, Y1, X2, Y2);
	Rotation = mathUtil.ComputeRadians(X1, Y1, X2, Y2);

	transform.Move(ShapeMatrix, X1, Y1);
	transform.RotateRadians(ShapeMatrix, Rotation);
	transform.Move(ShapeMatrix, Length / 2.0, 0.0);

	if (LineType == LINE_TYPE_RECT)
		transform.Scale(ShapeMatrix, Length + Width, Width);
	else if (LineType == LINE_TYPE_ROUND) 
		transform.Scale(ShapeMatrix, Length, Width);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y1, X2, Y2, Width);
}

void LineBrush::DrawLineX(GLfloat X1, GLfloat X2, GLfloat Y, GLfloat Width, GLfloat OpacityValue) {
	transform.Identity(ShapeMatrix);
	Opacity = OpacityValue;

	transform.Move(ShapeMatrix, (X1 + X2) / 2.0, Y);
	if (LineType == LINE_TYPE_RECT)
		transform.Scale(ShapeMatrix, fabs(X1 - X2) + Width, Width);
	else if (LineType == LINE_TYPE_ROUND) 
		transform.Scale(ShapeMatrix, fabs(X1 - X2), Width);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X1, Y, X2, Y, Width);
}

void LineBrush::DrawLineY(GLfloat Y1, GLfloat Y2, GLfloat X, GLfloat Width, GLfloat OpacityValue) {
	transform.Identity(ShapeMatrix);
	Opacity = OpacityValue;

	transform.Move(ShapeMatrix, X, (Y1 + Y2) / 2.0);
	if (LineType == LINE_TYPE_RECT)
		transform.Scale(ShapeMatrix, Width, fabs(Y1 - Y2) + Width);
	else if (LineType == LINE_TYPE_ROUND)
		transform.Scale(ShapeMatrix, fabs(Y1 - Y2), Width);

	Render();

	if (LineType == LINE_TYPE_ROUND)
		DrawCircle(X, Y1, X, Y2, Width);
}

void LineBrush::Render() {
	camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	imageUtil.RenderRaw();
}

void LineBrush::DrawCircle(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width) {
	transform.Identity(ShapeMatrix);
	transform.Move(ShapeMatrix, X1, Y1);
	RenderCircle(Width);

	transform.Identity(ShapeMatrix);
	transform.Move(ShapeMatrix, X2, Y2);
	RenderCircle(Width);
}

void LineBrush::RenderCircle(GLfloat Width) {
	camera.SetCamera(RenderType);

	glUseProgram(SHAPE_SHADER);
	camera.PrepareRender(SHADER_TYPE_SHAPE);

	glUniform1f(SHAPE_OPACITY_LOCATION, Opacity);
	glUniform3f(SHAPE_COLOR_LOCATION, Color.r, Color.g, Color.b);
	glUniformMatrix4fv(SHAPE_MODEL_LOCATION, 1, GL_FALSE, glm::value_ptr(ShapeMatrix));

	gluDisk(SysRes.GLU_CIRCLE, 0.0, Width * 0.5, 80, 1);
}
