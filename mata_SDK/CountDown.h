#pragma once
#include <Scene.h>

class CountDown : public GameObject {
private:
	TimerUtil Timer{};
	TimerUtil DeleteTimer{};

	TextUtil Text{};
	SoundChannel SndChannel{};

	GLfloat Size{};

	int Count = 4;

public:
	CountDown() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetColorRGB(255, 213, 80);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.8);
	}

	void UpdateFunc(float FrameTime) {
		Timer.Update(FrameTime);
		if (Timer.CheckSec(1, CHECK_AND_INTERPOLATE)) {
			Count--;
			Size += 0.2;
			if (Count > 0) {
				soundUtil.Play(Snd.CountDownReady, SndChannel);
				SndChannel->setVolume(Glb.SFXVolume);
			}
			else {
				soundUtil.Play(Snd.CountDownGo, SndChannel);
				SndChannel->setVolume(Glb.SFXVolume);

				// 카운트다운을 모두 세면 타이머 동작 시작
				if (auto TimeWatch = scene.Find("time_watch"); TimeWatch)
					TimeWatch->Start();

				// 이드 입력 활성화
				if (auto ED = scene.Find("ed"); ED)
					ED->EnableInput();

				// bgm 재생
				if (auto Manager = scene.Find("play_mode_manager"); Manager)
					Manager->PlayBGM();

				Timer.Stop();
				Timer.Reset();
			}
		}

		// 카운트를 모두 센 후 1초 후 삭제 
		if (Count == 0) {
			DeleteTimer.Update(FrameTime);
			if (DeleteTimer.Sec() >= 1) 
				scene.DeleteObject(this);
		}

		mathUtil.Lerp(Size, 0.0, 10.0, FrameTime);
	}

	void RenderFunc() {
		if (scene.Mode() != "PlayMode")
			return;

		if (0 < Count && Count < 4)
			Text.Render(0.0, 0.0, 0.4 + Size, L"%d", Count);
		else if (Count == 0) {
			Text.SetColor(0.0, 1.0, 0.0);
			Text.Render(0.0, 0.0, 0.4 + Size, L"GO!");
		}
	}
};