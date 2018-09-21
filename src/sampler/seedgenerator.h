#ifndef __SEEDGENERATOR_H__
#define __SEEDGENERATOR_H__

#include <wx/wx.h>

class SeedGenerator
{
private:
	SeedGenerator() {};
	~SeedGenerator();
	SeedGenerator(const SeedGenerator&);
	SeedGenerator& operator= (const SeedGenerator&);

	static wxMutex s_inst_mutex;
	static SeedGenerator* s_impl;
	static int s_seed;

public:
	static SeedGenerator& Instance();
	static int GetSeed();
};

#endif