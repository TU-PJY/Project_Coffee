#pragma once
#include "UtilPack.h"

class GameObject {
public:
	std::string ObjectTag{};
	int         ObjectLayer{};

	bool        FloatingCommand{};
	bool        StaticCommand{};
	
	bool        DeleteCommand{};
	bool        SwapCommand{};

	// Initialize matrix. Select a rendering type.
	void Begin(int RenderType=RENDER_TYPE_DEFAULT);

	// Overlays color over the image.
	void SetColor(GLfloat R, GLfloat G, GLfloat B);

	// Overlays color over the image.
	void SetColor(glm::vec3& Color);

	// Overlays color over the image.
	void SetColorRGB(int R, int G, int B);

	// Updates the object's position relative to the viewport. Choose whether to apply aspect ratio.
	void ComputeViewportPosition(GLfloat& DestX, GLfloat& DestY, bool ApplyAspect = true);

	// Updates the object's position relative to the viewport. Choose whether to apply aspect ratio.
	void ComputeViewportPosition(glm::vec2& DestValue, bool ApplyAspect);

	// Updates the object's position relative to the local coordinate system.
	void ComputeLocalPosition(GLfloat& DestX, GLfloat& DestY);

	// Updates the object's position relative to the local coordinate system.
	void ComputeLocalPosition(glm::vec2& DestPosition);

	void SetUnitFlip(int FlipOpt);
	void SetUnitOpacity(GLfloat Value);
	void SetUnitBlur(GLfloat Strength);
	void ResetUnitTransform();

	void SetFlip(int FlipOpt);
	void SetBlur(GLfloat Strength);
	
	// class destructor
	virtual ~GameObject() {}

	// Updates a value inside an object class.
	virtual void UpdateFunc(float FrameTime) {}

	// Renders image resources used by object classes.
	virtual void RenderFunc() {}

	// Sends keyboard events to a specific object class.
	virtual void InputKey(KeyEvent& Event) {}

	// Sends mouse events to a specific object class.
	virtual void InputMouse(int Type) {}

	// Sends mouse scroll events to a specific object class.
	virtual void InputScroll(int Type) {}

	// Initializes the control state for a specific object class.
	virtual void ResetControlState() {}

	// Obtains an AABB object from a specific object class.
	virtual AABB GetAABB() { return {}; }

	// Obtains an OOBB object from a specific object class.
	virtual OOBB GetOOBB() { return{}; }

	// Obtains an BoundingCircle object from a specific object class.
	virtual BoundingCircle GetBoundingCircle() { return {}; }

	// camera functions
	virtual void MoveCamera(GLfloat X, GLfloat Y) {}
	virtual void MoveCamera(glm::vec2 Position) {}
	virtual void RotateCamera(GLfloat Degree) {}
	virtual void CameraZoom(int ZoomType, GLfloat ZoomValue) {}
	virtual void ChangeCameraZoom(GLfloat ZoomValue) {}
	virtual GLfloat ComputeNextZoom(int ZoomType, GLfloat ZoomValue) { return {}; }

	// FPS indicator function
	virtual void RenderIndicator() {}

private:
	glm::vec4 ViewportPosition();
	glm::vec4 LocalPosition();
};