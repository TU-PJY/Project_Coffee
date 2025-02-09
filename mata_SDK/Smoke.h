#pragma once
#include <Scene.h>

class Smoke : public GameObject {
private:
	glm::vec2 Position{};

	// 새로운 연기 객체 추가 여부
	bool NewSmokeAdded{};

	// 크기
	GLfloat Size { 2.5 };
	
public:
	Smoke(GLfloat PositionValue) {
		Position.x = PositionValue;
		Position.y = -1.0 - Size * 0.5;
	}

	void UpdateFunc(float FrameTime) {
		Position.y += FrameTime * 0.8;

		// 연기가 화면 위쪽까지 도달하면 아래쪽에 새로운 객체를 생성한다
		if (!NewSmokeAdded && Position.y - Size * 0.5 >= -1.0) {
			Position.y = -1.0 + Size * 0.5;
			scene.AddObject(new Smoke(Position.x), "smoke", LAYER5);
			NewSmokeAdded = true;
		}

		if(Position.y - Size * 0.5 >= 1.0 || Position.x < CameraPosition.x - ASP(1.0) - Size * 0.5)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		if (Position.x > CameraPosition.x + ASP(1.0) + Size * 0.5)
			return;

		Begin();
		transform.Move(MoveMatrix, Position.x + 0.2, Position.y);
		transform.Scale(MoveMatrix, Size, Size);
		imageUtil.Render(Img.Smoke, 0.5);
	}
};