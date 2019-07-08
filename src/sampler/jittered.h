#ifndef __JITTERED_H__
#define __JITTERED_H__

#include "sampler.h"

class Jittered: public Sampler
{
public:
	Jittered(void);								
	Jittered(const int ns);					
	Jittered(const int ns, const int n_sets);	
	Jittered(const Jittered& js);					
	Jittered& operator= (const Jittered& rhs);
	virtual ~Jittered();
	virtual Sampler* Clone() const;
	virtual void GenerateSamples();
};

#endif