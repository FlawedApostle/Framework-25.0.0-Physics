#pragma once
#include <VMath.h>
#include<QMath.h>

using namespace MATH;

class CameraController {
public:
	CameraController();

    void OnMouseMove(float dx, float dy);
    void Update(float dt);

    Matrix4 GetViewMatrix() const;

    void SetTarget(const Vec3& t);
    void SetDistance(float d);

private:
    float yaw;
    float pitch;

    Vec3 target;
    Vec3 offset;

    Vec3 cameraPos;

    float sensitivity;
    float smooth;

};