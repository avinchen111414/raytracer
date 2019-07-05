#ifndef __BBOX_H__
#define __BBOX_H__

class Point3D;
class Ray;
class BBox
{
public:
	BBox(double _x0, double _y0, double _z0,
			double _x1, double _y1, double _z1);
	BBox(const Point3D& p0, const Point3D& p1);
	BBox(const BBox& other);
	BBox();
	BBox& operator= (const BBox& rhs);
	~BBox();

	bool Hit (const Ray& ray) const;
	bool Inside (const Point3D& point) const;
	void Merge (const BBox& other);

public:
	double x0, y0, z0, x1, y1, z1;
	static const double kEpsilon;
};

#endif