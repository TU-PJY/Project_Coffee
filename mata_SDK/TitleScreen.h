#pragma once
#include <Scene.h>

class TitleScreen : public GameObject {
private:
	// ��Ʈ�� �� ����
	bool IntroEnd{};

	bool RenderText{};

	// �޴� �ؽ�Ʈ �� ���� ���õ� �ؽ�Ʈ
	TextUtil Text{};
	bool MenuFocused[3]{};
	int MenuIndex{};

	bool QuestionToDesktop{};
	bool QuestionReset{};
	bool QuestionFocused[2]{};

	bool SettingState{};
	bool SettingFocused[6]{};
	int MenuIndex2{};
	int MenuIndex3{};

	TimerUtil StartTimer{};
	bool GameStart{};

	bool MoveState = true;

	std::wstring MenuItems[3] = {
		L"����!",
		L"ȯ�漳��",
		L"����ȭ������\n������"
	};

	std::wstring QuestionItems[2] = {
		L"��",
		L"�ƴϿ�"
	};

	std::wstring QuestionItems2[2] = {
		L"�ƴϿ�",
		L"��"
	};

	GLfloat TextOpacity{};

	SoundChannel SndChannel{};

	TimerUtil MoveDelayTimer{};
	GLfloat TitleCameraPosition = -1.3;
	GLfloat TitleCameraZoom = 2.0;

	GLfloat TitleCameraPosition2{};
	GLfloat TitleCameraZoom2{};
	GLfloat TitleCameraHeight{};

	SinLoop XionLoop{};
	SinLoop EDLoop{};
	GLfloat XionSize{};
	GLfloat EDSize{};
	TimerUtil ShakeTimer{};
	glm::vec2 ShakePosition{};

public:
	TitleScreen(bool IntroFlag) {
		if (IntroFlag) {
			IntroEnd = true;
			RenderText = true;
			TextOpacity = 1.0;
		}
		else
			IntroEnd = false;

		MenuFocused[0] = true;
		SettingFocused[0] = true;
		QuestionFocused[0] = true;

		Text.Init(L"�ȼ��κ��κ�ü", FW_DONTCARE);
		Text.SetAlign(ALIGN_LEFT);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.8);
		Text.SetLineGap(0.01);

		soundUtil.Play(Snd.TitleBgm, Glb.BGMChannel);
		Glb.BGMChannel->setVolume(Glb.BGMVolume);
	}

	void InputKey(KeyEvent& Event) {
		if (!RenderText)
			return;

		if (Event.Type == SPECIAL_KEY_DOWN) {
			switch (Event.SpecialKey) {
			case SK_ARROW_UP:
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);
				SndChannel->setVolume(Glb.SFXVolume);

				if (!SettingState && !QuestionToDesktop)
					MenuIndex--;
				else if ((SettingState && !QuestionReset) || QuestionToDesktop)
					MenuIndex2--;
				else if (SettingState && QuestionReset)
					MenuIndex3--;
				break;

			case SK_ARROW_DOWN:
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);
				SndChannel->setVolume(Glb.SFXVolume);

				if (!SettingState && !QuestionToDesktop)
					MenuIndex++;
				else if ((SettingState && !QuestionReset) || QuestionToDesktop)
					MenuIndex2++;
				else if (SettingState && QuestionReset)
					MenuIndex3++;
				break;

			case SK_ARROW_LEFT:
				if (SettingState && 0 < MenuIndex2 && MenuIndex2 < 4) {
					switch (MenuIndex2) {
					case 1:
						EX.SwitchBool(Glb.FullscreenAcvivated);
						Dat.UserSettingData.UpdateDigitData("Setting", "Fullscreen", (int)Glb.FullscreenAcvivated);
						System.SwitchScreenState();
						break;
					case 2:
						Glb.BGMVolume -= 0.1;
						EX.ClampValue(Glb.BGMVolume, 0.0, CLAMP_LESS);
						Dat.UserSettingData.UpdateDigitData("Setting", "BGMVolume", Glb.BGMVolume);
						Glb.BGMChannel->setVolume(Glb.BGMVolume);
						break;
					case 3:
						Glb.SFXVolume -= 0.1;
						EX.ClampValue(Glb.SFXVolume, 0.0, CLAMP_LESS);
						Dat.UserSettingData.UpdateDigitData("Setting", "SFXVolume", Glb.SFXVolume);
						break;
					}

					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);
					SndChannel->setVolume(Glb.SFXVolume);
				}
				break;

			case SK_ARROW_RIGHT:
				if (SettingState && 0 < MenuIndex2 && MenuIndex2 < 4) {
					switch (MenuIndex2) {
					case 1:
						EX.SwitchBool(Glb.FullscreenAcvivated);
						Dat.UserSettingData.UpdateDigitData("Setting", "Fullscreen", (int)Glb.FullscreenAcvivated);
						System.SwitchScreenState();
						break;
					case 2:
						Glb.BGMVolume += 0.1;
						EX.ClampValue(Glb.BGMVolume, 1.0, CLAMP_GREATER);
						Dat.UserSettingData.UpdateDigitData("Setting", "BGMVolume", Glb.BGMVolume);
						Glb.BGMChannel->setVolume(Glb.BGMVolume);
						break;
					case 3:
						Glb.SFXVolume += 0.1;
						EX.ClampValue(Glb.SFXVolume, 1.0, CLAMP_GREATER);
						Dat.UserSettingData.UpdateDigitData("Setting", "SFXVolume", Glb.SFXVolume);
						break;
					}

					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);
					SndChannel->setVolume(Glb.SFXVolume);
				}
				break;
			}

			EX.ClampValue(MenuIndex, 0, 2, CLAMP_RETURN);
			if(SettingState && !QuestionReset)
				EX.ClampValue(MenuIndex2, 0, 5, CLAMP_RETURN);
			else if(QuestionToDesktop)
				EX.ClampValue(MenuIndex2, 0, 1, CLAMP_RETURN);
			else if(SettingState && QuestionReset)
				EX.ClampValue(MenuIndex3, 0, 1, CLAMP_RETURN);

			for (int i = 0; i < 3; i++)
				MenuFocused[i] = false;

			if (SettingState && !QuestionReset) {
				for (int i = 0; i < 6; i++)
					SettingFocused[i] = false;
				SettingFocused[MenuIndex2] = true;
			}

			else if (QuestionToDesktop) {
				for (int i = 0; i < 2; i++)
					QuestionFocused[i] = false;
				QuestionFocused[MenuIndex2] = true;
			}

			else if (SettingState && QuestionReset) {
				for (int i = 0; i < 2; i++)
					QuestionFocused[i] = false;
				QuestionFocused[MenuIndex3] = true;
			}

			MenuFocused[MenuIndex] = true;
		}

		else if (Event.Type == NORMAL_KEY_DOWN) {
			switch (Event.NormalKey) {
			case NK_ENTER:
				if (!SettingState && !QuestionToDesktop) {
					soundUtil.Stop(SndChannel);
					if (MenuIndex == 0) {
						soundUtil.Stop(Glb.BGMChannel);
						soundUtil.Play(Snd.CartCrash, SndChannel);
						SndChannel->setVolume(Glb.SFXVolume);
						GameStart = true;
						scene.DeleteInputObject(this);
					}

					else {
						soundUtil.Play(Snd.MenuSelect, SndChannel);
						SndChannel->setVolume(Glb.SFXVolume);
					}

					switch (MenuIndex) {
					case 0:
						break;
					case 1:
						SettingState = true;
						break;
					case 2:
						QuestionToDesktop = true;
						break;
					}
					break;
				}

				if (SettingState && !QuestionReset) {
					if (MenuIndex2 == 0 || MenuIndex2 == 4 || MenuIndex2 == 5) {
						soundUtil.Stop(SndChannel);
						soundUtil.Play(Snd.MenuSelect, SndChannel);
						SndChannel->setVolume(Glb.SFXVolume);
					}

					switch (MenuIndex2) {
					case 0:
						SettingState = false;
						for (int i = 0; i < 6; i++)
							SettingFocused[i] = false;
						SettingFocused[0] = true;
						MenuIndex2 = 0;
						break;

					case 4:
						QuestionReset = true;
						break;
					}
				}

				else if (QuestionToDesktop) {
					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);
					SndChannel->setVolume(Glb.SFXVolume);

					switch (MenuIndex2) {
					case 0:
						System.Exit();
						break;
					case 1:
						QuestionToDesktop = false;
						QuestionFocused[1] = false;
						QuestionFocused[0] = true;
						MenuIndex2 = 0;
						break;
					}
				}

				else if (SettingState && QuestionReset) {
					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);
					SndChannel->setVolume(Glb.SFXVolume);

					switch (MenuIndex3) {
					case 0:
						QuestionReset = false;
						QuestionFocused[1] = false;
						QuestionFocused[0] = true;
						MenuIndex3 = 0;
						break;

					case 1:
						Dat.HighscoreData.ResetData();
						Glb.HighScore = 0;
						Glb.MaxRep = 0;

						QuestionReset = false;
						QuestionFocused[1] = false;
						QuestionFocused[0] = true;
						MenuIndex3 = 0;
						break;
					}
				}
				break;

			case NK_ESCAPE:
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);
				SndChannel->setVolume(Glb.SFXVolume);

				if (!SettingState && !QuestionToDesktop) 
					QuestionToDesktop = true;

				else if (QuestionToDesktop) {
					QuestionToDesktop = false;
					QuestionFocused[1] = false;
					QuestionFocused[0] = true;
					MenuIndex2 = 0;
				}

				else if (SettingState && !QuestionReset) {
					SettingState = false;
					for (int i = 0; i < 6; i++)
						SettingFocused[i] = false;
					SettingFocused[0] = true;
					MenuIndex2 = 0;
				}

				else if (SettingState && QuestionReset) {
					QuestionReset = false;
					QuestionFocused[1] = false;
					QuestionFocused[0] = true;
					MenuIndex3 = 0;
				}
				break;
			}
		}
	}

	void UpdateFunc(float FrameTime) {
		// ȭ���� ������ �� 2�� �� �÷��� ���� ��ȯ�Ѵ�
		if (GameStart) {
			StartTimer.Update(FrameTime);
			if (StartTimer.MiliSec() >= 1)
				scene.SwitchMode(PlayModePtr);
		}

		if (!IntroEnd) {
			if (!GameStart) {
				MoveDelayTimer.Update(FrameTime);
				if (MoveDelayTimer.Sec() >= 3) {
					mathUtil.Lerp(TitleCameraPosition, 0.7, 0.4, FrameTime);
					mathUtil.Lerp(TitleCameraZoom, 1.5, 0.4, FrameTime);
				}

				if (TitleCameraPosition > 0.3)
					RenderText = true;
			}	
		}
		else {
			TitleCameraPosition = 0.7;
			TitleCameraZoom = 1.5;
		}

		if (SettingState) {
			mathUtil.Lerp(TitleCameraPosition2, -2.0, 2.0, FrameTime);
			mathUtil.Lerp(TitleCameraZoom2, 0.0, 2.0, FrameTime);
			mathUtil.Lerp(TitleCameraHeight, 0.0, 2.0, FrameTime);
		}
		else if (QuestionToDesktop) {
			mathUtil.Lerp(TitleCameraPosition2, 0.3, 2.0, FrameTime);
			mathUtil.Lerp(TitleCameraZoom2, 3.0, 2.0, FrameTime);
			mathUtil.Lerp(TitleCameraHeight, -0.25, 2.0, FrameTime);
		}
		else {
			mathUtil.Lerp(TitleCameraPosition2, 0.0, 2.0, FrameTime);
			mathUtil.Lerp(TitleCameraZoom2, 0.0, 2.0, FrameTime);
			mathUtil.Lerp(TitleCameraHeight, 0.0, 2.0, FrameTime);
		}


		cameraControl.MoveCamera(TitleCameraPosition + TitleCameraPosition2, 0.4 + TitleCameraHeight);
		cameraControl.ChangeCameraZoom(TitleCameraZoom + TitleCameraZoom2);

		if(GameStart)
			cameraControl.ChangeCameraZoom(1.0);

		if(RenderText) {
			TextOpacity += FrameTime;
			EX.ClampValue(TextOpacity, 1.0, CLAMP_GREATER);
		}

		XionSize = XionLoop.Update(0.03, 15.0, FrameTime);
	//	EDSize = EDLoop.Update(0.03, 4.0, FrameTime);
		ShakeTimer.Update(FrameTime);
		if (ShakeTimer.CheckMiliSec(0.02, 2, CHECK_AND_INTERPOLATE)) {
			ShakePosition.x = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
			ShakePosition.y = randomUtil.Gen(RANDOM_TYPE_REAL, -0.01, 0.01);
		}
	}

	void RenderFunc() {
		if (GameStart) {
			Begin(RENDER_TYPE_STATIC);
			transform.Scale(MoveMatrix, ASP(2.0), 2.0);
			imageUtil.Render(SysRes.COLOR_TEXTURE);
		}

		else {
			// �ÿ�
			Begin();
			transform.Move(MoveMatrix, -1.3, XionSize * 0.5);
			transform.Scale(MoveMatrix, 2.0, 2.0 + XionSize);
			imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Nervous);

			// �̵�
			Begin();
			transform.Move(MoveMatrix, ShakePosition.x, EDSize * 0.5 + ShakePosition.y);
			transform.Scale(MoveMatrix, 2.0, 2.0 + EDSize);
			imageUtil.RenderStaticSpriteSheet(Img.ED, ED_Title);

			// īƮ
			Begin();
			transform.Move(MoveMatrix, 0.7, -0.3);
			transform.Scale(MoveMatrix, 1.5, 1.5);
			imageUtil.RenderStaticSpriteSheet(Img.Cart, 0);

			if (RenderText) {
				Text.SetOpacity(TextOpacity);

				// �޴� ǥ��
				if (!SettingState && !QuestionToDesktop) {
					Text.SetUseShadow(true);
					Text.SetAlign(ALIGN_LEFT);
					Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);

					for (int i = 0; i < 3; i++) {
						if (MenuFocused[i])
							Text.SetColorRGB(255, 213, 80);
						else
							Text.SetColor(1.0, 1.0, 1.0);

						Text.Render(ASP(1.0) - 0.1, 0.25 - 0.25 * i, 0.1, MenuItems[i].c_str());
					}

					Text.SetColor(1.0, 1.0, 1.0);
					if(Glb.HighScore > 0)
						Text.Render(ASP(1.0) - 0.1, 0.75, 0.08, L"�ְ� ����\n%d", Glb.HighScore);
					else
						Text.Render(ASP(1.0) - 0.1, 0.75, 0.08, L"�ְ� ����\n-", Glb.HighScore);

					if(Glb.MaxRep > 0)
						Text.Render(ASP(1.0) - 0.1, 0.55, 0.08, L"�ְ� �ݾ�\n%d", Glb.MaxRep);
					else
						Text.Render(ASP(1.0) - 0.1, 0.55, 0.08, L"�ְ� �ݾ�\n-", Glb.MaxRep);
				}

				else if (SettingState && !QuestionReset) {
					Begin(RENDER_TYPE_STATIC);
					transform.Scale(MoveMatrix, ASP(2.0), 2.0);
					imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

					Text.SetUseShadow(false);
					Text.SetColor(1.0, 1.0, 1.0);
					Text.SetAlign(ALIGN_MIDDLE);
					Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
					Text.Render(0.0, 0.8, 0.15, L"ȯ�漳��");

					Text.SetAlign(ALIGN_LEFT);
					for (int i = 0; i < 6; i++) {
						if (SettingFocused[i])
							Text.SetColorRGB(255, 213, 80);
						else
							Text.SetColor(1.0, 1.0, 1.0);

						switch (i) {
						case 0:
							Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"Ÿ��Ʋ�� ���ư���");
							break;
						case 1:
							if (Glb.FullscreenAcvivated)
								Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"ȭ�� ���: ��üȭ��");
							else
								Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"ȭ�� ���: â");
							break;
						case 2:
							Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"������� ����: %.1f", Glb.BGMVolume);
							break;
						case 3:
							Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"ȿ���� ����: %.1f", Glb.SFXVolume);
							break;
						case 4:
							Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"���� ��Ȳ �ʱ�ȭ");
							break;
						case 5:
							Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"ũ����");
							break;
						}
					}

					if (0 < MenuIndex2 && MenuIndex2 < 4) {
						Text.SetHeightAlign(HEIGHT_ALIGN_DEFAULT);
						Text.SetAlign(ALIGN_DEFAULT);
						Text.Render(ASP(-1.0) + 0.1, -0.9, 0.08, L"�� �� ����Ű�� ����");
					}

				}

				else if (QuestionToDesktop) {
					Begin(RENDER_TYPE_STATIC);
					transform.Scale(MoveMatrix, ASP(2.0), 2.0);
					imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

					Text.SetUseShadow(false);
					Text.SetColor(1.0, 1.0, 1.0);
					Text.SetAlign(ALIGN_MIDDLE);
					Text.Render(0.0, 0.8, 0.15, L"����ȭ������ �������?");

					Text.SetAlign(ALIGN_LEFT);
					for (int i = 0; i < 2; i++) {
						if (QuestionFocused[i])
							Text.SetColorRGB(255, 213, 80);
						else
							Text.SetColor(1.0, 1.0, 1.0);

						Text.Render(ASP(1.0) - 0.1, 0.125 - i * 0.25, 0.1, QuestionItems[i].c_str());
					}
				}

				else if (QuestionReset) {
					Begin(RENDER_TYPE_STATIC);
					transform.Scale(MoveMatrix, ASP(2.0), 2.0);
					imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

					Text.SetUseShadow(false);
					Text.SetColor(1.0, 1.0, 1.0);
					Text.SetAlign(ALIGN_MIDDLE);
					Text.Render(0.0, 0.8, 0.15, L"�����ΰ���?");

					Text.SetAlign(ALIGN_LEFT);
					for (int i = 0; i < 2; i++) {
						if (QuestionFocused[i])
							Text.SetColorRGB(255, 213, 80);
						else
							Text.SetColor(1.0, 1.0, 1.0);

						Text.Render(ASP(1.0) - 0.1, 0.125 - i * 0.25, 0.1, QuestionItems2[i].c_str());
					}
				}
			}
		}

		//RenderStart = true;
	}
};