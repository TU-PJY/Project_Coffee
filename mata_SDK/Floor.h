#pragma once
#include <Scene.h>

class Floor : public GameObject {
	GLfloat Position{};
	bool NewFloorGenerated{};

public:
	Floor(GLfloat PositionValue, bool GenFlag) {
		Position = PositionValue;
		NewFloorGenerated = GenFlag;
	}

	void UpdateFunc(float FrameTime) {
		// �ڽ��� ��ġ�� ȭ�� �������� �鷯���� �ڽ��� �ٷ� �����ʿ� ���ο� ��ü�� �߰��Ѵ�
		if (!NewFloorGenerated && Position <= CameraPosition.x + ASP(1.0) - 0.4) {
			scene.AddObject(new Floor(Position + 0.8, false), "floor", LAYER1);
			NewFloorGenerated = true;
		}

		// ȭ���� �ָ� ����� �����Ѵ�
		if (Position < CameraPosition.x - ASP(1.0) - 2.0)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// ȭ�鿡�� ������ �ʴ� �ٴ��� ���������� �ʴ´�
		if (Position > CameraPosition.x + ASP(1.0) + 0.4)
			return;

		Begin();
		transform.Move(MoveMatrix, Position, -0.85);
		transform.Scale(MoveMatrix, 0.8, 0.8);
		imageUtil.Render(Img.FloorTile);
	} 
};