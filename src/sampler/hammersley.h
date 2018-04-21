#ifndef __HAMMERSLEY_H__
#define __HAMMERSLEY_H__

#include "sampler.h"

class Hammersley: public Sampler
{
public:
	Hammersley(void);								
	Hammersley(const int ns);					
	Hammersley(const int ns, const int n_sets);	
	Hammersley(const Hammersley& js);					
	Hammersley& operator= (const Hammersley& rhs);

	virtual ~Hammersley();

	virtual Sampler* clone() const;

private:
	virtual void generate_samples();
	double phi(int j);
};


#endif // !__HAMMERSLEY_H__
