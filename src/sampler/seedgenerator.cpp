#include "seedgenerator.h"

SeedGenerator* SeedGenerator::s_impl = NULL;
int SeedGenerator::s_seed = 0;
wxMutex SeedGenerator::s_inst_mutex;

SeedGenerator::~SeedGenerator()
{
	if (s_impl)
		delete s_impl;
}

SeedGenerator& SeedGenerator::Instance()
{
	if (!s_impl)
	{
		wxMutexLocker locker(s_inst_mutex);
		if (!s_impl)
		{
			s_impl = new SeedGenerator();
			return *s_impl;
		}
	}
	return *s_impl;
}

int SeedGenerator::GetSeed()
{
	wxMutexLocker locker(s_inst_mutex);
	return s_seed++;
}