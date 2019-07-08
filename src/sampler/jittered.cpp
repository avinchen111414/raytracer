#include <cmath>
#include "jittered.h"
#include "utilities/utility.h"


Jittered::Jittered()
	: Sampler()
{
	GenerateSamples();
}

Jittered::Jittered(const int ns)
	: Sampler(ns)
{
	GenerateSamples();
}

Jittered::Jittered(const int ns, const int n_sets)
	: Sampler(ns, n_sets)
{
	GenerateSamples();
}

Jittered::Jittered(const Jittered& js)
	: Sampler(js)
{
	GenerateSamples();
}

Jittered& Jittered::operator= (const Jittered& rhs)
{
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

Jittered::~Jittered() {}

void Jittered::GenerateSamples()
{
	int n = (int) sqrt((float)num_samples); 
	for (int p = 0; p < num_sets; p++)
		for (int j = 0; j < n; j++)		
			for (int k = 0; k < n; k++) {
				Point2D sp((k + RandFloat()) / n, (j + RandFloat()) / n);				
				samples.push_back(sp);
			}
}

Sampler* Jittered::Clone() const
{
	return (new Jittered(*this));
}