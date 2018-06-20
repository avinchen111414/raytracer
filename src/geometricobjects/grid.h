#ifndef __GRID_H__
#define __GRID_H__

#include "compound.h"

class BBox;

class Grid: public Compound
{
public:
	Grid();
	
	virtual ~Grid();
	virtual GeometricObject* clone() const {return nullptr;}
	virtual const BBox* get_bounding_box() const;
	virtual bool hit(const Ray& ray, double& tmin, ShadeRec& s) const;
	virtual bool shadow_hit(const Ray& ray, float& tmin) const;

	void setup_cells();

protected:
	Point3D min_coordinates() const;
	Point3D max_coordinates() const;

private:
	Grid(const Grid& other);
	Grid& operator= (const Grid& rhs);

private:
	std::vector<GeometricObject*> m_cells;
	BBox m_bbox;
	size_t m_nx, m_ny, m_nz;

};

#endif