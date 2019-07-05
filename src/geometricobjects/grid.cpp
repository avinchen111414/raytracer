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

const BBox* Grid::GetBoundingBox() const
{
	return &m_bbox;
}

bool Grid::Hit(const Ray& ray, double& tmin, ShadeRec& s) const
{ 
	// Same as BBox::hit
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

	// Initial cell coordinates
	int ix, iy, iz;
	
	// Ray starts inside the grid
	if (m_bbox.Inside(ray.o))
	{
		ix = Clamp((ox - x0) / (x1 - x0) * m_nx, 0, m_nx - 1);
		iy = Clamp((oy - y0) / (y1 - y0) * m_ny, 0, m_ny - 1);
		iz = Clamp((oz - z0) / (z1 - z0) * m_nz, 0, m_nz - 1);
	}
	// Ray starts outside the grid
	else
	{
		// t0 is the ray parameter entering the grid
		Point3D p = ray.o + t0 * ray.d;
		ix = Clamp((p.x - x0) / (x1 - x0) * m_nx, 0, m_nx - 1);
		iy = Clamp((p.y - y0) / (y1 - y0) * m_ny, 0, m_ny - 1);
		iz = Clamp((p.z - z0) / (z1 - z0) * m_nz, 0, m_nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions
	double dtx = (tx_max - tx_min) / m_nx;
	double dty = (ty_max - ty_min) / m_ny;
	double dtz = (tz_max - tz_min) / m_nz;

	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	// increments in x directions is positive, means the ray come from
	// left to right
	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = m_nx;
	}
	else {
		tx_next = tx_min + (m_nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = INT_MAX;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = m_ny;
	}
	else {
		ty_next = ty_min + (m_ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = INT_MAX;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = m_nz;
	}
	else {
		tz_next = tz_min + (m_nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = INT_MAX;
		iz_step = -1;
		iz_stop = -1;
	}

	// traverse the grid

	while (true) 
	{
		GeometricObject* object_ptr = m_cells[ix + m_nx * iy + m_nx * m_ny * iz];
		
		// tx_next 最小，说明射线将要进入YOZ平面，由tx_next给出的cell
		if (tx_next < ty_next && tx_next < tz_next)
		{
			if (object_ptr && object_ptr->Hit(ray, tmin, s) && tmin < tx_next)
			{
				const_cast<Material*>(m_material) = object_ptr->GetMaterial();
				return true;
			}

			tx_next += dtx;
			ix += ix_step;
			if (ix == ix_stop)
				return false;
		}
		else
		{
			if (ty_next < tz_next)
			{
				if (object_ptr && object_ptr->Hit(ray, tmin, s) && tmin < ty_next)
				{
					const_cast<Material*>(m_material) = object_ptr->GetMaterial();
					return true;
				}

				ty_next += dty;
				iy += iy_step;
				if (iy == iy_stop)
					return false;
			}
			else
			{
				if (object_ptr && object_ptr->Hit(ray, tmin, s) && tmin < tz_next)
				{
					const_cast<Material*>(m_material) = object_ptr->GetMaterial();
					return true;
				}

				tz_next += dtz;
				iz += iz_step;
				if (iz == iz_stop)
					return false;
			}
		}
	}
}

bool Grid::ShadowHit(const Ray& ray, float& tmin) const
{
	// Same as BBox::hit
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

	// Initial cell coordinates
	int ix, iy, iz;

	// Ray starts inside the grid
	if (m_bbox.Inside(ray.o))
	{
		ix = Clamp((ox - x0) / (x1 - x0) * m_nx, 0, m_nx - 1);
		iy = Clamp((oy - y0) / (y1 - y0) * m_ny, 0, m_ny - 1);
		iz = Clamp((oz - z0) / (z1 - z0) * m_nz, 0, m_nz - 1);
	}
	// Ray starts outside the grid
	else
	{
		// t0 is the ray parameter entering the grid
		Point3D p = ray.o + t0 * ray.d;
		ix = Clamp((p.x - x0) / (x1 - x0) * m_nx, 0, m_nx - 1);
		iy = Clamp((p.y - y0) / (y1 - y0) * m_ny, 0, m_ny - 1);
		iz = Clamp((p.z - z0) / (z1 - z0) * m_nz, 0, m_nz - 1);
	}

	// ray parameter increments per cell in the x, y, and z directions
	double dtx = (tx_max - tx_min) / m_nx;
	double dty = (ty_max - ty_min) / m_ny;
	double dtz = (tz_max - tz_min) / m_nz;

	double 	tx_next, ty_next, tz_next;
	int 	ix_step, iy_step, iz_step;
	int 	ix_stop, iy_stop, iz_stop;

	// increments in x directions is positive, means the ray come from
	// left to right
	if (dx > 0) {
		tx_next = tx_min + (ix + 1) * dtx;
		ix_step = +1;
		ix_stop = m_nx;
	}
	else {
		tx_next = tx_min + (m_nx - ix) * dtx;
		ix_step = -1;
		ix_stop = -1;
	}

	if (dx == 0.0) {
		tx_next = INT_MAX;
		ix_step = -1;
		ix_stop = -1;
	}


	if (dy > 0) {
		ty_next = ty_min + (iy + 1) * dty;
		iy_step = +1;
		iy_stop = m_ny;
	}
	else {
		ty_next = ty_min + (m_ny - iy) * dty;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dy == 0.0) {
		ty_next = INT_MAX;
		iy_step = -1;
		iy_stop = -1;
	}

	if (dz > 0) {
		tz_next = tz_min + (iz + 1) * dtz;
		iz_step = +1;
		iz_stop = m_nz;
	}
	else {
		tz_next = tz_min + (m_nz - iz) * dtz;
		iz_step = -1;
		iz_stop = -1;
	}

	if (dz == 0.0) {
		tz_next = INT_MAX;
		iz_step = -1;
		iz_stop = -1;
	}

	// traverse the grid

	while (true) 
	{
		GeometricObject* object_ptr = m_cells[ix + m_nx * iy + m_nx * m_ny * iz];

		// tx_next 最小，说明射线将要进入YOZ平面，由tx_next给出的cell
		if (tx_next < ty_next && tx_next < tz_next)
		{
			if (object_ptr && object_ptr->ShadowHit(ray, tmin) && tmin < tx_next)
				return true;

			tx_next += dtx;
			ix += ix_step;
			if (ix == ix_stop)
				return false;
		}
		else
		{
			if (ty_next < tz_next)
			{
				if (object_ptr && object_ptr->ShadowHit(ray, tmin) && tmin < ty_next)
					return true;

				ty_next += dty;
				iy += iy_step;
				if (iy == iy_stop)
					return false;
			}
			else
			{
				if (object_ptr && object_ptr->ShadowHit(ray, tmin) && tmin < tz_next)
					return true;

				tz_next += dtz;
				iz += iz_step;
				if (iz == iz_stop)
					return false;
			}
		}
	}
}

void Grid::SetupCells()
{
	Point3D p0 = MinCoordinates();
	Point3D p1 = MaxCoordinates();

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

	for (size_t idx = 0; idx < num_cells; idx++)
		m_cells.push_back(nullptr);

	std::vector<int> counts;
	counts.reserve(num_cells);

	for (size_t idx = 0; idx < num_cells; idx++)
		counts.push_back(0);

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const BBox* obj_bbox = m_objects[idx]->GetBoundingBox();
		if (!obj_bbox)
			continue;
		int ixmin = Clamp((obj_bbox->x0 - p0.x) / (p1.x - p0.x) * m_nx, 
			0.0, m_nx - 1);
		int iymin = Clamp((obj_bbox->y0 - p0.y) / (p1.y - p0.y) * m_ny,
			0.0, m_ny - 1);
		int izmin = Clamp((obj_bbox->z0 - p0.z) / (p1.z - p0.z) * m_nz,
			0.0, m_nz - 1);
		int ixmax = Clamp((obj_bbox->x1 - p0.x) / (p1.x - p0.x) * m_nx, 
			0.0, m_nx - 1);
		int iymax = Clamp((obj_bbox->y1 - p0.y) / (p1.y - p0.y) * m_ny,
			0.0, m_ny - 1);
		int izmax = Clamp((obj_bbox->z1 - p0.z) / (p1.z - p0.z) * m_nz,
			0.0, m_nz - 1);

		for (int iz = izmin; iz <= izmax; iz++)
			for (int iy = iymin; iy <= iymax; iy++)
				for (int ix = ixmin; ix <= ixmax; ix++)
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
							compound->AddObject(m_cells[index]);
							compound->AddObject(m_objects[idx]);
							m_cells[index] = compound;
						}
						else
						{
							compound->AddObject(m_objects[idx]);
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

Point3D Grid::MinCoordinates() const
{
	Point3D p(INT_MAX);

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const BBox* bbox = m_objects[idx]->GetBoundingBox();
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

Point3D Grid::MaxCoordinates() const
{
	Point3D p(-INT_MAX);

	for (size_t idx = 0; idx < m_objects.size(); idx++)
	{
		const BBox* bbox = m_objects[idx]->GetBoundingBox();
		if (!bbox)
			continue;
		if (bbox->x1 > p.x)
			p.x = bbox->x1;
		if (bbox->y1 > p.y)
			p.y = bbox->y1;
		if (bbox->z1 > p.z)
			p.z = bbox->z1;
	} 

	p.x += kEpsilon; p.y += kEpsilon; p.z += kEpsilon;
	return p;
}








