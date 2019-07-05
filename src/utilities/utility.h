#ifndef __UTILITY_H__
#define __UTILITY_H__

// rand from [0,1]
float RandFloat();

const double 	PI 			= 3.1415926535897932384;
const double 	TWO_PI 		= 6.2831853071795864769;
const double 	PI_ON_180 	= 0.0174532925199432957;
const double 	invPI 		= 0.3183098861837906715;
const double 	invTWO_PI 	= 0.1591549430918953358;
const float		kEpsilon	= 0.001f;

#define FLT_MAX         3.402823466e+38F

inline double Clamp(double value, double min, double max)
{
	if (value < min)
		return min;
	if (value > max)
		return max;
	return value;
}

#endif