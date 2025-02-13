#pragma once
#include <Scene.h>

class TimeAdded : public GameObject {
private:
	TimerUtil Timer{};
	GLfloat Rotation{-25.0};
	GLfloat Opacity{0.0};
	GLfloat Size = 0.05;
	
	// �ִϸ��̼� ������
	int Seq = 1;

	SoundChannel SndChannel{};

	TextUtil Text{};

public:
	TimeAdded() {
		soundUtil.Play(Snd.AddTime, SndChannel);
		SndChannel->setVolume(Glb.SFXVolume);

		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
		Text.SetColorRGB(255, 213, 80);
	}

	void UpdateFunc(float FrameTime) {
		if (Seq == 1) {
			mathUtil.Lerp(Rotation, 25.0, 10.0, FrameTime);
			mathUtil.Lerp(Opacity, 1.0, 10.0, FrameTime);
			mathUtil.Lerp(Size, 0.25, 10.0, FrameTime);

			if (Rotation >= 24.999)
				Seq++;
		}

		else if (Seq == 2) {
			mathUtil.Lerp(Opacity, 0.0, 10.0, FrameTime);
			mathUtil.Lerp(Size, 0.0, 10.0, FrameTime);

			if (Opacity <= 0.001)
				scene.DeleteObject(this);
		}
	}

	void RenderFunc() {
		Text.Rotate(Rotation);
		Text.Render(ASP(1.0) - 0.6, 0.3, Size, L"+10");
	}
};

class TimeWatch : public GameObject {
private:
	// �ʱ� �ð� 10��
	GLfloat Time = 10;
	GLfloat AddValue{};

	bool Running{};

	SinLoop Loop{};
	GLfloat Size{};


	TextUtil Text{};

	glm::vec3 Color = { 1, 0.84, 0.31 };

public:
	TimeWatch() {
		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_UNDER);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
	}

	void UpdateFunc(float FrameTime) {
		// �ð� ����
		if(Running)
			Time -= FrameTime;
		EX.ClampValue(Time, 0.0, CLAMP_LESS);

		if (AddValue > 0.0) {
			Time += FrameTime * 20.0;
			AddValue -= FrameTime * 20.0;
			EX.ClampValue(AddValue, 0.0, CLAMP_LESS);
		}

		// �ð��� ������ ���ӿ��� �ȴ�
		if (Time <= 0.0) {
			Glb.GameOver = true;

			if (auto Manager = scene.Find("play_mode_manager"); Manager)
				Manager->StopBGM();

			// ���ӿ��� ���� ����
			if(Glb.Score > 0)
				Glb.Ending = GameOver_TimeOut;
			else
				Glb.Ending = GameOver_Suppressed;
		}
		
		if (Glb.GameOver) 
			Size = Loop.Update(0.025, 5.0, FrameTime);

		mathUtil.Lerp(Color, glm::vec3(1, 0.84, 0.31), 5.0, FrameTime);
	}

	void RenderFunc() {
		if (scene.Mode() != "PlayMode")
			return;

		// �ð� ���
		if (!Glb.GameOver) {
			if(Time < 6.0)
				Text.SetColor(1.0, 0.0, 0.0);

			else {
				if(AddValue > 0.0)
					Text.SetColor(0.0, 1.0, 0.0);
				else
					Text.SetColor(Color);
			}

			Text.Render(0.0, 1.0, 0.25, L"%d", (int)Time);
		}

		else {
			Text.SetColorRGB(255, 213, 80);

			if (Glb.GameOver && Time <= 0.0)
				Text.Render(0.0, 1.0, 0.3 + Size, L"TIME OUT!");

			else if (Glb.GameOver && Time > 0.0)
				Text.Render(0.0, 1.0, 0.3 + Size, L"GAME OVER");
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

	void DeleteTime() {
		Color = glm::vec3(1.0, 0.0, 0.0);
		Time -= 2;
		EX.ClampValue(Time, 0.0, CLAMP_LESS);
	}
};