#include "utility.h"
#include <cstdlib>
#include <ctime>

float RandFloat()
{
	int r_int = rand();
	return (float)r_int / RAND_MAX;
}