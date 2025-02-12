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
		// 자신의 위치가 화면 안쪽으로 들러오면 자신의 바로 오른쪽에 새로운 객체를 추가한다
		if (!NewFloorGenerated && Position <= CameraPosition.x + ASP(1.0) - 0.4) {
			scene.AddObject(new Floor(Position + 0.8, false), "floor", LAYER1);
			NewFloorGenerated = true;
		}

		// 화면을 멀리 벗어나면 삭제한다
		if (Position < CameraPosition.x - ASP(1.0) - 2.0)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		// 화면에서 보이지 않는 바닥은 렌더링하지 않는다
		if (Position > CameraPosition.x + ASP(1.0) + 0.4)
			return;

		Begin();
		transform.Move(MoveMatrix, Position, -0.85);
		transform.Scale(MoveMatrix, 0.8, 0.8);
		imageUtil.Render(Img.FloorTile);
	} 
};