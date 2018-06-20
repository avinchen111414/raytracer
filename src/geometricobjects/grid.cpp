#include <iostream>
#include "utilities/bbox.h"
#include "utilities/utility.h"
#include "utilities/ray.h"
#include "utilities/bbox.h"
#include "grid.h"

Grid::Grid()
	: Compound(), m_bbox(0, 0, 0, 0, 0, 0)
{

}

Grid::~Grid()
{
	for (size_t idx = 0; idx < m_cells.size(); idx++)
	{
		delete m_cells[idx];
	}
	m_cells.clear();
}

const BBox* Grid::get_bounding_box() const
{
	return &m_bbox;
}

bool Grid::hit(const Ray& ray, double& tmin, ShadeRec& s) const
{
	double ox = ray.o.x;
	double oy = ray.o.y;
	double oz = ray.o.z;
	double dx = ray.d.x;
	double dy = ray.d.y;
	double dz = ray.d.z;

	double x0 = m_bbox.x0;
	double y0 = m_bbox.y0;
	double z0 = m_bbox.z0;
	double x1 = m_bbox.x1;
	double y1 = m_bbox.y1;
	double z1 = m_bbox.z1;

	double tx_min, ty_min, tz_min;
	double tx_max, ty_max, tz_max;

	double a = 1.0 / dx;
	if (a >= 0) {
		tx_min = (x0 - ox) * a;
		tx_max = (x1 - ox) * a;
	}
	else {
		tx_min = (x1 - ox) * a;
		tx_max = (x0 - ox) * a;
	}

	double b = 1.0 / dy;
	if (b >= 0) {
		ty_min = (y0 - oy) * b;
		ty_max = (y1 - oy) * b;
	}
	else {
		ty_min = (y1 - oy) * b;
		ty_max = (y0 - oy) * b;
	}

	double c = 1.0 / dz;
	if (c >= 0) {
		tz_min = (z0 - oz) * c;
		tz_max = (z1 - oz) * c;
	}
	else {
		tz_min = (z1 - oz) * c;
		tz_max = (z0 - oz) * c;
	}

	double t0, t1;

	if (tx_min > ty_min)
		t0 = tx_min;
	else
		t0 = ty_min;

	if (tz_min > t0)
		t0 = tz_min;

	if (tx_max < ty_max)
		t1 = tx_max;
	else
		t1 = ty_max;

	if (tz_max < t1)
		t1 = tz_max;

	if (t0 > t1)
		return(false);

	return true;
}

bool Grid::shadow_hit(const Ray& ray, float& tmin) const
{
	return true;
}

void Grid::setup_cells()
{
	Point3D p0 = min_coordinates();
	Point3D p1 = max_coordinates();

	m_bbox.x0 = p0.x;
	m_bbox.y0 = p0.y;
	m_bbox.z0 = p0.z;

	m_bbox.x1 = p1.x;
	m_bbox.y1 = p1.y;
	m_bbox.z1 = p1.z;

	size_t num_objects = m_objects.size();
	float wx = p1.x - p0.x;
	float wy = p1.y - p0.y;
	float wz = p1.z - p0.z;

	float multiplier = 2.0f;
	float s = pow(wx * wy * wz, 0.33333333);
	m_nx = multiplier * wx / s + 1;
	m_ny = multiplier * wy / s + 1;
	m_nz = multiplier * wz / s + 1;

	int num_cells = m_nx * m_ny * m_nz;
	m_cells.reserve(num_cells);

	for (size_t idx = 0; idx < m_cells.size(); idx++)
		m_cells.push_back(nullptr);

	std::vector<int> counts;
	counts.reserve(num_cells);

	for (size_t idx = 0; idx < counts.size(); idx++)
		counts.push_back(0);

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const BBox* obj_bbox = m_objects[idx]->get_bounding_box();
		if (!obj_bbox)
			continue;
		int ixmin = clamp((obj_bbox->x0 - p0.x) / (p1.x - p0.x) * m_nx, 
			0.0, m_nx - 1);
		int iymin = clamp((obj_bbox->y0 - p0.y) / (p1.y - p0.y) * m_ny,
			0.0, m_ny - 1);
		int izmin = clamp((obj_bbox->z0 - p0.z) / (p1.z - p0.z) * m_nz,
			0.0, m_nz - 1);
		int ixmax = clamp((obj_bbox->x1 - p0.x) / (p1.x - p0.x) * m_nx, 
			0.0, m_nx - 1);
		int iymax = clamp((obj_bbox->y1 - p0.y) / (p1.y - p0.y) * m_ny,
			0.0, m_ny - 1);
		int izmax = clamp((obj_bbox->z1 - p0.z) / (p1.z - p0.z) * m_nz,
			0.0, m_nz - 1);

		for (int iz = izmin; iz <= izmax; iz++)
			for (int iy = iymin; iy <= iymax; iy++)
				for (int ix = ixmin; ix <= izmax; iz++)
				{
					int index = ix + iy * m_nx + iz * m_nx * m_ny;
					if (m_cells[index] == nullptr)
					{
						m_cells[index] = m_objects[idx];
					}
					else
					{
						Compound* compound = dynamic_cast<Compound*>
							(m_cells[index]);
						if (!compound)
						{
							Compound* compound = new Compound;
							compound->add_object(m_cells[index]);
							compound->add_object(m_objects[idx]);
							m_cells[index] = compound;
						}
						else
						{
							compound->add_object(m_objects[idx]);
						}
					}
					counts[index] += 1;
				}
	}

	m_objects.clear();

	int num_zeroes 	= 0;
	int num_ones 	= 0;
	int num_twos 	= 0;
	int num_threes 	= 0;
	int num_greater = 0;

	for (int j = 0; j < num_cells; j++) {
		if (counts[j] == 0)
			num_zeroes += 1;
		if (counts[j] == 1)
			num_ones += 1;
		if (counts[j] == 2)
			num_twos += 1;
		if (counts[j] == 3)
			num_threes += 1;
		if (counts[j] > 3)
			num_greater += 1;
	}

	std::cout << "num_cells =" << num_cells << std::endl;
	std::cout << "numZeroes = " << num_zeroes << "  numOnes = " << num_ones << "  numTwos = " << num_twos << std::endl;  
	std::cout << "numThrees = " << num_threes << "  numGreater = " << num_greater << std::endl;		 
}

Point3D Grid::min_coordinates() const
{
	Point3D p(INT_MAX);

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const BBox* bbox = m_objects[idx]->get_bounding_box();
		if (!bbox)
			continue;
		if (bbox->x0 < p.x)
			p.x = bbox->x0;
		if (bbox->y0 < p.y)
			p.y = bbox->y0;
		if (bbox->z0 < p.z)
			p.z = bbox->z0;
	} 

	p.x -= kEpsilon; p.y -= kEpsilon; p.z -= kEpsilon;
	return p;
}

Point3D Grid::max_coordinates() const
{
	Point3D p(-INT_MAX);

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const BBox* bbox = m_objects[idx]->get_bounding_box();
		if (!bbox)
			continue;
		if (bbox->x0 > p.x)
			p.x = bbox->x0;
		if (bbox->y0 > p.y)
			p.y = bbox->y0;
		if (bbox->z0 > p.z)
			p.z = bbox->z0;
	} 

	p.x += kEpsilon; p.y += kEpsilon; p.z += kEpsilon;
	return p;
}








