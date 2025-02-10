#pragma once
#include <Scene.h>

class Xion : public GameObject {
private:
	// 현재 위치
	glm::vec2 Position{};

	// 목표 이동 위치
	GLfloat DestPosition{};

	// 수직 루프 애니메이션 크기
	GLfloat VerticalSize{};

	// 루프 애니메이션
	SinLoop VerticalLoop{};

	// 기울임 값
	GLfloat TiltValue{};

	// 기울임 루프 애니메이션
	SinLoop TiltLoop{};
	
	// 떨림 위치
	glm::vec2 ShakeValue{};

	// 떨림 타이머
	TimerUtil ShakeTimer{};

	// 회전값
	GLfloat Rotation{};

	// 수평 회전값
	GLfloat HRotation{};
	GLfloat DestHRotation{};

	// 움직이는 상태
	bool MoveState{};

	// 이드에게 차인 상태
	bool HitState{};

	// 넘어진 상태
	bool FellDown{};

	// 밀리는 상태
	bool PushState{};
	
	// 프레임
	int Frame{};

	SoundChannel SndChannel{};

	// 게임오버 딜레이 타이머
	TimerUtil GameOverTimer{};

public:
	// MoveState가 활성화된 상태로 생성되면 지정된 위치로 이동한다.
	// MoveState가 비활성화된 상태로 생성되면 이동하지 않는다.
	Xion(GLfloat PositionValue, GLfloat DestPositionValue, bool BoolMoveState, int FrameValue);
	void UpdateFunc(float FrameTime);
	void RenderFunc();
	void HitPeople();
	void PushPeople();
	void SetFrame(int Value);
};