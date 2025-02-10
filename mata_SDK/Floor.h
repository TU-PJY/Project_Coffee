#pragma once
#include <Scene.h>

class Floor : public GameObject {
	GLfloat Position{};
	bool NewFloorGenerated{};

public:
	Floor(GLfloat PositionValue) {
		Position = PositionValue;
	}

	void UpdateFunc(float FrameTime) {
		if (!NewFloorGenerated && Position <= CameraPosition.x + ASP(1.0) - 0.4) {
			scene.AddObject(new Floor(Position + 0.8), "floor", LAYER1);
			NewFloorGenerated = true;
		}

		if (Position < CameraPosition.x - ASP(1.0) - 0.8)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		if (Position > CameraPosition.x + ASP(1.0) + 0.4)
			return;

		Begin();
		transform.Move(MoveMatrix, Position, -0.85);
		transform.Scale(MoveMatrix, 0.8, 0.8);
		imageUtil.Render(Img.FloorTile);
	} 
};