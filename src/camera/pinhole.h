#ifndef __PINHOLE_H__
#define __PINHOLE_H__

#include "camera.h"
#include "utilities/vector3d.h"

class Point2D;
class World;
class RenderThread;

class PinHole: public Camera
{
public:
	PinHole();
	PinHole(const PinHole& camera);
	virtual ~PinHole();

	virtual void render_scene(const World& w, int start_row, int end_row,
		RenderThread* paint_thread);

	Vector3D ray_direction(const Point2D& p) const;

public:
	float d;
	float zoom;
};

#endif