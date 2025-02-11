#pragma once
#include <Scene.h>

class ScoreIndicator : public GameObject {
private:
	GLfloat FeedbackHeight{};
	GLfloat FeedbackOpacity{};
	int AddedScore{};

	TimerUtil Timer{};

public:
	void UpdateFunc(float FrameTime) {
		mathUtil.Lerp(FeedbackHeight, 0.0, 8.0, FrameTime);

		if(FeedbackOpacity > 0.0) {
			Timer.Update(FrameTime);
			if (Timer.MiliSec(1) >= 0.5)
				FeedbackOpacity -= FrameTime * 2.0;
			EX.ClampValue(FeedbackOpacity, 0.0, CLAMP_LESS);
		}
		else 
			Timer.Reset();
	}

	void RenderFunc() {
		Txt.PixelText.Reset();

		Txt.PixelText.SetOpacity(0.8);
		Txt.PixelText.SetColor(0.0, 0.0, 0.0);
		Txt.PixelText.Render((ASP(-1.0) + 0.07) + 0.015, (-1.0 + 0.1 + FeedbackHeight) - 0.015, 0.15, L"SCORE %d", Glb.Score);

		Txt.PixelText.SetOpacity(1.0);
		Txt.PixelText.SetColorRGB(255, 213, 80);
		Txt.PixelText.Render((ASP(-1.0) + 0.07), (-1.0 + 0.1 + FeedbackHeight), 0.15, L"SCORE %d", Glb.Score);

		if (FeedbackOpacity > 0.0) {
			Txt.PixelText.SetOpacity(FeedbackOpacity * 0.8);
			Txt.PixelText.SetColor(0.0, 0.0, 0.0);
			Txt.PixelText.Render((ASP(-1.0) + 0.07) + 0.01, (-1.0 + 0.3 + FeedbackHeight) - 0.01, 0.1, L"+%d", AddedScore);

			Txt.PixelText.SetOpacity(FeedbackOpacity);
			Txt.PixelText.SetColorRGB(255, 213, 80);
			Txt.PixelText.Render((ASP(-1.0) + 0.07), (-1.0 + 0.3 + FeedbackHeight), 0.1, L"+%d", AddedScore);
		}
	}

	void AddScore(int Value) {
		Timer.Reset();
		Glb.Score += Value;
		AddedScore = Value;
		FeedbackHeight = 0.1;
		FeedbackOpacity = 1.0;
	}
};