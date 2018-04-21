#include "utility.h"
#include <cstdlib>

float rand_float()
{
	int r_int = rand();
	return (float)r_int / RAND_MAX;
}