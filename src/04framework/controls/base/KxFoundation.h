#ifndef KxFoundation_H_
#define KxFoundation_H_
#include "HQtGui.h"
#define kFloatMin  -3.402823466e+38F
#define kFloatMax  3.402823466e+38F
#define kIntMax  2147483647 
#define kIntMin   (-2147483647 - 1)

#define		kFloatEpsilon		1.0e-5F
#define     kUInt8Max 0xff 
#define     kUInt16Max 0xffff 

#define kFieldDelta		10
#define kDefaultDigits	1
#define kFieldWidth		80
#define kLabelWidth		100


class KX_DLLEXPORT KxFoundation 
{
public:
	static float clamp(float value, float low, float high);
	static bool equivalent(float x, float y, float fudge  = kFloatEpsilon );
	static float max(float a, float b, float c);
	static float min(float a, float b, float c);
	static float max(float a, float b);
	static float min(float a, float b);
};
#endif