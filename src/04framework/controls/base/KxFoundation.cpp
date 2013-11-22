#include "KxFoundation.h"

float KxFoundation::clamp(float value, float low, float high)
{
	if(value < low)
		return low;
	if(value > high)
		return high;
	return value;
}

bool KxFoundation::equivalent(float x, float y, float fudge /* = kFloatEpsilon */ )
{
	return ((x > y) ? (x - y <= fudge) : (y - x <= fudge));
}

float KxFoundation::max(float a, float b, float c)
{
	if(a <= b){
		if(c >= b)
			return c;
		else 
			return b;
	}else{
		if(c >= a)
			return c;
		else 
			return a;
	}
}

float KxFoundation::min(float a, float b, float c)
{
	if(a <= b){
		if(c <= a)
			return c;
		else
			return a;
	}else{
		if(c <= b)
			return c;
		else
			return b;
	}
}

float KxFoundation::max(float a, float b)
{
	return a > b ? a : b;
}

float KxFoundation::min(float a, float b)
{
	return a < b ? a: b;
}

