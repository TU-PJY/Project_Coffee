#include "FrustumUtil.h"
#include "Config.h"
#include "CameraUtil.h"
#include "CameraController.h"

FrustumUtil frustum;

void FrustumUtil::Update() {
	FrustumBox.Update(CameraPosition, ASP(2.0), 2.0, CameraRotation);

#ifdef SHOW_FRUSTUM_BOX
	camera.SetCamera(RENDER_TYPE_DEFAULT);
	FrustumBox.Render();
#endif
}

bool FrustumUtil::Check(AABB& aabb) {
	if(FrustumBox.CheckCollision(aabb))
		return true;
	return false;
}

bool FrustumUtil::Check(OOBB& oobb) {
	if (FrustumBox.CheckCollision(oobb))
		return true;
	return false;
}

bool FrustumUtil::Check(BoundingCircle& circle) {
	if (FrustumBox.CheckCollision(circle))
		return true;
	return false;
}
