#include "hammersley.h"
#include "utilities/point2d.h"

Hammersley::Hammersley()
	: Sampler()
{
	generate_samples();
}

Hammersley::Hammersley(const int ns)
	: Sampler(ns)
{
	generate_samples();
}

Hammersley::Hammersley(const int ns, const int n_sets)
	: Sampler(ns, n_sets)
{
	generate_samples();
}

Hammersley::Hammersley(const Hammersley& js)
	: Sampler(js)
{
	generate_samples();
}

Hammersley& Hammersley::operator= (const Hammersley& rhs)
{
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

Hammersley::~Hammersley() {}

void Hammersley::generate_samples()
{
	for (int p = 0; p != num_sets; p++)
	{
		for (int j = 0; j != num_samples; j++)
		{
			Point2D pv((float)j / (float)num_samples, (float)phi(j));
			samples.push_back(pv);
		}
	}
}

double Hammersley::phi(int j)
{
	double x = 0.0;
	double f = 0.5;
	while(j)
	{
		x += f * (double)(j & 0x1);
		j = j >> 1;
		f *= 0.5;
	}

	return x;
}

Sampler* Hammersley::clone() const
{
	return new Hammersley(*this);
}