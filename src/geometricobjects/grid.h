#ifndef __GRID_H__
#define __GRID_H__

#include "compound.h"
#include "utilities/bbox.h"

class BBox;

class Grid: public Compound
{
public:
	Grid();
	
	virtual ~Grid();
	virtual GeometricObject* Clone() const {return nullptr;}
	virtual const BBox* GetBoundingBox() const;
	virtual bool Hit(const Ray& ray, double& tmin, ShadeRec& s) const;
	virtual bool ShadowHit(const Ray& ray, float& tmin) const;

	void SetupCells();

protected:
	Point3D MinCoordinates() const;
	Point3D MaxCoordinates() const;

private:
	Grid(const Grid& other);
	Grid& operator= (const Grid& rhs);

protected:
	std::vector<GeometricObject*> m_cells;
	BBox m_bbox;
	size_t m_nx, m_ny, m_nz;

};

#endif