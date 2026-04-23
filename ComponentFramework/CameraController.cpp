#include <iostream>
#include "CameraController.h"

#include "MMath.h"

CameraController::CameraController()
{
	yaw = 0.0f;
	pitch = 0.0f;

	target = Vec3(0, 0, 0);
	offset = Vec3(0, 0, 25);

	cameraPos = target + offset;

	sensitivity = 0.005f;
	smooth = 8.0f; 

}

void CameraController::OnMouseMove(float dx, float dy) 
{
	yaw += dx * sensitivity;
	pitch += dy * sensitivity;

	// clamp pitch so camera doesn’t flip
	float limit = 1.5f; // ~85 degrees
	if (pitch > limit) pitch = limit;
	if (pitch < -limit) pitch = -limit;
}

void CameraController::Update(float dt) 
{

	// build rotation (yaw = Y axis, pitch = X axis)
	Matrix4 rotY = MMath::rotate(yaw, Vec3(0, 1, 0));
	Matrix4 rotX = MMath::rotate(pitch, Vec3(1, 0, 0));

	Matrix4 rot = rotY * rotX;

	// rotate offset
	Vec3 desiredOffset = rot * offset;

	Vec3 desiredPos = target + desiredOffset;

	// smooth damping
	float t = 1.0f - exp(-smooth * dt);
	cameraPos = VMath::lerp(cameraPos, desiredPos, t);
}

Matrix4 CameraController::GetViewMatrix() const 
{
	return MMath::lookAt(cameraPos, target, Vec3(0, 1, 0));
}

void CameraController::SetTarget(const Vec3& t) {
	target = t;
}

void CameraController::SetDistance(float d)
{
	offset = { 0,0,d };
}
