#pragma once
#include <Scene.h>

class Cart : public GameObject {
private:
	bool FellDown{};
	glm::vec2 Position{};
	GLfloat ChloeFrame{};

public:
	Cart(bool Fell, glm::vec2 PositionValue);
	void UpdateFunc(float FrameTime);
	void RenderFunc();
};