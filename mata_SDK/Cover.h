#pragma once
#include <Scene.h>

class Cover : public GameObject {
private:
	GLfloat Opacity{ 0.0 };

public:
	void UpdateFunc(float FrameTime);
	void RenderFunc();
	bool GetState();
};