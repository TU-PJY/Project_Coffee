#include "CollisionUtil.h"
#include "TransformUtil.h"
#include "ImageUtil.h"
#include "CameraUtil.h"
#include "SystemResource.h"
#include "MathUtil.h"


void AABB::Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale) {
	aabb.Center = XMFLOAT3(X, Y, 0.0);
	aabb.Extents = XMFLOAT3(xScale / 2.0, yScale / 2.0, 0.0);
}

void AABB::Update(glm::vec2& Position, GLfloat xScale, GLfloat yScale) {
	aabb.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	aabb.Extents = XMFLOAT3(xScale / 2.0, yScale / 2.0, 0.0);
}

void AABB::Render() {
#ifdef SHOW_BOUND_BOX
	LineRect.SetColor(1.0, 0.0, 0.0);
	Rect.SetColor(1.0, 0.0, 0.0);

	LineRect.Draw(aabb.Center.x, aabb.Center.y, aabb.Extents.x * 2.0, aabb.Extents.y * 2.0, 0.01, 0.0);
	if (Collide)
		Rect.Draw(aabb.Center.x, aabb.Center.y, aabb.Extents.x * 2.0, aabb.Extents.y * 2.0, 0.0, 0.3);
#endif
}

bool AABB::CheckCollision(const AABB& Other) {
	if (aabb.Intersects(Other.aabb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollision(const OOBB& Other) {
	if (aabb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollision(const BoundingCircle& Other) {
	if (aabb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (aabb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (aabb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollisionRay(RayVector& Ray) {
	if (aabb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool AABB::CheckCollisionLine(RayVector& Ray) {
	if (aabb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		if (Ray.Distance <= Ray.Length) {
			Collide = true;
			return true;
		}
		else {
			Collide = false;
			return false;
		}
	}

	Collide = false;
	return false;
}

BoundingBox AABB::Get() const{
	return aabb;
}



void OOBB::Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue) {
	oobb.Center = XMFLOAT3(X, Y, 0.0);
	oobb.Extents = XMFLOAT3(BoxWidth / 2.0, BoxHeight / 2.0, 0.0);
	XMVECTOR Quaternion = XMQuaternionRotationRollPitchYaw(0.0, 0.0, XMConvertToRadians(RotationValue));
	XMStoreFloat4(&oobb.Orientation, Quaternion);
	Rotation = RotationValue;
}

void OOBB::Update(glm::vec2& Position, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue) {
	oobb.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	oobb.Extents = XMFLOAT3(BoxWidth / 2.0, BoxHeight / 2.0, 0.0);
	XMVECTOR Quaternion = XMQuaternionRotationRollPitchYaw(0.0, 0.0, XMConvertToRadians(RotationValue));
	XMStoreFloat4(&oobb.Orientation, Quaternion);
	Rotation = RotationValue;
}

void OOBB::Render() {
#ifdef SHOW_BOUND_BOX
	LineRect.SetColor(1.0, 0.0, 0.0);
	Rect.SetColor(1.0, 0.0, 0.0);

	LineRect.Draw(oobb.Center.x, oobb.Center.y, oobb.Extents.x * 2.0, oobb.Extents.y * 2.0, 0.01, Rotation);
	if (Collide)
		Rect.Draw(oobb.Center.x, oobb.Center.y, oobb.Extents.x * 2.0, oobb.Extents.y * 2.0, Rotation, 0.3);
#endif
}

bool OOBB::CheckCollision(const OOBB& Other) {
	if (oobb.Intersects(Other.oobb)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollision(const AABB& Other) {
	if (oobb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollision(const BoundingCircle& Other) {
	if (oobb.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (oobb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (oobb.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool OOBB::CheckCollisionRay(RayVector& Ray) {
	if (oobb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}


bool OOBB::CheckCollisionLine(RayVector& Ray) {
	if (oobb.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		if (Ray.Distance <= Ray.Length) {
			Collide = true;
			return true;
		}
		else {
			Collide = false;
			return false;
		}
	}

	Collide = false;
	return false;
}

BoundingOrientedBox OOBB::Get() const {
	return oobb;
}



void BoundingCircle::Update(GLfloat X, GLfloat Y, GLfloat SizeValue) {
	sphere.Center = XMFLOAT3(X, Y, 0.0);
	sphere.Radius = SizeValue * 0.5;
}

void BoundingCircle::Update(glm::vec2& Position, GLfloat SizeValue) {
	sphere.Center = XMFLOAT3(Position.x, Position.y, 0.0);
	sphere.Radius = SizeValue * 0.5;
}

void BoundingCircle::Render() {
#ifdef SHOW_BOUND_BOX
	Circle.SetColor(1.0, 0.0, 0.0);
	LineCircle.SetColor(1.0, 0.0, 0.0);

	LineCircle.Draw(sphere.Center.x, sphere.Center.y, sphere.Radius * 2.0 - 0.01, 0.01);
	if (Collide)
		Circle.Draw(sphere.Center.x, sphere.Center.y, sphere.Radius * 2.0, 0.3);
#endif
}

bool BoundingCircle::CheckCollision(const BoundingCircle& Other) {
	if (sphere.Intersects(Other.sphere)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool BoundingCircle::CheckCollision(const AABB& Other) {
	if (sphere.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool BoundingCircle::CheckCollision(const OOBB& Other) {
	if (sphere.Intersects(Other.Get())) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool BoundingCircle::CheckCollisionPoint(GLfloat X, GLfloat Y) {
	XMFLOAT3 Point(X, Y, 0.0);
	if (sphere.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool BoundingCircle::CheckCollisionPoint(glm::vec2& Position) {
	XMFLOAT3 Point(Position.x, Position.y, 0.0);
	if (sphere.Contains(XMLoadFloat3(&Point)) == ContainmentType::CONTAINS) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool BoundingCircle::CheckCollisionRay(RayVector& Ray) {
	if (sphere.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		Collide = true;
		return true;
	}

	Collide = false;
	return false;
}

bool BoundingCircle::CheckCollisionLine(RayVector& Ray) {
	if (sphere.Intersects(Ray.Origin, Ray.Direction, Ray.Distance)) {
		if (Ray.Distance <= Ray.Length) {
			Collide = true;
			return true;
		}
		else {
			Collide = false;
			return false;
		}
	}

	Collide = false;
	return false;
}


BoundingSphere BoundingCircle::Get() const {
	return sphere;
}