#ifndef __SAMPLER_H__
#define __SAMPLER_H__

#include <vector>
#include "utilities/point2d.h"
#include "utilities/point3d.h"

class Sampler
{
public:
	Sampler(void);								
	Sampler(const int num);						
	Sampler(const int num, const int num_sets);		
	Sampler(const Sampler& s);
	virtual ~Sampler(void);
	Sampler& operator= (const Sampler& rhs);

	virtual void generate_samples() = 0;
	void setup_shuffled_indices();
	Point2D sample_unit_square();
	Point2D sample_unit_disk();
	Point3D sample_hemisphere();

	inline int get_num_samples() {return num_samples;};
	void map_samples_to_unit_disk();
	void map_samples_to_hemisphere(const float e);

	virtual Sampler* clone() const = 0;

protected:
	int num_samples;
	int num_sets;
	std::vector<Point2D> samples;
	std::vector<Point2D> disk_samples;
	std::vector<Point3D> hemisphere_samples;
	std::vector<int> shuffled_indices;
	unsigned long count;
	int jump;
};

#endif