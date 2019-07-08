#include "hammersley.h"
#include "utilities/point2d.h"

Hammersley::Hammersley()
	: Sampler()
{
	GenerateSamples();
}

Hammersley::Hammersley(const int ns)
	: Sampler(ns)
{
	GenerateSamples();
}

Hammersley::Hammersley(const int ns, const int n_sets)
	: Sampler(ns, n_sets)
{
	GenerateSamples();
}

Hammersley::Hammersley(const Hammersley& js)
	: Sampler(js)
{
	GenerateSamples();
}

Hammersley& Hammersley::operator= (const Hammersley& rhs)
{
	if (this == &rhs)
		return (*this);

	Sampler::operator= (rhs);

	return (*this);
}

Hammersley::~Hammersley() {}

void Hammersley::GenerateSamples()
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

Sampler* Hammersley::Clone() const
{
	return new Hammersley(*this);
}