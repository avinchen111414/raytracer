#include <cmath>
#include "jittered.h"
#include "utilities/utility.h"


Jittered::Jittered()
	: Sampler()
{
	generate_samples();
}

Jittered::Jittered(const int ns)
	: Sampler(ns)
{
	generate_samples();
}

Jittered::Jittered(const int ns, const int n_sets)
	: Sampler(ns, n_sets)
{
	generate_samples();
}

Jittered::Jittered(const Jittered& js)
	: Sampler(js)
{
	generate_samples();
}

Jittered& Jittered::operator= (const Jittered& rhs)
{
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

Jittered::~Jittered() {}

void Jittered::generate_samples()
{
	int n = (int) sqrt((float)num_samples);
	for (int p = 0; p != num_sets; p++)
	{
		for (int j = 0; j != n; j++)
		{
			for (int k = 0; k != n; k++)
			{
				Point2D sp((k + rand_float()) / n, (j + rand_float()) / n);
				samples.push_back(sp);
			}
		}
	}
}

Sampler* Jittered::clone() const
{
	return (new Jittered(*this));
}