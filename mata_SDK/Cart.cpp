#include "Cart.h"

Cart::Cart(bool Fell, glm::vec2 PositionValue) {
	FellDown = Fell;
	Position = PositionValue;

	// �Ѿ��� ������ ��� 1�� �������� �������Ѵ�
	if (Fell)
		ChloeFrame = 1;
}

void Cart::UpdateFunc(float FrameTime) {
	// ȭ�� ���� ��� ��� ����
	if (Position.x < CameraPosition.x - ASP(1.0) - 0.75)
		scene.DeleteObject(this);
}

void Cart::RenderFunc() {
	if (Position.x > CameraPosition.x + ASP(1.0) + 0.75)
		return;

	Begin();
	transform.Move(MoveMatrix, Position);
	transform.Scale(MoveMatrix, 1.5, 1.5);
	imageUtil.RenderStaticSpriteSheet(Img.Cart, ChloeFrame);
}