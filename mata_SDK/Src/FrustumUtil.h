#pragma once
#include "CollisionUtil.h"
class FrustumUtil {
private:
	OOBB FrustumOOBB{};
	AABB FrustumAABB{};

	bool Rotated{};

public:
	void Update();
	bool Check(AABB& aabb);
	bool Check(OOBB& oobb);
	bool Check(BoundingCircle& circle);
};
extern FrustumUtil frustum;