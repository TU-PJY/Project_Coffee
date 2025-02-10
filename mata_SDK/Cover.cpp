#include "Cover.h"

void Cover::UpdateFunc(float FrameTime) {
	Opacity += FrameTime;
	EX.ClampValue(Opacity, 1.0, CLAMP_GREATER);
}

void Cover::RenderFunc(){
	Begin(RENDER_TYPE_STATIC);
	transform.Scale(MoveMatrix, ASP(2.0), 2.0);
	imageUtil.Render(SysRes.COLOR_TEXTURE, Opacity);
}

bool Cover::GetState() {
	if (Opacity == 1.0)
		return true;
	
	return false;
}
