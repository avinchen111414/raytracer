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

public:
	Sampler& operator= (const Sampler& rhs);
	inline int GetNumSamples() {return num_samples;};

public:
	virtual Point2D SampleUnitSquare();
	virtual Point2D SampleUnitDisk();
	virtual Point3D SampleHemisphere();
	virtual void MapSamplesToUnitDisk();
	virtual void MapSamplesToHemisphere(const float e);
	virtual Sampler* Clone() const = 0;
	virtual void GenerateSamples() = 0;
	virtual void SetupShuffledIndices();

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