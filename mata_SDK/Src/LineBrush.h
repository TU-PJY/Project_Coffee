#pragma once
#include "SDKHeader.h"

class LineBrush {
private:
	GLfloat   Rotation;
	GLfloat   Length{};
	glm::vec3 Color{};
	GLfloat   Opacity{};

	int       RenderType { RENDER_TYPE_DEFAULT };
	int       LineType{ LINE_TYPE_RECT };

public:
	void SetRenderType(int Opt);
	void SetColor(GLfloat R, GLfloat G, GLfloat B);
	void SetColor(glm::vec3& ColorValue);
	void SetColorRGB(int R, int G, int B);
	void SetLineType(int LineTypeOpt);
	void Draw(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width, GLfloat OpacityValue = 1.0);
	void DrawLineX(GLfloat X1, GLfloat X2, GLfloat Y, GLfloat Width, GLfloat OpacityValue = 1.0);
	void DrawLineY(GLfloat Y1, GLfloat Y2, GLfloat X, GLfloat Width, GLfloat OpacityValue = 1.0);

private:
	void Render();
	void DrawCircle(GLfloat X1, GLfloat Y1, GLfloat X2, GLfloat Y2, GLfloat Width);
	void RenderCircle(GLfloat Width);
};