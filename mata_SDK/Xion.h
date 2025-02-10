#pragma once
#include <Scene.h>

class Xion : public GameObject {
private:
	// 현재 위치
	GLfloat Position{};

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

	// 움직이는 상태
	bool MoveState{};

	int Frame{};

public:
	// MoveState가 활성화된 상태로 생성되면 지정된 위치로 이동한다.
	// MoveState가 비활성화된 상태로 생성되면 이동하지 않는다.
	Xion(GLfloat PositionValue, bool BoolMoveState, int FrameValue);
	void UpdateFunc(float FrameTime);
	void RenderFunc();
};