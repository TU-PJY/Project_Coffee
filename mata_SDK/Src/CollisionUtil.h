#pragma once
#include "SDKHeader.h"
#include "RectBrush.h"
#include "CircleBrush.h"
using namespace DirectX;

class OOBB;
class BoundingCircle;

class AABB {
private:
	BoundingBox   aabb;
	bool          Collide{};

#ifdef SHOW_BOUND_BOX
	LineRectBrush LineRect{true};
	RectBrush     Rect{true};
#endif

public:
	void Update(GLfloat X, GLfloat Y, GLfloat xScale, GLfloat yScale);
	void Update(glm::vec2& Position, GLfloat xScale, GLfloat yScale);
	void Render();
	bool CheckCollision(const AABB& Other);
	bool CheckCollision(const OOBB& Other);
	bool CheckCollision(const BoundingCircle& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);
	bool CheckCollisionPoint(glm::vec2& Position);
	bool CheckCollisionRay(RayVector& Ray);
	bool CheckCollisionLine(RayVector& Ray);
	BoundingBox Get() const;
};


class OOBB {
private:
	BoundingOrientedBox oobb;
	GLfloat             Rotation{};
	bool                Collide{};

#ifdef SHOW_BOUND_BOX
	LineRectBrush       LineRect{ true };
	RectBrush           Rect{ true };
#endif

public:
	void Update(GLfloat X, GLfloat Y, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue);
	void Update(glm::vec2& Position, GLfloat BoxWidth, GLfloat BoxHeight, GLfloat RotationValue);
	void Render();
	bool CheckCollision(const OOBB& Other);
	bool CheckCollision(const AABB& Other);
	bool CheckCollision(const BoundingCircle& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);
	bool CheckCollisionPoint(glm::vec2& Position);
	bool CheckCollisionRay(RayVector& Ray);
	bool CheckCollisionLine(RayVector& Ray);
	BoundingOrientedBox Get() const;
};


class BoundingCircle {
private:
	BoundingSphere  sphere;
	glm::vec2       Center{};
	GLfloat         Radius{};
	GLfloat         Size{};

	bool            Collide{};

#ifdef SHOW_BOUND_BOX
	LineCircleBrush LineCircle { true };
	CircleBrush     Circle{ true };
#endif

public:
	void Update(GLfloat X, GLfloat Y, GLfloat Diameter);
	void Update(glm::vec2& Position, GLfloat SizeValue);
	void Render();
	bool CheckCollision(const BoundingCircle& Other);
	bool CheckCollision(const AABB& Other);
	bool CheckCollision(const OOBB& Other);
	bool CheckCollisionPoint(GLfloat X, GLfloat Y);
	bool CheckCollisionPoint(glm::vec2& Position);
	bool CheckCollisionRay(RayVector& Ray);
	bool CheckCollisionLine(RayVector& Ray);
	BoundingSphere Get() const;
};