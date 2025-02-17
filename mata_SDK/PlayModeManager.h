#pragma once
#include <Scene.h>

#include "TitleMode.h"
#include "GameOverMode.h"
#include "PauseMode.h"

#include "Cover.h"

class PlayModeManager : public GameObject {
private:
	TimerUtil Timer1{}, Timer2{};
	bool GameStart{};

	// 타이틀 화면으로 돌아가기
	bool GoToTitle{};
	bool CoverAdded{};

public:
	// 일시정지 모드로 전환
	void InputKey(KeyEvent& Event) {
		if (GameStart && Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_ESCAPE)
			scene.StartFloatingMode(PauseMode.Start, true);
	}

	void UpdateFunc(float FrameTime) {
		// 3초 후 화면이 어두워진다
		if (Glb.GameOver) {
			Timer1.Update(FrameTime);
			if (Timer1.Sec() >= 3) {
				scene.AddObject(new Cover, "cover", LAYER7);
				Timer1.Reset();
				Timer1.Stop();
			}

			//  화면이 완전히 어두워진 후 1초 뒤 게임오버 스크린으로 전환한다
			if (auto Cover = scene.Find("cover"); Cover) {
				if (Cover->GetState()) {
					Timer2.Update(FrameTime);

					if (Timer2.Sec() >= 1) {
						// 게임오버 모드로 전환
						scene.SwitchMode(GameOverMode.Start);
					}
				}
			}
		}

		else if (GoToTitle) {
			if (!CoverAdded) {
				scene.AddObject(new Cover, "cover", LAYER7);
				CoverAdded = true;
			}

			//  화면이 완전히 어두워진 후 1초 뒤 타이틀 화면으로 전환한다
			if (auto Cover = scene.Find("cover"); Cover) {
				if (Cover->GetState()) {
					Timer2.Update(FrameTime);

					if (Timer2.Sec() >= 1) {
						// 타이틀 모드로 전환
						scene.SwitchMode(TitleMode.Start);
					}
				}
			}
		}
	}

	// Bgm 재생
	void PlayBGM() {
		int RandomIndex = randomUtil.Gen(RANDOM_TYPE_INT, 0, 2);
		soundUtil.Play(Snd.PlayBgm[RandomIndex], Glb.BGMChannel);
		Glb.BGMChannel->setVolume(Glb.BGMVolume);
		GameStart = true;
	}

	// Bgm 정지
	void StopBGM() {
		soundUtil.Stop(Glb.BGMChannel);
		GameStart = false;
	}

	void SetGoToTitle() {
		GoToTitle = true;
	}
};