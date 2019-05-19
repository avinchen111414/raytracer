#include "camera.h"
#include "world/world.h"

Camera::Camera()
	:	m_eye(0, 0, 500),
	m_lookat(0),
	m_up(0, 1, 0),
	m_u(1, 0, 0),
	m_v(0, 1, 0),
	m_w(0, 0, 1),
	m_exposure_time(1.0)
{}

Camera::Camera(const Camera& c)
	:	m_eye(c.m_eye),
	m_lookat(c.m_lookat),
	m_up(c.m_up),
	m_u(c.m_u),
	m_v(c.m_v),
	m_w(c.m_w),
	m_exposure_time(c.m_exposure_time)
{

}

Camera::~Camera() {}

void Camera::ComputeUvw()
{
	m_w = m_eye - m_lookat;
	m_w.normalize();
	m_u = m_up ^ m_w;
	m_u.normalize();
	m_v = m_w ^ m_u;

	if (m_eye.x == m_lookat.x && m_eye.z == m_lookat.z && m_eye.y > m_lookat.y) { // camera looking vertically down
		m_u = Vector3D(0, 0, 1);
		m_v = Vector3D(1, 0, 0);
		m_w = Vector3D(0, 1, 0);	
	}

	if (m_eye.x == m_lookat.x && m_eye.z == m_lookat.z && m_eye.y < m_lookat.y) { // camera looking vertically up
		m_u = Vector3D(1, 0, 0);
		m_v = Vector3D(0, 0, 1);
		m_w = Vector3D(0, -1, 0);
	}
}