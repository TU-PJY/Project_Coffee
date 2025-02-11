#pragma once
#include <Scene.h>

class TimeAdded : public GameObject {
private:
	TimerUtil Timer{};
	GLfloat Rotation{-25.0};
	GLfloat Opacity{0.0};
	GLfloat Size = 0.05;
	
	// 애니메이션 시퀀스
	int Seq = 1;

	SoundChannel SndChannel{};

public:
	TimeAdded() {
		soundUtil.Play(Snd.AddTime, SndChannel);
	}

	void UpdateFunc(float FrameTime) {
		if (Seq == 1) {
			Rotation += FrameTime * 200.0;
			EX.ClampValue(Rotation, 25.0, CLAMP_GREATER);

			Opacity += FrameTime * 4.0;
			EX.ClampValue(Opacity, 1.0, CLAMP_GREATER);

			Size += 0.8 * FrameTime;
			EX.ClampValue(Size, 0.25, CLAMP_GREATER);

			if (Rotation >= 25.0)
				Seq++;
		}

		else if (Seq == 2) {
			Timer.Update(FrameTime);
			if (Timer.Sec() >= 1) {
				Size -= 0.5 * FrameTime;
				EX.ClampValue(Size, 0.0, CLAMP_LESS);

				Opacity -= FrameTime * 2.0;
				EX.ClampValue(Opacity, 0.0, CLAMP_LESS);

				if (Opacity <= 0)
					scene.DeleteObject(this);
			}
		}
	}

	void RenderFunc() {
		Txt.PixelText.Reset();
		Txt.PixelText.SetAlign(ALIGN_MIDDLE);
		Txt.PixelText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Txt.PixelText.Rotate(Rotation);

		Txt.PixelText.SetColor(0.0, 0.0, 0.0);
		Txt.PixelText.SetOpacity(Opacity * 0.8);
		Txt.PixelText.Render(ASP(1.0) - 0.6 + 0.02, 0.3 - 0.02, Size, L"+10");

		Txt.PixelText.SetColorRGB(255, 245, 0);
		Txt.PixelText.SetOpacity(Opacity);
		Txt.PixelText.Render(ASP(1.0) - 0.6, 0.3, Size, L"+10");
	}
};

class TimeWatch : public GameObject {
private:
	// 초기 시간 10초
	GLfloat Time = 10;
	GLfloat AddValue{};

	bool Running{true};

	TimerUtil Timer{};
	bool RenderState{true};

public:
	void UpdateFunc(float FrameTime) {
		// 시간 감소
		if(Running)
			Time -= FrameTime;

		if (AddValue > 0.0) {
			Time += FrameTime * 20.0;
			AddValue -= FrameTime * 20.0;
		}

		else
			AddValue = 0.0;

		EX.ClampValue(Time, 0.0, CLAMP_LESS);

		// 시간이 끝나면 게임오버 된다
		if (Time <= 0.0) {
			Glb.GameOver = true;

			// 게임오버 엔딩 지정
			if(Glb.Score > 0)
				Glb.Ending = TimeOut;
			else
				Glb.Ending = Suppress;
		}
		
		if (Glb.GameOver) {
			Timer.Update(FrameTime);
			if (Timer.CheckMiliSec(0.3, 1, CHECK_AND_INTERPOLATE))
				EX.SwitchBool(RenderState);
		}
	}

	void RenderFunc() {
		Txt.PixelText.Reset();
		Txt.PixelText.SetAlign(ALIGN_MIDDLE);
		Txt.PixelText.SetHeightAlign(HEIGHT_ALIGN_UNDER);
		
		// 시간 출력
		if (!Glb.GameOver) {
			Txt.PixelText.SetOpacity(0.8);
			Txt.PixelText.SetColor(0.0, 0.0, 0.0);
			Txt.PixelText.Render(0.0 + 0.025, 1.0 - 0.025, 0.25, L"%d", (int)Time);

			Txt.PixelText.SetOpacity(1.0);
			if(Time < 6.0)
				Txt.PixelText.SetColor(1.0, 0.0, 0.0);
			else {
				if(AddValue > 0.0)
					Txt.PixelText.SetColor(0.0, 1.0, 0.0);
				else
					Txt.PixelText.SetColorRGB(255, 213, 80);
			}
			Txt.PixelText.Render(0.0, 1.0, 0.25, L"%d", (int)Time);
		}

		else {
			if (RenderState) {
				Txt.PixelText.SetOpacity(0.8);
				Txt.PixelText.SetColor(0.0, 0.0, 0.0);

				if(Glb.GameOver && Time <= 0.0)
					Txt.PixelText.Render(0.0 + 0.03, 1.0 - 0.03, 0.3, L"TIME OUT!");
				else if(Glb.GameOver && Time > 0.0)
					Txt.PixelText.Render(0.0 + 0.03, 1.0 - 0.03, 0.3, L"GAME OVER");

				Txt.PixelText.SetOpacity(1.0);
				Txt.PixelText.SetColorRGB(255, 213, 80);

				if (Glb.GameOver && Time <= 0.0)
					Txt.PixelText.Render(0.0, 1.0, 0.3, L"TIME OUT!");
				else if (Glb.GameOver && Time > 0.0)
					Txt.PixelText.Render(0.0, 1.0, 0.3, L"GAME OVER");
			}
		}
	}

	void Stop() {
		Running = false;
	}

	void Start() {
		Running = true;
	}

	void AddTime() {
		AddValue += 10;
		scene.AddObject(new TimeAdded, "time_added", LAYER7);
	}
};