#pragma once
#include <Scene.h>

class Xion : public GameObject {
private:
	// ���� ��ġ
	glm::vec2 Position{};

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
	
	// ���� ��ġ
	glm::vec2 ShakeValue{};

	// ���� Ÿ�̸�
	TimerUtil ShakeTimer{};

	// ȸ����
	GLfloat Rotation{};

	// ���� ȸ����
	GLfloat HRotation{};
	GLfloat DestHRotation{};

	// �����̴� ����
	bool MoveState{};

	// �̵忡�� ���� ����
	bool HitState{};

	// �Ѿ��� ����
	bool FellDown{};

	// �и��� ����
	bool PushState{};
	
	// ������
	int Frame{};

	SoundChannel SndChannel{};

	// ���ӿ��� ������ Ÿ�̸�
	TimerUtil GameOverTimer{};

public:
	// MoveState�� Ȱ��ȭ�� ���·� �����Ǹ� ������ ��ġ�� �̵��Ѵ�.
	// MoveState�� ��Ȱ��ȭ�� ���·� �����Ǹ� �̵����� �ʴ´�.
	Xion(GLfloat PositionValue, GLfloat DestPositionValue, bool BoolMoveState, int FrameValue);
	void UpdateFunc(float FrameTime);
	void RenderFunc();
	void HitPeople();
	void PushPeople();
	void SetFrame(int Value);
};