#pragma once
#include <Scene.h>

#include "Cover.h"
#include "TitleMode.h"

struct Reparations {
	int DestroyedType;
	int NumDestroyed;
};

class GameOverScreen : public GameObject {
private:
	// Ŭ�ο��� ���� ������ ���� Ÿ�̸�
	TimerUtil Timer{};

	// Ŭ�ο��� ���� ������ ����
	GLfloat CheekInterval{};

	// �ÿ��� ���� ���� Ÿ�̸�
	TimerUtil ShiverTimer{};

	// ���� �ִϸ��̼� ��ġ
	glm::vec2 ShiverValue{};
	glm::vec2 ShiverValue2{};

	// ���� ũ�� ��ġ
	GLfloat VerticalSize{};

	// �ÿ� ���� ũ�� ��ġ
	GLfloat XionSize{};
	SinLoop XionSizeLoop{};

	// �ؽ�Ʈ ũ��
	GLfloat TextSize{};

	// �ؽ�Ʈ ũ�� ����
	SinLoop TextSizeLoop{};

	// ������
	int ChloeFrame{};
	int EDFrame{};

	// �̵� ������ Ÿ�̸�
	TimerUtil EDFrameTimer{};

	// Ȩ��� ��ȯ ����
	bool ExitState{};

	std::wstring Str{};
	std::wstring Str2{};

	TextUtil Text{}, ScoreText{};

	// ���� 
	///////
	// ���� ǥ�õǴ� ����
	bool ShowRep{};

	// �� ���� ǥ�õǴ� ����
	bool ShowTotalRep{};
	 
	// ��� ǰ���� �����ϴ� ����
	std::vector <Reparations> RepVec{};
	size_t VecSize{};
	int CurrentSize{};

	// �� ����
	int TotalRep{};

	// ���ݾ��� ���������� �����ֱ� ���� Ÿ�̸�
	TimerUtil RepTimer{};

	// �� ���ݾ� �ؽ�Ʈ ��鸲 ��
	glm::vec2 TextShake{};
	GLfloat TextShakeValue{0.05};
	TimerUtil TextShakeTimer{};

	SoundChannel SndChannel1{};
	SoundChannel SndChannel2{};
	SoundChannel SndChannel3{};
	GLfloat Volume = 1.0;


	// Ÿ��Ʋ�� ������ ���� Ÿ�̸�
	TimerUtil DeleteTimer{};

	bool NewHighScore{};
	bool NewHighRep{};

public:
	GameOverScreen() {
		if(Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion)
			System.SetBackColorRGB(74, 78, 101);
		else
			System.SetBackColorRGB(255, 178, 80);

		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetUseShadow(true);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetLineGap(0.01);
		Text.Rotate(-10.0);

		ScoreText.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		ScoreText.SetAlign(ALIGN_MIDDLE);
		ScoreText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		ScoreText.SetUseShadow(true);
		ScoreText.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.7);
		ScoreText.SetColor(1.0, 1.0, 1.0);
		ScoreText.SetLineGap(0.02);

		if (Glb.Ending == GameOver_TimeOut) {
			CheekInterval = 0.7;
			Str = L"����!!!";
			Str2 = L"�̵���������!!!";
		}

		else if (Glb.Ending == GameOver_HitXion) {
			CheekInterval = 0.25;
			Str = L"��¥ ����!!!";
			Str2 = L"�̵������������ФФФФ�!!!";
		}

		else if (Glb.Ending == GameOver_Suppressed) {
			Str = L"����!!!";
			Str2 = L"�̵�! ���Ƴ±���!";
		}

		soundUtil.Play(Snd.GameEnd[Glb.Ending], SndChannel1);
		SndChannel1->setVolume(Glb.BGMVolume);

		// �ı��� ǰ���� �����ϸ� ���Ϳ� �����Ѵ�
		for (int i = 0; i < 6; i++) {
			Reparations Rep{};
			if (Glb.DestroyedItems[i] > 0) {
				Rep.DestroyedType = i;
				Rep.NumDestroyed = Glb.DestroyedItems[i];
				RepVec.emplace_back(Rep);
			}
		}

		VecSize = RepVec.size();

		// �� ���� ���
		TotalRep += Glb.DestroyedItems[Item_BoxCoffee] * 25000;
		TotalRep += Glb.DestroyedItems[Item_BottleCoffee] * 2500;
		TotalRep += Glb.DestroyedItems[Item_CanCoffee] * 1200;
		TotalRep += Glb.DestroyedItems[Item_People] * 1500000;
		TotalRep += Glb.DestroyedItems[Item_Xion] * 1000000;
		TotalRep += Glb.DestroyedItems[Item_HouseDoc] * 100000;

		if (Glb.HighScore < Glb.Score) {
			NewHighScore = true;
			Dat.HighscoreData.UpdateDigitData("HighScore", "Score", Glb.Score);
		}

		if (Glb.MaxRep < TotalRep) {
			NewHighRep = true;
			Dat.HighscoreData.UpdateDigitData("HighScore", "Rep", TotalRep);
		}
	}

	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN && Event.NormalKey == NK_ENTER) {
			if (Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion) {
				// ù ���� �Է��� �� ���� ȭ������ ��ȯ�ϰ�, �� ��° �Է��� Ÿ��Ʋ ���� ��ȯ�Ѵ�
				if (!ShowRep) {
					ShowRep = true;
					ScoreText.SetUseShadow(false);
				}
				else if (ShowTotalRep) {
					ExitState = true;
					scene.AddObject(new Cover, "cover", LAYER7);
					// ������ ���¸� Ȱ��ȭ�ϰ� �� �̻� �Է��� ���� �ʵ��� �Ѵ�
					scene.DeleteInputObject(this);
				}
			}

			// �¿����� ��� �ٷ� Ÿ��Ʋ ���� ��ȯ�Ѵ�
			else {
				ExitState = true;
				scene.AddObject(new Cover, "cover", LAYER7);
				// ������ ���¸� Ȱ��ȭ�ϰ� �� �̻� �Է��� ���� �ʵ��� �Ѵ�
				scene.DeleteInputObject(this);
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		if (Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion) {
			Timer.Update(FrameTime);
			if (Timer.CheckMiliSec(CheekInterval, 2, CHECK_AND_INTERPOLATE)) {
				ChloeFrame++;
				
				// ���� �������� Ŭ�ο��� �̵��� ���� ������
				if (ChloeFrame == 1) {
					EDFrame = 1;
					EDFrameTimer.Reset();
					VerticalSize = 0.3;
					soundUtil.Stop(SndChannel2);
					soundUtil.Play(Snd.HitCheek, SndChannel2);
					SndChannel2->setVolume(Glb.SFXVolume);
				}
				EX.ClampValue(ChloeFrame, 0, 1, CLAMP_RETURN);
			}

			// ���� �ִϸ��̼�
			ShiverTimer.Update(FrameTime);
			if (ShiverTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
				ShiverValue.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
				ShiverValue2.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			}

			// ���� ũ�� �ִϸ��̼�
			mathUtil.Lerp(VerticalSize, 0.0, 10.0, FrameTime);

			// �̵�� ���� ���� �� ���� �Ŀ� �ٽ� ���� ���������� �����Ѵ�
			if (EDFrame == 1) {
				EDFrameTimer.Update(FrameTime);
				if (EDFrameTimer.CheckMiliSec(CheekInterval * 0.5, 2, CHECK_AND_RESET))
					EDFrame = 0;
			}
		}

		else 
			XionSize = XionSizeLoop.Update(0.05, 20.0, FrameTime);

		// �ؽ�Ʈ �ִϸ��̼�
		TextSize = TextSizeLoop.Update(0.01, 10.0, FrameTime);

		// �� ������ ���������� �����ش�
		if (ShowRep) {
			RepTimer.Update(FrameTime);
			if (CurrentSize != VecSize && RepTimer.CheckMiliSec(0.5, 1, CHECK_AND_INTERPOLATE)) {
				CurrentSize++;
				soundUtil.Play(Snd.Rep, SndChannel3);
				SndChannel3->setVolume(Glb.SFXVolume);
			}
			
			else if (CurrentSize == VecSize && RepTimer.CheckMiliSec(1, 1, CHECK_AND_INTERPOLATE)) {
				ShowTotalRep = true;
				soundUtil.Play(Snd.RepTotal, SndChannel3);
				SndChannel3->setVolume(Glb.SFXVolume);
				RepTimer.Stop();
				RepTimer.Reset();
			}

			// ���� �Ѿ��� ��鸮�� �ִϸ��̼ǰ� �Բ� �����ش�
			if (ShowTotalRep) {
				TextShakeTimer.Update(FrameTime);
				if (TextShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
					TextShake.x = randomUtil.Gen(RANDOM_TYPE_REAL, -TextShakeValue, TextShakeValue);
					TextShake.y = randomUtil.Gen(RANDOM_TYPE_REAL, -TextShakeValue, TextShakeValue);
				}

				mathUtil.Lerp(TextShakeValue, 0.0, 5.0, FrameTime);
			}
		}

		// ȭ���� ��ο����鼭 Ȩ���� ��ȯ�Ѵ�
		// ������ �ε巴�� ���δ�
		if (ExitState) {
			Volume -= FrameTime * 0.5;
			EX.ClampValue(Volume, 0.0, CLAMP_LESS);
			SndChannel1->setVolume(Volume * Glb.BGMVolume);
			SndChannel2->setVolume(Volume * Glb.SFXVolume);

			if(auto Cover = scene.Find("cover"); Cover)
				if (Cover->GetState()) {
					DeleteTimer.Update(FrameTime);
					if (DeleteTimer.Sec() >= 1)
						scene.SwitchMode(TitleMode.Start);
				}
		}
	}

	void RenderFunc() {
		// ��� ������
		Begin();
		transform.Scale(MoveMatrix, 3.0, 3.0);

		if(Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion)
			imageUtil.Render(Img.GameOverBackGround, 0.7);
		else
			imageUtil.Render(Img.GameOverBackGround2, 0.7);

		// �ÿ� ������
		Begin();
		transform.Move(MoveMatrix, ASP(-1.0) + 0.5 + ShiverValue.x, -0.3 + ShiverValue.y + XionSize * 0.5);
		transform.Scale(MoveMatrix, 1.5, 1.5 + XionSize);

		// �������� �ÿ��� �������� �޶�����
		if(Glb.Ending == GameOver_HitXion)
			imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Hurt);
		else if(Glb.Ending == GameOver_TimeOut)
			imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Cry1);
		else if (Glb.Ending == GameOver_Suppressed) {
			imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Happy);
		}

		// �̵� ������
		Begin();
		transform.Move(MoveMatrix, ASP(1.0) - 1.03 + ShiverValue2.x - VerticalSize * 0.5, -0.7 - VerticalSize * 0.5 + ShiverValue2.y - XionSize * 0.5);
		transform.Scale(MoveMatrix, 2.0 + VerticalSize, 2.0 - VerticalSize - XionSize);
		transform.Shear(MoveMatrix, -VerticalSize * 0.5, 0.0);

		if (Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion)
			imageUtil.RenderStaticSpriteSheet(Img.ED_GameOver, EDFrame);
		else {
			SetFlip(FLIP_TYPE_X);
			imageUtil.RenderStaticSpriteSheet(Img.ED, ED_Happy);
		}

		// Ŭ�ο� ������
		if (Glb.Ending == GameOver_TimeOut || Glb.Ending == GameOver_HitXion) {
			Begin();
			transform.Move(MoveMatrix, ASP(1.0) - 0.5 + ShiverValue2.x - VerticalSize * 0.5, -0.6 - VerticalSize * 0.5 + ShiverValue2.y);
			transform.Scale(MoveMatrix, 2.2 + VerticalSize, 2.2 - VerticalSize);
			transform.Shear(MoveMatrix, -VerticalSize * 0.5, 0.0);
			imageUtil.RenderStaticSpriteSheet(Img.Chloe, ChloeFrame);
		}

		if (!ShowRep) {
			// �ؽ�Ʈ ���
			ScoreText.SetColor(1.0, 1.0, 1.0);
			Text.Render(ASP(-1.0) + 0.8, 0.85, 0.2 + TextSize, Str.c_str());
			Text.Render(ASP(-1.0) + 0.8, 0.6, 0.1 + TextSize, Str2.c_str());

			// ���� ���
			ScoreText.Render(0.0, 0.4, 0.2, L"SCORE\n%d", Glb.Score);

			if (NewHighScore) {
				ScoreText.SetColorRGB(255, 213, 80);
				ScoreText.Render(0.0, 0.55, 0.1, L"HighScore!");
			}

			ScoreText.SetColor(1.0, 1.0, 1.0);
			ScoreText.SetAlign(ALIGN_LEFT);
			ScoreText.SetHeightAlign(HEIGHT_ALIGN_UNDER);
			ScoreText.Render(ASP(1.0) - 0.05, 0.95, 0.1, L"Enter�� ���� ���");
			ScoreText.SetAlign(ALIGN_MIDDLE);
			ScoreText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		}

		// ���� ���
		else {
			// �� ��� ���
			Begin();
			transform.Scale(MoveMatrix, ASP(2.0), 2.0);
			imageUtil.Render(SysRes.COLOR_TEXTURE, 0.6);

			// ���� ���
			GLfloat RenderHeight{ 0.8 };

			for (int i = 0; i < CurrentSize; i++) {
				ScoreText.SetColor(1.0, 1.0, 1.0);

				switch (RepVec[i].DestroyedType) {
				case Item_BoxCoffee:
					ScoreText.Render(0.0, RenderHeight, 0.1, L"Ŀ�� ���� ���: %d�� x 25000���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
					break;

				case Item_BottleCoffee:
					ScoreText.Render(0.0, RenderHeight, 0.1, L"�� Ŀ�� ���: %d�� x 2500���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
					break;

				case Item_CanCoffee:
					ScoreText.Render(0.0, RenderHeight, 0.1, L"ĵ Ŀ�� ���: %d�� x 1200���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
					break;

				case Item_People:
					ScoreText.Render(0.0, RenderHeight, 0.1, L"���� ���� ���Ǳ�: %d�� x 1500000���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
					break;

				case Item_Xion:
					ScoreText.Render(0.0, RenderHeight, 0.1, L"�ÿ� ġ���: 1000000���");
					break;

				case Item_HouseDoc:
					ScoreText.Render(0.0, RenderHeight, 0.1, L"�� ź ������ ���: %d�� x 100000���", Glb.DestroyedItems[RepVec[i].DestroyedType]);
					break;
				}

				RenderHeight -= 0.2;
			}

			if (ShowTotalRep) {
				ScoreText.SetColorRGB(255, 213, 80);
				ScoreText.Render(TextShake.x, -0.7 + TextShake.y, 0.15, L"�հ�: %d ���", TotalRep);

				if (NewHighRep)
					ScoreText.Render(TextShake.x, -0.5 + TextShake.y, 0.1, L"�����!!");

				ScoreText.SetColor(1.0, 1.0, 1.0);
				ScoreText.SetAlign(ALIGN_LEFT);
				ScoreText.SetHeightAlign(HEIGHT_ALIGN_DEFAULT);
				ScoreText.Render(ASP(1.0) - 0.05, -0.95, 0.1, L"Enter�� ���� ���");
				ScoreText.SetAlign(ALIGN_MIDDLE);
				ScoreText.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
			}
		}
	}
};