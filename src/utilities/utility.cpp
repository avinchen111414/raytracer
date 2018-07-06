#include "utility.h"
#include <cstdlib>
#include <ctime>

float rand_float()
{
	int r_int = rand();
	return (float)r_int / RAND_MAX;
}