#include <algorithm>
#include <cstdlib>
#include <cmath>
#include <vector>

#include "sampler.h"
#include "utilities/utility.h"


Sampler::Sampler()
	: num_samples(1),
	num_sets(83),
	count(0),
	jump(0) 
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
}

Sampler::Sampler(const int ns)
	: num_samples(ns),
	num_sets(83),
	count(0),
	jump(0)
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
}

Sampler::Sampler(const int ns, const int n_sets)
	: num_samples(ns),
	num_sets(n_sets),
	count(0),
	jump(0)
{
	samples.reserve(num_samples * num_sets);
	setup_shuffled_indices();
}

Sampler::Sampler(const Sampler& s)				
	: 	num_samples(s.num_samples),
	num_sets(s.num_sets),
	samples(s.samples),
	shuffled_indices(s.shuffled_indices),
	count(s.count),
	jump(s.jump)
{}

Sampler& 
	Sampler::operator= (const Sampler& rhs)	{
		if (this == &rhs)
			return (*this);

		num_samples 		= rhs.num_samples;
		num_sets			= rhs.num_sets;
		samples				= rhs.samples;
		shuffled_indices	= rhs.shuffled_indices;
		count				= rhs.count;
		jump				= rhs.jump;

		return (*this);
}

Sampler::~Sampler()
{

}

void Sampler::setup_shuffled_indices(void) {
		shuffled_indices.reserve(num_samples * num_sets);
		std::vector<int> indices;

		for (int j = 0; j < num_samples; j++)
			indices.push_back(j);

		for (int p = 0; p < num_sets; p++) {
			random_shuffle(indices.begin(), indices.end());	

			for (int j = 0; j < num_samples; j++)
				shuffled_indices.push_back(indices[j]);
		}	
}

Point2D Sampler::sample_unit_square()
{
	if (count % num_samples == 0)
		jump = (rand() % num_sets) * num_samples;
	return (samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}

Point2D Sampler::sample_unit_disk()
{
	if (count % num_samples == 0)
		jump = (rand() % num_sets) * num_samples;
	return (disk_samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}

Point3D Sampler::sample_hemisphere()
{
	if (count % num_samples == 0)
		jump = (rand() % num_sets) * num_samples;
	return (hemisphere_samples[jump + shuffled_indices[jump + count++ % num_samples]]);
}

void Sampler::map_samples_to_unit_disk()
{
	int size = samples.size();
	float r, phi;
	Point2D sp;

	// samples is range of (0, 1)
	disk_samples.reserve(num_samples * num_sets);
	for (int j = 0; j != size; j++)
	{
		sp.x = (float)(2.0 * samples[j].x - 1.0f);
		sp.y = (float)(2.0 * samples[j].y - 1.0f);

		if (sp.x > -sp.y) {			// sectors 1 and 2
			if (sp.x > sp.y) {		// sector 1
				r = sp.x;
				phi = sp.y / sp.x;
			}
			else {					// sector 2
				r = sp.y;
				phi = 2 - sp.x / sp.y;
			}
		}
		else {						// sectors 3 and 4
			if (sp.x < sp.y) {		// sector 3
				r = -sp.x;
				phi = 4 + sp.y / sp.x;
			}
			else {					// sector 4
				r = -sp.y;
				if (sp.y != 0.0)	// avoid division by zero at origin
					phi = 6 - sp.x / sp.y;
				else
					phi  = 0.0;
			}
		}
		phi *= (float)PI / 4.0f;

		disk_samples[j].x = r * cos(phi);
		disk_samples[j].y = r * sin(phi);
	}

	samples.erase(samples.begin(), samples.end());
}

void Sampler::map_samples_to_hemisphere(const float e)
{
	int size = samples.size();
	hemisphere_samples.reserve(num_samples * num_sets);
	for (int j = 0; j != size; j++)
	{
		double phi = 2.0 * PI * samples[j].x;
		float cos_phi = static_cast<float>(cos(phi));
		float sin_phi = static_cast<float>(sin(phi));	
		float cos_theta = pow((1.0f - samples[j].y), 1.0f / (e + 1.0f));
		float sin_theta = sqrt (1.0f - cos_theta * cos_theta);
		float pu = sin_theta * cos_phi;
		float pv = sin_theta * sin_phi;
		float pw = cos_theta;
		hemisphere_samples.push_back(Point3D(pu, pv, pw));
	}

	samples.erase(samples.begin(), samples.end());
}