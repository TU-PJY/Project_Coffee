#pragma once
#include "SDKHeader.h"

class Camera {
private:
	glm::vec3 CamPos{}, CamDirection{}, CamUp{};
	bool      StaticRenderCommand{};
	
public:
	glm::mat4 ViewMatrix{ 1.0 }, Projection{ 1.0 }, MoveMatrix{ 1.0 }, RotateMatrix{ 1.0 };
	GLfloat   ZoomValue{ 1.0f };

	void CalculateASPECT();
	void Init();
	void SetCamera(int RenderType);
	void PrepareRender(int ShaderType);
};
extern Camera camera;