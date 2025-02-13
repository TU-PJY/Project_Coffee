#pragma once
#include <Scene.h>

class Pillar : public GameObject {
private:
	GLfloat Position{};
	bool NextPillarGenerated{};
	bool GenerateNextPillar{};

public:
	Pillar(GLfloat PositionValue, bool GenNextFlag=true) {
		GenerateNextPillar = GenNextFlag;
		Position = PositionValue;
	}

	void UpdateFunc(float FrameTime) {
		if (GenerateNextPillar) {
			if (!NextPillarGenerated && Position + CameraPosition.x * 0.3 < CameraPosition.x + ASP(0.5)) {
				scene.AddObject(new Pillar(Position + 3.0), "pillar", LAYER_BG);
				NextPillarGenerated = true;
			}
		}

		if (Position + CameraPosition.x * 0.3 < CameraPosition.x - ASP(1.0) - 0.75)
			scene.DeleteObject(this);
	}

	void RenderFunc() {
		if (Position + CameraPosition.x * 0.3 > CameraPosition.x + ASP(1.0) + 0.75)
			return;

		Begin();
		transform.Move(MoveMatrix, Position + CameraPosition.x * 0.3, 0.3);
		transform.Scale(MoveMatrix, 1.5, 1.5);
		imageUtil.Render(Img.Pillar);
	}
};

