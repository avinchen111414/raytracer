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

	virtual void render_scene(const World& w, const RenderTile& tile,
		RenderThread* paint_thread) = 0;

	void compute_uvw();

public:
	Point3D eye;
	Point3D lookat;
	Vector3D up;
	Vector3D u, v, w;
	float exposure_time;
};

#endif