#pragma once
#include "SDKHeader.h"

class CircleBrush {
private:
	glm::vec3 Color{};
	GLfloat   Radius{};
	GLfloat   Opacity{};
	int       RenderType{ RENDER_TYPE_DEFAULT };

	bool      CamInheritanceCommand{};

public:
	CircleBrush(bool Flag = false);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColor(glm::vec3& ColorValue);
	void SetColorRGB(int R, int G, int B);
	void SetRenderType(int Opt);
	void Draw(GLfloat X, GLfloat Y, GLfloat Diameter, GLfloat OpacityValue=1.0f);

private:
	void Render();
};

class LineCircleBrush {
private:
	glm::vec3 Color{};
	GLfloat   Radius{}, WidthValue{};
	GLfloat   Opacity{};
	int       RenderType{ RENDER_TYPE_DEFAULT };

	bool      CamInheritanceCommand{};

public:
	LineCircleBrush(bool Flag = false);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColor(glm::vec3& ColorValue);
	void SetColorRGB(int R, int G, int B);
	void SetRenderType(int Opt);
	void Draw(GLfloat X, GLfloat Y, GLfloat Diameter, GLfloat Width, GLfloat OpacityValue = 1.0f);

private:
	void Render();
};