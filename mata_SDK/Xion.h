#pragma once
#include <Scene.h>

class Xion : public GameObject {
private:
	// ���� ��ġ
	GLfloat Position{};

	// ��ǥ �̵� ��ġ
	GLfloat DestPosition{};

	// ���� ���� �ִϸ��̼� ũ��
	GLfloat VerticalSize{};

	// ���� �ִϸ��̼�
	SinLoop VerticalLoop{};

	// ����� ��
	GLfloat TiltValue{};

	// ����� ���� �ִϸ��̼�
	SinLoop TiltLoop{};

	// �����̴� ����
	bool MoveState{};

	int Frame{};

public:
	// MoveState�� Ȱ��ȭ�� ���·� �����Ǹ� ������ ��ġ�� �̵��Ѵ�.
	// MoveState�� ��Ȱ��ȭ�� ���·� �����Ǹ� �̵����� �ʴ´�.
	Xion(GLfloat PositionValue, bool BoolMoveState, int FrameValue);
	void UpdateFunc(float FrameTime);
	void RenderFunc();
};