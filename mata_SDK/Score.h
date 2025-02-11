#pragma once
#include <Scene.h>

class ScoreIndicator : public GameObject {
private:
	GLfloat FeedbackHeight{};
	GLfloat FeedbackOpacity{};
	int AddedScore{};

	TimerUtil Timer{};

	TextUtil Text{};

public:
	ScoreIndicator() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
	}

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
		Text.SetColorRGB(255, 213, 80);
		Text.SetOpacity(1.0);
		Text.Render((ASP(-1.0) + 0.07), (-1.0 + 0.1 + FeedbackHeight), 0.15, L"SCORE %d", Glb.Score);
		if (FeedbackOpacity > 0.0) {
			Text.SetOpacity(FeedbackOpacity);
			Text.Render((ASP(-1.0) + 0.07), (-1.0 + 0.3 + FeedbackHeight), 0.1, L"+%d", AddedScore);
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