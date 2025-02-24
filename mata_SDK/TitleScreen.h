#pragma once
#include <Scene.h>
#include "Tutorial.h"

class TitleScreen : public GameObject {
private:
	// 인트로 끝 여부
	bool IntroEnd{};

	// true가 되어야 타이틀 화면 조작 가능
	bool RunningState{};

	bool TextRenderState{};

	// 메뉴 텍스트 및 현재 선택된 텍스트
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

	bool CreditStart{};

	bool MoveState = true;

	std::wstring MenuItems[3] = {
		L"시작!",
		L"환경설정",
		L"바탕화면으로\n나가기"
	};

	std::wstring QuestionItems[2] = {
		L"예",
		L"아니오"
	};

	std::wstring QuestionItems2[2] = {
		L"아니오",
		L"예"
	};

	GLfloat TextOpacity{};

	SoundChannel SndChannel{};

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

	glm::vec2 TitlePosition{ 0.0, 0.5 };
	GLfloat TitleSize{ 1.8 };

	bool TutorialState{};

public:
	TitleScreen(bool IntroFlag) {
		if (IntroFlag) {
			IntroEnd = true;
			TextRenderState = true;
			RunningState = true;
			TextOpacity = 1.0;
			TitlePosition.x = ASP(1.0) - 0.45;
			TitlePosition.y = -1.0 + 0.2;
			TitleSize = 1.0;
		}
		else 
			IntroEnd = false;

		MenuFocused[0] = true;
		SettingFocused[0] = true;
		QuestionFocused[0] = true;

		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetAlign(ALIGN_LEFT);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
		Text.SetShadow(0.1, glm::vec3(0.0, 0.0, 0.0), 0.8);
		Text.SetLineGap(0.02);

		soundUtil.Play(Snd.TitleBgm, Glb.BGMChannel);
		Glb.BGMChannel->setVolume(Glb.BGMVolume);
	}

	void InputKey(KeyEvent& Event) {
		if (!RunningState) {
			if (Event.Type == NORMAL_KEY_DOWN) {
				if (Event.NormalKey == NK_ENTER) {
					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);

					if (!QuestionToDesktop) {
						SndChannel->setVolume(Glb.SFXVolume);
						RunningState = true;
					}

					else {
						if (MenuIndex2 == 0)
							System.Exit();

						else if (MenuIndex2 == 1) {
							QuestionToDesktop = false;
							for (int i = 0; i < 2; i++)
								QuestionFocused[i] = false;
							QuestionFocused[0] = true;
							MenuIndex2 = 0;
						}
					}
				}

				else if (Event.NormalKey == NK_ESCAPE) {
					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);
					SndChannel->setVolume(Glb.SFXVolume);

					if (!QuestionToDesktop)
						QuestionToDesktop = true;

					else {
						QuestionToDesktop = false;
						for (int i = 0; i < 2; i++)
							QuestionFocused[i] = false;
						QuestionFocused[0] = true;
						MenuIndex2 = 0;
					}
				}
			}

			else if (Event.Type == SPECIAL_KEY_DOWN) {
				if (!QuestionToDesktop)
					return;

				switch (Event.SpecialKey) {
				case SK_ARROW_UP:
					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);
					SndChannel->setVolume(Glb.SFXVolume);

					MenuIndex2--;

					break;

				case SK_ARROW_DOWN:
					soundUtil.Stop(SndChannel);
					soundUtil.Play(Snd.MenuSelect, SndChannel);
					SndChannel->setVolume(Glb.SFXVolume);

					MenuIndex2++;

					break;
				}

				EX.ClampValue(MenuIndex2, 0, 1, CLAMP_RETURN);

				for (int i = 0; i < 2; i++)
					QuestionFocused[i] = false;
				QuestionFocused[MenuIndex2] = true;
			}
		}

		if (!TextRenderState)
			return;

		else {
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
							//	Glb.BGMVolumeInt--;
							EX.ClampValue(Glb.BGMVolume, 0.0, CLAMP_LESS);
							//	EX.ClampValue(Glb.BGMVolumeInt, 0, CLAMP_LESS);
							Dat.UserSettingData.UpdateDigitData("Setting", "BGMVolume", Glb.BGMVolume);
							Glb.BGMChannel->setVolume(Glb.BGMVolume);
							break;
						case 3:
							Glb.SFXVolume -= 0.1;
							//Glb.SFXVolumeInt--;
							EX.ClampValue(Glb.SFXVolume, 0.0, CLAMP_LESS);
							//EX.ClampValue(Glb.SFXVolumeInt, 0, CLAMP_LESS);
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
							//	Glb.BGMVolumeInt++;
							EX.ClampValue(Glb.BGMVolume, 1.0, CLAMP_GREATER);
							//	EX.ClampValue(Glb.BGMVolumeInt, 10, CLAMP_GREATER);
							Dat.UserSettingData.UpdateDigitData("Setting", "BGMVolume", Glb.BGMVolume);
							Glb.BGMChannel->setVolume(Glb.BGMVolume);
							break;
						case 3:
							Glb.SFXVolume += 0.1;
							//	Glb.SFXVolumeInt++;
							EX.ClampValue(Glb.SFXVolume, 1.0, CLAMP_GREATER);
							//EX.ClampValue(Glb.SFXVolumeInt, 10, CLAMP_GREATER);
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
				if (SettingState && !QuestionReset)
					EX.ClampValue(MenuIndex2, 0, 5, CLAMP_RETURN);
				else if (QuestionToDesktop)
					EX.ClampValue(MenuIndex2, 0, 1, CLAMP_RETURN);
				else if (SettingState && QuestionReset)
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
							if (!Glb.NeedTutorial) {
								soundUtil.Stop(Glb.BGMChannel);
								soundUtil.Play(Snd.CartCrash, SndChannel);
								SndChannel->setVolume(Glb.SFXVolume);
								GameStart = true;
								scene.DeleteInputObject(this);
							}

							// 튜토리얼이 필요한 상황일 경우 튜토리얼 객체를 추가한다
							else {
								soundUtil.Stop(Glb.BGMChannel);
								soundUtil.Play(Snd.MenuSelect, SndChannel);
								SndChannel->setVolume(Glb.SFXVolume);
								scene.DeleteInputObject(this);
								scene.AddObject(new Tutorial, "tutorial", LAYER7);
								scene.AddInputObject(scene.Find("tutorial"));
								TutorialState = true;
							}

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

						case 5:
							CreditStart = true;
							soundUtil.Stop(Glb.BGMChannel);
							scene.DeleteInputObject(this);
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
							Glb.NeedTutorial = true;

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
	}

	void UpdateFunc(float FrameTime) {
		// 화면이 암전된 후 1초 후 플레이 모드로 전환한다
		if (GameStart) {
			StartTimer.Update(FrameTime);
			if (StartTimer.Sec() >= 1)
				scene.SwitchMode(PlayModePtr);
		}

		if (CreditStart) {
			StartTimer.Update(FrameTime);
			if (StartTimer.Sec() >= 1)
				scene.SwitchMode(CreditModePtr);
		}

		if (!IntroEnd) {
			if (!GameStart) {
				if (RunningState) {
					mathUtil.Lerp(TitleCameraZoom, 1.5, 0.4, FrameTime);
					mathUtil.Lerp(TitleCameraPosition, 0.7, 0.4, FrameTime);

					mathUtil.Lerp(TitlePosition.x, ASP(1.0) - 0.45, 5.0, FrameTime);
					mathUtil.Lerp(TitlePosition.y, -1.0 + 0.2, 5.0, FrameTime);
					mathUtil.Lerp(TitleSize, 1.0, 5.0, FrameTime);
				}

				if (TitleCameraPosition > 0.3)
					TextRenderState = true;
			}	
		}
		else {
			TitleCameraPosition = 0.7;
			TitleCameraZoom = 1.5;
		}

		if (RunningState) {
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
		}


		cameraControl.MoveCamera(TitleCameraPosition + TitleCameraPosition2, 0.4 + TitleCameraHeight);
		cameraControl.ChangeCameraZoom(TitleCameraZoom + TitleCameraZoom2);

		if(GameStart)
			cameraControl.ChangeCameraZoom(1.0);

		if(TextRenderState) {
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
		if (TutorialState)
			return;

		if (GameStart || CreditStart) {
			Begin(RENDER_TYPE_STATIC);
			transform.Scale(MoveMatrix, ASP(2.0), 2.0);
			imageUtil.Render(SysRes.COLOR_TEXTURE);
		}

		else {
			// 시온
			Begin();
			transform.Move(MoveMatrix, -1.3, XionSize * 0.5);
			transform.Scale(MoveMatrix, 2.0, 2.0 + XionSize);
			imageUtil.RenderStaticSpriteSheet(Img.Xion, Xion_Nervous);

			// 이드
			Begin();
			transform.Move(MoveMatrix, ShakePosition.x, EDSize * 0.5 + ShakePosition.y);
			transform.Scale(MoveMatrix, 2.0, 2.0 + EDSize);
			imageUtil.RenderStaticSpriteSheet(Img.ED, ED_Title);

			// 카트
			Begin();
			transform.Move(MoveMatrix, 0.7, -0.3);
			transform.Scale(MoveMatrix, 1.5, 1.5);
			imageUtil.RenderStaticSpriteSheet(Img.Cart, 0);

			if (TextRenderState) {
				Text.SetOpacity(TextOpacity);

				// 메뉴 표시
				if (!SettingState && !QuestionToDesktop) {
					// 뒷 배경 렌더링
					Begin(RENDER_TYPE_STATIC);
					transform.Move(MoveMatrix, ASP(1.0), 0.0);
					transform.Scale(MoveMatrix, 1.8, 2.0);
					imageUtil.Render(SysRes.COLOR_TEXTURE, 0.3 * TextOpacity);

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


					// 최고 점수 또는 최고 금액을 갱신했을 경우 초록색으로 표시한다
					Text.SetColor(1.0, 1.0, 1.0);

					if (Glb.IsHighScore)
						Text.SetColor(0.0, 1.0, 0.0);
					if (Glb.HighScore > 0)
						Text.Render(ASP(1.0) - 0.1, 0.85, 0.08, L"최고 점수\n%d", Glb.HighScore);
					else
						Text.Render(ASP(1.0) - 0.1, 0.85, 0.08, L"최고 점수\n-", Glb.HighScore);

					Text.SetColor(1.0, 1.0, 1.0);

					if (Glb.IsHighRep)
						Text.SetColor(0.0, 1.0, 0.0);
					if (Glb.MaxRep > 0)
						Text.Render(ASP(1.0) - 0.1, 0.6, 0.08, L"최고 금액\n%d 골드", Glb.MaxRep);
					else
						Text.Render(ASP(1.0) - 0.1, 0.6, 0.08, L"최고 금액\n- 골드", Glb.MaxRep);
				}
			}

			if (SettingState && !QuestionReset) {
				Begin(RENDER_TYPE_STATIC);
				transform.Scale(MoveMatrix, ASP(2.0), 2.0);
				imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

				Text.SetUseShadow(false);
				Text.SetColor(1.0, 1.0, 1.0);
				Text.SetAlign(ALIGN_MIDDLE);
				Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
				Text.Render(0.0, 0.8, 0.15, L"환경설정");

				Text.SetAlign(ALIGN_LEFT);
				for (int i = 0; i < 6; i++) {
					if (SettingFocused[i])
						Text.SetColorRGB(255, 213, 80);
					else
						Text.SetColor(1.0, 1.0, 1.0);

					switch (i) {
					case 0:
						Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"타이틀로 돌아가기");
						break;
					case 1:
						if (Glb.FullscreenAcvivated)
							Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"화면 모드: 전체화면");
						else
							Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"화면 모드: 창");
						break;
					case 2:
						Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"배경음악 볼륨: %.1f", Glb.BGMVolume);
						break;
					case 3:
						Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"효과음 볼륨: %.1f", Glb.SFXVolume);
						break;
					case 4:
						Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"진행 상황 초기화");
						break;
					case 5:
						Text.Render(ASP(1.0) - 0.1, 0.625 - i * 0.25, 0.1, L"크래딧");
						break;
					}
				}

				if (0 < MenuIndex2 && MenuIndex2 < 4) {
					Text.SetHeightAlign(HEIGHT_ALIGN_DEFAULT);
					Text.SetAlign(ALIGN_DEFAULT);
					Text.Render(ASP(-1.0) + 0.1, -0.9, 0.08, L"좌 우 방향키로 설정");
				}

			}

			if (QuestionToDesktop) {
				Begin(RENDER_TYPE_STATIC);
				transform.Scale(MoveMatrix, ASP(2.0), 2.0);
				imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

				Text.SetUseShadow(false);
				Text.SetColor(1.0, 1.0, 1.0);
				Text.SetAlign(ALIGN_MIDDLE);
				Text.Render(0.0, 0.8, 0.15, L"바탕화면으로 나갈까요?");

				Text.SetAlign(ALIGN_LEFT);
				for (int i = 0; i < 2; i++) {
					if (QuestionFocused[i])
						Text.SetColorRGB(255, 213, 80);
					else
						Text.SetColor(1.0, 1.0, 1.0);

					Text.Render(ASP(1.0) - 0.1, 0.125 - i * 0.25, 0.1, QuestionItems[i].c_str());
				}
			}

			if (QuestionReset) {
				Begin(RENDER_TYPE_STATIC);
				transform.Scale(MoveMatrix, ASP(2.0), 2.0);
				imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

				Text.SetUseShadow(false);
				Text.SetColor(1.0, 1.0, 1.0);
				Text.SetAlign(ALIGN_MIDDLE);
				Text.Render(0.0, 0.8, 0.15, L"정말인가요?");

				Text.SetAlign(ALIGN_LEFT);
				for (int i = 0; i < 2; i++) {
					if (QuestionFocused[i])
						Text.SetColorRGB(255, 213, 80);
					else
						Text.SetColor(1.0, 1.0, 1.0);

					Text.Render(ASP(1.0) - 0.1, 0.125 - i * 0.25, 0.1, QuestionItems2[i].c_str());
				}
			}

			// 타이틀
			if (!SettingState && !QuestionToDesktop) {
				Begin(RENDER_TYPE_STATIC);
				transform.Move(MoveMatrix, TitlePosition);
				transform.Scale(MoveMatrix, TitleSize, TitleSize);
				imageUtil.Render(Img.Title);
			}
		}

		if (!RunningState && !QuestionToDesktop) {
			Text.SetAlign(ALIGN_MIDDLE);
			Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);
			Text.SetUseShadow(true);
			Text.SetColorRGB(255, 213, 80);
			Text.Render(0.0, -0.7, 0.1, L"Enter를 눌러 계속");
			Text.SetUseShadow(false);
		}
	}
};