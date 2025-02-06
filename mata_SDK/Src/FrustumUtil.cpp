#include "FrustumUtil.h"
#include "Config.h"
#include "CameraUtil.h"
#include "CameraController.h"

FrustumUtil frustum;

void FrustumUtil::Update() {
	if (CameraRotation > 0.0 || CameraRotation < 0.0) {
		FrustumOOBB.Update(CameraPosition, ASP(2.0), 2.0, CameraRotation);
		Rotated = true;
#ifdef SHOW_FRUSTUM_BOX
		camera.SetCamera(RENDER_TYPE_DEFAULT);
		FrustumOOBB.Render();
#endif
	}

	else {
		FrustumAABB.Update(CameraPosition, ASP(2.0), 2.0);
		Rotated = false;
#ifdef SHOW_FRUSTUM_BOX
		camera.SetCamera(RENDER_TYPE_DEFAULT);
		FrustumAABB.Render();
#endif
	}
}

bool FrustumUtil::Check(AABB& aabb) {
	if (Rotated && FrustumOOBB.CheckCollision(aabb))
		return true;

	else if (!Rotated && FrustumAABB.CheckCollision(aabb))
		return true;

	return false;
}

bool FrustumUtil::Check(OOBB& oobb) {
	if (Rotated && FrustumOOBB.CheckCollision(oobb))
		return true;

	else if (!Rotated && FrustumAABB.CheckCollision(oobb))
		return true;

	return false;
}

bool FrustumUtil::Check(BoundingCircle& circle) {
	if (Rotated && FrustumOOBB.CheckCollision(circle))
		return true;

	else if (!Rotated && FrustumAABB.CheckCollision(circle))
		return true;

	return false;
}
