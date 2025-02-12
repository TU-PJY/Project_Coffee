#pragma once
#include <Scene.h>

class PauseScreen : public GameObject {
private:
	TextUtil Text{};

	// 현재 가리키는 항목 인덱스
	int MenuIndex{};

	// 묻기 메뉴 항목 인덱스
	int QuestionIndex{};

	// 묻기 상태
	bool QuestionState{};

	// 바탕화면으로 나가기 묻기
	bool QuestionToDesktop{};

	// 메인화면으로 나가기 묻기
	bool QuestionToMain{};

	// 메뉴 항목
	std::wstring MenuItems[3] = {
		L"계속하기",
		L"메인화면으로 나가기",
		L"바탕화면으로 나가기"
	};

	bool MenuFocused[3]{};

	std::wstring QuestionItems[2] = {
		L"예",
		L"아니오"
	};

	bool QuestionFocused[2]{};

	SoundChannel SndChannel{};

public:
	PauseScreen() {
		Text.Init(L"픽셀로보로보체", FW_DONTCARE);
		Text.SetColor(1.0, 1.0, 1.0);
		Text.SetAlign(ALIGN_MIDDLE);
		Text.SetHeightAlign(HEIGHT_ALIGN_MIDDLE);

		soundUtil.Play(Snd.MenuSelect, SndChannel);
		soundUtil.SetFreqCutOff(Glb.BGMChannel, 200);

		MenuFocused[0] = true;
	}

	~PauseScreen() {
		soundUtil.DisableFreqCutOff(Glb.BGMChannel);
	}

	void InputKey(KeyEvent& Event) {
		if (Event.Type == NORMAL_KEY_DOWN) {
			if (Event.NormalKey == NK_ESCAPE) {
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);

				if (!QuestionState)
					scene.EndFloatingMode();
				else {
					QuestionState = false;
					QuestionToDesktop = false;
					QuestionToMain = false;
					QuestionIndex = 0;
					QuestionFocused[0] = true;
					QuestionFocused[1] = false;
				}
			}

			else if (Event.NormalKey == NK_ENTER) {
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);

				if (!QuestionState) {
					switch (MenuIndex) {
					case 0:
						scene.EndFloatingMode();
						break;
					case 1:
						QuestionState = true;
						QuestionToMain = true;
						break;
					case 2:
						QuestionState = true;
						QuestionToDesktop = true;
						break;
					}
				}

				else {
					switch (QuestionIndex) {
					case 0:
						if (QuestionToMain) 
						{}
						else if (QuestionToDesktop)
							System.Exit();
						break;

					case 1:
						QuestionState = false;
						QuestionIndex = 0;
						QuestionFocused[0] = true;
						QuestionFocused[1] = false;

						if (QuestionToMain) 
							QuestionToMain = false;

						else if (QuestionToDesktop) 
							QuestionToDesktop = false;
						break;
					}
				}
			}
		}

		else if (Event.Type == SPECIAL_KEY_DOWN) {
			if (Event.SpecialKey == SK_ARROW_UP) {
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);

				if (!QuestionState)  MenuIndex--;
				else  QuestionIndex--;
			}

			else if (Event.SpecialKey == SK_ARROW_DOWN) {
				soundUtil.Stop(SndChannel);
				soundUtil.Play(Snd.MenuSelect, SndChannel);

				if (!QuestionState)  MenuIndex++;
				else  QuestionIndex++;
			}

			EX.ClampValue(MenuIndex, 0, 2, CLAMP_RETURN);
			EX.ClampValue(QuestionIndex, 0, 1, CLAMP_RETURN);

			for (int i = 0; i < 3; i++)  MenuFocused[i] = false;
			for (int i = 0; i < 2; i++)  QuestionFocused[i] = false;

			MenuFocused[MenuIndex] = true;
			QuestionFocused[QuestionIndex] = true;
		}
	}

	void RenderFunc() {
		// 뒷배경 
		Begin(RENDER_TYPE_STATIC);
		transform.Scale(MoveMatrix, ASP(2.0), 2.0);
		imageUtil.Render(SysRes.COLOR_TEXTURE, 0.7);

		// 텍스트 출력
		if (!QuestionState) {
			Text.SetColor(1.0, 1.0, 1.0);
			Text.Render(0.0, 0.8, 0.15, L"일시정지");

			GLfloat RenderHeight = 0.25;
			for (int i = 0; i < 3; i++) {
				// 선택된 항목은 노란색으로 표시된다
				if (MenuFocused[i])
					Text.SetColorRGB(255, 213, 80);
				else
					Text.SetColor(1.0, 1.0, 1.0);

				Text.Render(0.0, RenderHeight, 0.1, MenuItems[i].c_str());
				RenderHeight -= 0.25;
			}
		}

		else {
			Text.SetColor(1.0, 1.0, 1.0);
			if (QuestionToMain) 
				Text.Render(0.0, 0.8, 0.15, L"메인화면으로 나갈까요?");
			else if(QuestionToDesktop)
				Text.Render(0.0, 0.8, 0.15, L" 바탕화면으로 나갈까요?");

			GLfloat RenderHeight = 0.125;
			for (int i = 0; i < 2; i++) {
				if(QuestionFocused[i])
					Text.SetColorRGB(255, 213, 80);
				else
					Text.SetColor(1.0, 1.0, 1.0);

				Text.Render(0.0, RenderHeight, 0.1, QuestionItems[i].c_str());
				RenderHeight -= 0.25;
			}
		}
	}
};