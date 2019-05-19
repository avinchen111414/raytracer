#ifndef __PINHOLE_H__
#define __PINHOLE_H__

#include "camera.h"
#include "utilities/vector3d.h"

class Point2D;
class World;
class RenderThread;
class RenderTile;

class PinHole: public Camera
{
public:
	PinHole();
	PinHole(const PinHole& camera);
	virtual ~PinHole();

	virtual void RenderScene(const World& w, const RenderTile& tile,
		RenderThread* paint_thread);

	Vector3D RayDirection(const Point2D& p) const;

public:
	float m_d;
	float m_zoom;
};

#endif