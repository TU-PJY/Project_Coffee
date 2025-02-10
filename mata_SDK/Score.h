#pragma once
#include <Scene.h>

class ScoreIndicator : public GameObject {
private:
	GLfloat Height{};

public:
	void UpdateFunc(float FrameTime) {
		mathUtil.Lerp(Height, 0.0, 8.0, FrameTime);
	}

	void RenderFunc() {
		Txt.PixelText.Reset();

		Txt.PixelText.SetOpacity(0.8);
		Txt.PixelText.SetColor(0.0, 0.0, 0.0);
		Txt.PixelText.Render((ASP(-1.0) + 0.07) + 0.015, (-1.0 + 0.1 + Height) - 0.015, 0.15, L"SCORE %d", Glb.Score);

		Txt.PixelText.SetOpacity(1.0);
		Txt.PixelText.SetColor(1.0, 1.0, 1.0);
		Txt.PixelText.Render((ASP(-1.0) + 0.07), (-1.0 + 0.1 + Height), 0.15, L"SCORE %d", Glb.Score);
	}

	void AddScore(int Value) {
		Glb.Score += Value;
		Height = 0.1;
	}
};