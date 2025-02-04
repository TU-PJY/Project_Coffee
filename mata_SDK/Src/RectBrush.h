#pragma once
#include "SDKHeader.h"

class LineRectBrush {
private:
	glm::vec3 Color{};
	GLfloat   Length{};
	GLfloat   Opacity{};
	int       RenderType{ RENDER_TYPE_DEFAULT };

	bool      CamInheritanceCommand{};

public:
	LineRectBrush(bool Flag=false);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColor(glm::vec3& ColorValue);
	void SetColorRGB(int R, int G, int B);
	void SetRenderType(int Opt);
	void Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat Width, GLfloat RotationValue=0.0f, GLfloat OpacityValue = 1.0f);

private:
	void DrawLine(GLfloat X, GLfloat Y, GLfloat OffsetX, GLfloat OffsetY, GLfloat Width, GLfloat Height, GLfloat RotationValue);
	void Render();
};


class RectBrush {
private:
	glm::vec3 Color{};
	GLfloat   Length{};
	GLfloat   Opacity{};
	int       RenderType{ RENDER_TYPE_DEFAULT };

	bool      CamInheritanceCommand{};

public:
	RectBrush(bool Flag=false);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColor(glm::vec3& ColorValue);
	void SetColorRGB(int R, int G, int B);
	void SetRenderType(int Opt);
	void Draw(GLfloat X, GLfloat Y, GLfloat SizeX, GLfloat SizeY, GLfloat RotationValue=0.0f, GLfloat OpacityValue = 1.0f);

private:
	void Render();
};