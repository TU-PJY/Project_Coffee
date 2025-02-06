#pragma once
#include <Scene.h>

typedef struct {
	PhysicsUtil ps;
	glm::vec2 Position;
	GLfloat MoveSpeed;
	GLfloat Rotation;
	GLfloat RotateSpeed;
	bool DropState;
}DropStruct;

class Explode : public GameObject {
private:
	// Ŀ�� ����� �����͸� �����ϴ� ����
	std::vector<DropStruct> DropVec{};

	// �ùķ��̼��� ��� ����
	int NumDrop = 15;

	// ���� Ŀ�Ǹ� �μ� ��� Ȱ��ȭ �Ѵ� 
	bool IsStick{};

	// �������� �ø��� ���� AABB�� OOBB
	AABB aabb{};
	OOBB oobb{};

public:
	Explode(glm::vec2 Position, bool StickCoffee) {
		IsStick = StickCoffee;

		// ��ƽ Ŀ���� ��� ������ ���δ�
		if (StickCoffee)
			NumDrop = 8;

		for (int i = 0; i < NumDrop; ++i) {
			DropStruct DS{};
			DS.Position = Position;

			// �߷� ����
			DS.ps.SetGravity(10.0);

			// ���� �ٴ� ���� ����
			GLfloat RandomFloorHeight = randomUtil.Gen(RANDOM_TYPE_REAL, -0.85, -0.95);
			DS.ps.SetFloorHeight(RandomFloorHeight);

			// ���� �߷� ���ӵ� �߰�
			GLfloat RandomAcc = randomUtil.Gen(RANDOM_TYPE_REAL, 3.0, 5.0);
			DS.ps.AddGravityAcc(RandomAcc);

			// ���� �ӵ� ����
			DS.MoveSpeed = randomUtil.Gen(RANDOM_TYPE_REAL, -2.0, 2.0);

			// ���� Ŀ�Ǹ� �ν��� ��� ���� ȸ�� �ӵ��� �����Ѵ�
			if (StickCoffee)
				DS.RotateSpeed = randomUtil.Gen(RANDOM_TYPE_REAL, 200.0, 1500.0);

			DropVec.emplace_back(DS);
		}
	}

	void UpdateFunc(float FrameTime) {
		// �� ����� ���� �ùķ��̼� ����
		for (auto& D : DropVec) {
			D.ps.UpdateFalling(D.Position.y, FrameTime);

			// �ٴڿ� ������������ �����δ�
			if (D.ps.GetFallingState()) {
				D.Position.x += D.MoveSpeed * FrameTime;

				// Ŀ�� ��ƽ�� ��� ȸ�� ������Ʈ
				if (IsStick)
					D.Rotation += D.RotateSpeed * FrameTime;
			}
		}
	}

	void RenderFunc() {
		int Count{};

		for (auto& D : DropVec) {
			if (IsStick) {
				oobb.Update(D.Position, 0.2, 0.2, D.Rotation);
				if (!frustum.Check(oobb)) {
					Count++;
					continue;
				}
			}
			else {
				aabb.Update(D.Position, 0.12, 0.12);
				if (!frustum.Check(aabb)) {
					Count++;
					continue;
				}
			}

			Begin();
			transform.Move(MoveMatrix, D.Position);

			if (IsStick) {
				transform.Scale(MoveMatrix, 0.2, 0.2);
				transform.Rotate(MoveMatrix, D.Rotation);
				imageUtil.Render(Img.Stick);
			}

			// �ٴڿ� �������� �ٸ� ��������Ʈ�� �������Ѵ�
			if (D.ps.GetFallingState()) {
				transform.Scale(MoveMatrix, 0.06, 0.06);
				imageUtil.RenderStaticSpriteSheet(Img.Drop, 0);
			}
			else {
				transform.Scale(MoveMatrix, 0.12, 0.12);
				imageUtil.RenderStaticSpriteSheet(Img.Drop, 1);
			}
		}

		// ��� ����� ȭ�� ���� ����� ������ �����Ǹ� ������ �����Ѵ�
		if (Count == NumDrop)
			scene.DeleteObject(this);
	}
};