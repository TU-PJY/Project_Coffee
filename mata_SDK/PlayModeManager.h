#pragma once
#include <Scene.h>
#include "Cover.h"
#include "GameOverMode.h"

class PlayModeManager : public GameObject {
private:
	TimerUtil Timer1{}, Timer2{};

	void UpdateFunc(float FrameTime) {
		// 3초 후 화면이 어두워진다
		if (Glb.GameOver) {
			Timer1.Update(FrameTime);
			if (Timer1.Sec() >= 4) {
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
	}

	// Bgm 재생
	void PlayBGM() {
		// 2가지 bgm중 하나를 선택해 플레이 한다
		int RandNum = randomUtil.Gen(RANDOM_TYPE_INT, 0, 1);
		soundUtil.Play(Snd.PlayBgm[RandNum], Glb.BGMChannel);
	}

	// Bgm 정지
	void StopBGM() {
		soundUtil.Stop(Glb.BGMChannel);
	}
};