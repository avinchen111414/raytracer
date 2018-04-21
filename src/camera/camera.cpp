#include "camera.h"
#include "world/world.h"

Camera::Camera()
	:	eye(0, 0, 500),
	lookat(0),
	up(0, 1, 0),
	u(1, 0, 0),
	v(0, 1, 0),
	w(0, 0, 1),
	exposure_time(1.0)
{}

Camera::Camera(const Camera& c)
	:	eye(c.eye),
	lookat(c.lookat),
	up(c.up),
	u(c.u),
	v(c.v),
	w(c.w),
	exposure_time(c.exposure_time)
{

}

Camera::~Camera() {}

void Camera::compute_uvw()
{
	w = eye - lookat;
	w.normalize();
	u = up ^ w;
	u.normalize();
	v = w ^ u;

	if (eye.x == lookat.x && eye.z == lookat.z && eye.y > lookat.y) { // camera looking vertically down
		u = Vector3D(0, 0, 1);
		v = Vector3D(1, 0, 0);
		w = Vector3D(0, 1, 0);	
	}

	if (eye.x == lookat.x && eye.z == lookat.z && eye.y < lookat.y) { // camera looking vertically up
		u = Vector3D(1, 0, 0);
		v = Vector3D(0, 0, 1);
		w = Vector3D(0, -1, 0);
	}
}