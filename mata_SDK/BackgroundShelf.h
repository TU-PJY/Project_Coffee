#pragma once
#include <Scene.h>

class BackgroundShelf : public GameObject{
private:
	GLfloat Position{};
	bool NextShelfGenerated{};

public:
	BackgroundShelf(GLfloat PositionValue) {
		Position = PositionValue;
	}

	void UpdateFunc(float FrameTime) {
		if (!NextShelfGenerated && Position < CameraPosition.x + ASP(1.0) - 2.5) {
			scene.AddObject(new BackgroundShelf(Position + 5.0), "background_shelf", LAYER1);
			NextShelfGenerated = true;
		}

		if (Position < CameraPosition.x - ASP(1.0) - 2.5)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		if (Position> CameraPosition.x + ASP(1.0) + 2.5)
			return;

		Begin();
		transform.Move(MoveMatrix, Position, 0.0);
		transform.Scale(MoveMatrix, 5.0, 5.0);
		imageUtil.Render(Img.BackGroundShelf);
	}
};