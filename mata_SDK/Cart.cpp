#include "Cart.h"

Cart::Cart(bool Fell, glm::vec2 PositionValue) {
	FellDown = Fell;
	Position = PositionValue;

	// 넘어진 상태일 경우 1번 프레임을 렌더링한다
	if (Fell)
		ChloeFrame = 1;
}

void Cart::UpdateFunc(float FrameTime) {
	// 화면 밖을 벗어날 경우 삭제
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