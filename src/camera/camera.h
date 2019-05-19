#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "utilities/vector3d.h"
#include "utilities/point3d.h"

class World;
class RenderThread;
class RenderTile;

class Camera
{
public:
	Camera();
	Camera(const Camera& camera);
	virtual ~Camera();

	virtual void RenderScene(const World& w, const RenderTile& tile,
		RenderThread* paint_thread) = 0;

	void ComputeUvw();

public:
	Point3D m_eye;
	Point3D m_lookat;
	Vector3D m_up;
	Vector3D m_u, m_v, m_w;
	float m_exposure_time;
};

#endif