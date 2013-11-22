#include "KxPortColor.h"
//
//
//	Class:	KxPortColor
//
//	Author: Brent McPherson
//
//	Description:
//		KxPortColor encapsulates an RGBA color value and provides a
//		way to operate on RGB colors and convert them to other
//		color spaces.
//


/*static*/ const KxPortColor KxPortColor::kTransparent(0.0, 0.0, 0.0, 0.0);
/*static*/ int KxPortColor::fsDepth = 24;
/*static*/ int KxPortColor::fsTransparentIndex = 255;

KxPortColor::KxPortColor()
//
//	Description:
//		Constructor
//
:	red(0.0), green(0.0), blue(0.0), alpha(1.0)
{}

KxPortColor::KxPortColor (const KxPortColor &c)
//
//	Description:
//		Constructor
//
:	red(c.red), green(c.green), blue(c.blue), alpha(c.alpha)
{}

KxPortColor::KxPortColor (float r, float g, float b, float a)
//
//	Description:
//		Constructor
//
:	red(r), green(g), blue(b), alpha(a)
{}



void KxPortColor::set (KxPortColorType t,
                      int c1, int c2, int c3, int a)
                      //
                      //	Description:
                      //		Set a color using unsigned data in range [0-65535].
                      //
{
    set(t, 
        float(c1)/kIntMax, float(c2)/kIntMax, 
        float(c3)/kIntMax, float(a)/kIntMax
        );
}

void KxPortColor::set (KxPortColorType t,
                      double c1, double c2, double c3, double a)
                      //
                      //	Description:
                      //		Set a color using double precision data in range [0-1.0].
                      //
{
    set(t, float(c1), float(c2), float(c3), float(a));
}

void KxPortColor::set (KxPortColorType colorType,
                      float c1, float c2, float c3, float a)
                      //
                      //	Description:
                      //		Set a color using float data in range [0-1.0].
                      //
{
    switch (colorType) {
    case kRGB:
        set(c1, c2, c3, a);
        break;
    case kHSV:
        fromHSV(c1, c2, c3);
        alpha = a;
        break;
    case kCMY:
        set(1.0f-c1, 1.0f-c2, 1.0f-c3, a);
        break;
    case kCMYK:
        fromCMYK(c1, c2, c3, a);
        break;
    }
}


void KxPortColor::get (KxPortColorType t,
                      int &c1, int &c2, int &c3, int &al) const
                      //
                      //	Description:
                      //		Return color as unsigned data in range [0-255].
                      //
{
    float x=0.0f, y=0.0f, z=0.0f, a=0.0f;
    get (t, x, y, z, a); 
	c1 = int(x*kUInt16Max);
	c2 = int(y*kUInt16Max);
	c3 = int(z*kUInt16Max);
	al = int(a*kUInt16Max);
}

void KxPortColor::get (KxPortColorType t,
                      int &c1, int &c2, int &c3) const
                      //
                      //	Description:
                      //		Return color as unsigned data in range [0-65535].
                      //
{
    int tmp=kIntMax;
    get(t, c1, c2, c3, tmp);
}


void KxPortColor::get (KxPortColorType t,
                      double &c1, double &c2, double &c3) const
                      //
                      //	Description:
                      //		Return color as double precision data in range [0-1.0].
{
    double tmp=1.0;
    get(t, c1, c2, c3, tmp);
}

void KxPortColor::get (KxPortColorType t,
                      double &c1, double &c2, double &c3, double &al) const
                      //
                      //	Description:
                      //		Return color as double precision data in range [0-1.0].
                      //
{
    float x=0.0f, y=0.0f, z=0.0f, a=0.0f;
    get (t, x, y, z, a); 
    c1 = x; c2 = y; c3 = z; al = a;
}

void KxPortColor::get (KxPortColorType t,
                      float &c1, float &c2, float &c3) const
                      //
                      //	Description:
                      //		Return color as single precision data in range [0-1.0].
                      //
{
    float tmp=1.0f;
    get (t, c1, c2, c3, tmp);
}

void KxPortColor::get (KxPortColorType colorType,
                      float &c1, float &c2, float &c3, float &a) const

                      //
                      //	Description:
                      //		Return color as single precision data in range [0-1.0].
                      //
{
    switch (colorType) {
    case kRGB:
        get(c1, c2, c3, a);
        break;
    case kHSV:
        toHSV(c1, c2, c3);
        a = alpha;
        break;
    case kCMYK:
        toCMYK(c1, c2, c3, a);
        break;
    case kCMY:
        c1 = 1.0f - red;
        c2 = 1.0f - green;
        c3 = 1.0f - blue;
        a  = alpha;
        break;
    }
}

//int KxPortColor::asXColor () const
////
////	Description:
////		Return color formatted for use with X11, ignored alpha
////
//{
//    const float colorScale = 255.0;
//
//#if defined(OSWin_)
//
//    int redColor = (int)(KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale);
//    int greenColor = (int)(KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale);
//    int blueColor = (int)(KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale);
//
//    // make sure we don't get floating point approximations that
//    // cause us to overflow.
//    return RGB(redColor, greenColor, blueColor);
//#elif defined(OSUnix_)
//    int redColor, greenColor, blueColor;
//    if(fsDepth == 24) {
//#if defined(OSLinux_)
//        redColor	= (int)(KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale) << 16;
//        greenColor	= (int)(KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale) << 8;
//        blueColor	= (int)(KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale);
//#else
//        redColor	= (int)(KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale);
//        greenColor	= (int)(KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale) << 8;
//        blueColor	= (int)(KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale) << 16;
//#endif
//    }else{
//        Tint32 shift = fsDepth/3;
//        redColor	= (int)(KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale) << (shift - 8);
//        greenColor	= (int)(KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale) << ((shift*2) - 8);
//        blueColor	= (int)(KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale) << ((shift*3) - 8);
//    }
//    return (redColor + greenColor + blueColor);
//#elif defined(OSMac_)	// Mac is ARGB
//
//    int alphaColor = (int)(KxFoundation::clamp(alpha, 0.0f, 1.0f) * colorScale) << 24;
//    int redColor = (int)(KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale) << 16;
//    int greenColor = (int)(KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale) << 8;
//    int blueColor = (int)(KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale);
//
//    return ( alphaColor + redColor + greenColor + blueColor);
//#endif
//}

int KxPortColor::int32rgba () const
//
//	Description:
//		Return RGBA8 format, for texture
//
{
#ifdef AL_LITTLE_ENDIAN
    
    // Currently no platform we supported is little endian
    //  Byte order may not correct for little endian machine
#endif

    const float colorScale = 255.0f;

    int redColor = (int)(KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale) << 24;
    int greenColor = (int)(KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale) << 16;
    int blueColor = (int)(KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale) << 8;
    int alphaColor = (int)(KxFoundation::clamp(alpha, 0.0f, 1.0f) * colorScale);

    return ( redColor + greenColor + blueColor + alphaColor );
}

int KxPortColor::int32argb () const
//
//	Description:
//		Return ARGB8 format, for texture
//
{
#ifdef AL_LITTLE_ENDIAN
   
    // Currently no platform we supported is little endian
    // Byte order may not correct for little endian machine
#endif

    const float colorScale = 255.0f;

    int alphaColor = (int)(KxFoundation::clamp(alpha, 0.0f, 1.0f) * colorScale) << 24;
    int redColor = (int)(KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale) << 16;
    int greenColor = (int)(KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale) << 8;
    int blueColor = (int)(KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale);

    return ( alphaColor + redColor + greenColor + blueColor);
}

int KxPortColor::int32abgr () const
//
//	Description:
//		Return ABGR8 format, for texture
//
{
#ifdef AL_LITTLE_ENDIAN
    
    // Currently no platform we supported is little endian
    // Byte order may not correct for little endian machine
#endif

    const float colorScale = 255.0f;

    int alphaColor = (int)(KxFoundation::clamp(alpha, 0.0f, 1.0f) * colorScale) << 24;
    int blueColor = (int)(KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale) << 16;
    int greenColor = (int)(KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale) << 8;
    int redColor = (int)(KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale) << 0;

    return ( alphaColor + blueColor + greenColor + redColor);
}

bool KxPortColor::equivalent  (const KxPortColor &rhs, float /* tol */) const
//
//	Description:
//		Return true if colors are quivalent within given tolerance
//		(ignoring alpha component)
//
{
    return (
        KxFoundation::equivalent(red, rhs.red) && 
        KxFoundation::equivalent(green, rhs.green) &&
        KxFoundation::equivalent(blue, rhs.blue)
        );
}

bool KxPortColor::isTransparent () const
{
    return KxFoundation::equivalent(alpha, 0.0f);
}


KxPortColor& KxPortColor::operator ^= (const KxPortColor &c)
// 
{
    const float colorScale = 255.0;
    unsigned char redColor, greenColor, blueColor;
    unsigned char color1, color2;

    color1 = (unsigned char)( KxFoundation::clamp(red, 0.0f, 1.0f) * colorScale);
    color2 = (unsigned char)( KxFoundation::clamp(c.red, 0.0f, 1.0f) * colorScale);
    redColor = color1 ^ color2;
    color1 = (unsigned char)( KxFoundation::clamp(blue, 0.0f, 1.0f) * colorScale);
    color2 = (unsigned char)( KxFoundation::clamp(c.blue, 0.0f, 1.0f) * colorScale);
    blueColor = color1 ^ color2;
    color1 = (unsigned char)( KxFoundation::clamp(green, 0.0f, 1.0f) * colorScale);
    color2 = (unsigned char)( KxFoundation::clamp(c.green, 0.0f, 1.0f) * colorScale);
    greenColor = color1 ^ color2;

    red		=	(float)redColor/colorScale;
    blue	=	(float)blueColor/colorScale;
    green	=	(float)greenColor/colorScale;
    return *this;
}



KxPortColor & KxPortColor::operator += (const KxPortColor &c)
//
//	Description:
//		Add colors together (ignoring alpha component)
//
{
    red   = red + c.red;
    green = green + c.green;
    blue  = blue + c.blue;
    return *this;
}

KxPortColor & KxPortColor::operator -= (const KxPortColor &c)
//
//	Description:
//		Subtract two colors (ignoring alpha component)
//
{
    red   = red - c.red;
    green = green - c.green;
    blue  = blue - c.blue;
    return *this;
}

KxPortColor & KxPortColor::operator *= (const KxPortColor &c)
//
//	Description:
//		Multiply colors together (ignoring alpha component)
//
{
    red   = red * c.red ;
    green = green * c.green;
    blue  = blue * c.blue;
    return *this;
}

KxPortColor & KxPortColor::operator /= (const KxPortColor &c)
//
//	Description:
//		Divide two colors (ignoring alpha component)
//
{
    Q_ASSERT(!KxFoundation::equivalent(c.red, float(0.0)));
    Q_ASSERT(!KxFoundation::equivalent(c.green, float(0.0)));
    Q_ASSERT(!KxFoundation::equivalent(c.blue, float(0.0)));

    red   = red / c.red;
    green = green / c.green;
    blue  = blue / c.blue;
    return *this;
}

KxPortColor & KxPortColor::operator += (float f)
//
//	Description:
//		Add constant to a color (ignoring alpha component)
//
{
    red   = red + f;
    green = green + f;
    blue  = blue + f;
    return *this;
}

KxPortColor & KxPortColor::operator -= (float f)
//
//	Description:
//		Subtract constant from a color (ignoring alpha component)
//
{
    red   = red - f;
    green = green - f;
    blue  = blue - f;
    return *this;
}

KxPortColor & KxPortColor::operator *= (float f)
//
//	Description:
//		Multiply constant by a color (ignoring alpha component)
//
{
    red   = red * f;
    green = green * f;
    blue  = blue * f;
    return *this;
}

KxPortColor & KxPortColor::operator /= (float f)
//
//	Description:
//		Divide color by a constant (ignoring alpha component)
//
{
    Q_ASSERT(!KxFoundation::equivalent(f, 0.0f));

    red   = red / f;
    green = green / f;
    blue  = blue / f;
    return *this;
}

/*friend*/ KxPortColor operator / (float f, const KxPortColor &c)
//
//	Description:
//		Divide constant by color (ignoring alpha component)
//
{
    Q_ASSERT(!KxFoundation::equivalent(c.red, 0.0f));
    Q_ASSERT(!KxFoundation::equivalent(c.green,0.0f));
    Q_ASSERT(!KxFoundation::equivalent(c.blue, 0.0f));

    return KxPortColor(f / c.red, f / c.green, f / c.blue, c.alpha);
}


//////////////////////////////////////////////////////////////////////
//	Internal conversion routines
//////////////////////////////////////////////////////////////////////

void KxPortColor::toRGBA (float rgba[4]) const
//
//	Description:
//		Returns the GL RGBA KxFoundation::equivalent to this colour.
//
{
    rgba[0] = red;
    rgba[1] = green;
    rgba[2] = blue;
    rgba[3] = alpha;
}


void KxPortColor::fromRGBA (const float rgba[4])
//
//	Description:
//		Makes this colour KxFoundation::equivalent to GL RGBA.
//
{
    red	  = rgba[0];
    green = rgba[1];
    blue  = rgba[2];
    alpha = rgba[3];
}

void KxPortColor::toHSV(float &hue, float &sat, float &lum) const
//
//	Description:
//		Get color as hue, saturation and luminosity.
//
{
    hue = 0.0;
    sat	= 1.0;
	lum	= KxFoundation::max(red, green, blue);
	float x = KxFoundation::min(red, green, blue);

    if (!KxFoundation::equivalent(lum, 0.0F)) {
        float lumMinusX = lum - x;
        sat = lumMinusX / lum;
        if (sat != 0.0) {
            if(!KxFoundation::equivalent(lumMinusX, 0.0F))
            {
                float tempr = (lum - red)   / lumMinusX;
                float tempg = (lum - green) / lumMinusX;
                float tempb = (lum - blue)  / lumMinusX;

                if (red == lum)
                    hue = (green == x ? 5 + tempb : 1 - tempg);
                else if (green == lum)
                    hue = (blue == x ? 1 + tempr : 3 - tempb);
                else
                    hue = (red == x ? 3 + tempg : 5 - tempr);
                hue *= 60;
                if (hue < 0) hue += 360.0;
            }
        }
    }
    //	Make colours really close to black become greyscale colors
    if (lum < kFloatEpsilon) sat = 0.0;
}

void KxPortColor::fromHSV(float hue, float sat, float lum)
//
//	Description:
//		Set color form hue, saturation and luminosity.
//
{
    if (hue == 360.0) hue = 0;
    hue /= 60.0;
    float frac = hue - (int)hue;
    float lx = lum * (1 - sat);
    float ly = lum * (1 - sat * frac);
    float lz = lum * (1 - sat * (1 - frac));

    switch ((int)hue) {
    case 0: case 6:
        red = lum; green = lz; blue = lx; break;
    case 1:
        red = ly; green = lum; blue = lx; break;
    case 2:
        red = lx; green = lum; blue = lz; break;
    case 3:
        red = lx; green = ly; blue = lum; break;
    case 4:
        red = lz; green = lx; blue = lum; break;
    case 5:
        red = lum; green = lx; blue = ly; break;
    }
}

void KxPortColor::toCMYK (float &c, float &m, float &y, float &k) const
//
//	Description:
//		Return CMYK values for this color.
//
{
    c = 1.0f - red;
    m = 1.0f - green;
    y = 1.0f - blue;
    k = 1.0f;
    if (c < k) k = c;
    if (m < k) k = m;
    if (y < k) k = y;
    c = c - k;
    m = m - k;
    y = y - k;
}

void KxPortColor::fromCMYK (float c, float m, float y, float k)
//
//	Description:
//		Set color from CMYK values.
//
{
    red   = 1.0f - c - k;
    green = 1.0f - m - k;
    blue  = 1.0f - y - k;
    if (red < 0.0f)   red   = 0.0f;
    if (green < 0.0f) green = 0.0f;
    if (blue < 0.0f)  blue  = 0.0f;
}

//////////////////////////////////////////////////////////////////////
//	Pre-defined colors
//////////////////////////////////////////////////////////////////////

const KxPortColor kCtransparent	(0.0, 0.0, 0.0, 0.0);
const KxPortColor kCblack		(0.0, 0.0, 0.0);
const KxPortColor kCblue			(0.0, 0.0, 1.0);
const KxPortColor kCgreen		(0.0, 1.0, 0.0);
const KxPortColor kCdarkgreen	(0.0, 0.4, 0.0);
const KxPortColor kCcyan			(0.0, 1.0, 1.0);
const KxPortColor kCred			(1.0, 0.0, 0.0);
const KxPortColor kCmagenta		(1.0, 0.0, 1.0);
const KxPortColor kCyellow		(1.0, 1.0, 0.0);
const KxPortColor kCwhite		(1.0, 1.0, 1.0);
const KxPortColor kCgrey			(0.5, 0.5, 0.5);
