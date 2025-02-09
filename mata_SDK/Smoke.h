#pragma once
#include <Scene.h>

class Smoke : public GameObject {
private:
	glm::vec2 Position{};

	// ���ο� ���� ��ü �߰� ����
	bool NewSmokeAdded{};

	// ũ��
	GLfloat Size { 2.5 };
	
public:
	Smoke(GLfloat PositionValue) {
		Position.x = PositionValue;
		Position.y = -1.0 - Size * 0.5;
	}

	void UpdateFunc(float FrameTime) {
		Position.y += FrameTime * 0.8;

		// ���Ⱑ ȭ�� ���ʱ��� �����ϸ� �Ʒ��ʿ� ���ο� ��ü�� �����Ѵ�
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