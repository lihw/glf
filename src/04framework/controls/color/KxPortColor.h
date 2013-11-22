#ifndef KxPortColor_H_
#define KxPortColor_H_

#include "../base/HQtGui.h"

#define kNumActiveBits		5
#define kNumDormantBits	5
#define kNumTemplateBits	1


#define kNumTemplateColors		(1<<(kNumTemplateBits))
#define kNumDormantColors		((1<<(kNumDormantBits)) - 1)
#define kNumActiveColors		((1<<(kNumActiveBits)) - 1)
#define kTemplatePlaneBase		0
#define kDormantPlaneBase		(kTemplatePlaneBase + kNumTemplateColors)
#define kActivePlaneBase		(kDormantPlaneBase + kNumDormantColors)
#define kDormantToActiveAdjustment (kActivePlaneBase - kDormantPlaneBase)

#define kNuserDefinedColors	8
#define kUserDefinedColorBase \
    (kDormantPlaneBase + kNumDormantColors - kNuserDefinedColors)


class KX_DLLEXPORT  KxPortColor {
public:
    enum KxPortColorType { kRGB, kHSV, kCMY, kCMYK };

    //	Constructors
    KxPortColor		();

    // deliberately no destructor as it would have to be virtual, but
    // there is an API class (MColor) for this class and that causes
    // all kinds of problems with virtual function tables since it
    // assumes API and internal classes are the same size and start
    // with the first data member
    //  ~KxPortColor()

    KxPortColor		(const KxPortColor &color);
    KxPortColor		(float, float, float, float alpha=1.0);
    KxPortColor		(KxPortColorType, int, int, int, 
        int alpha = kIntMax);
    KxPortColor		(KxPortColorType, float, float, float,
        float alpha=1.0);
    KxPortColor		(KxPortColorType, double, double, double,
        double alpha=1.0);

    //	Access methods
    void			get	(float &, float &, float &) const;
    void			get	(float &, float &, float &, float &alpha) const;
    void			set	(float, float, float, float alpha=1.0);

    void			set (KxPortColorType,
        int, int, int, int alpha);
    void			set (KxPortColorType,
        float, float, float, float alpha=1.0);
    void			set (KxPortColorType,
        double, double, double, double alpha=1.0);

    void			get (KxPortColorType, 
        int &, int &, int &) const;
    void			get (KxPortColorType, 
        int &, int &, int &, int &alpha) const;
    void			get (KxPortColorType,
        float &, float &, float &) const;
    void			get (KxPortColorType,
        float &, float &, float &, float &alpha) const;
    void			get (KxPortColorType,
        double &, double &, double &) const;
    void			get (KxPortColorType,
        double &, double &, double &, double &alpha) const;

    int	asXColor () const;
    int int32rgba () const;
    int int32argb () const;
    int int32abgr () const;
    static void		setDepth (int);

    //	Operators
    KxPortColor &	operator =  (const KxPortColor &);
    bool		operator == (const KxPortColor &) const;
    bool		operator != (const KxPortColor &) const;
    bool		equivalent  (const KxPortColor&,float esp=kFloatEpsilon) const;
    bool		isTransparent () const;

    static int		transparentIndex();
    static void		transparentIndex(int idx);

    KxPortColor		operator +  (const KxPortColor &) const;
    KxPortColor		operator -  (const KxPortColor &) const;
    KxPortColor &	operator += (const KxPortColor &);
    KxPortColor &	operator -= (const KxPortColor &);
    KxPortColor		operator *  (const KxPortColor &) const;
    KxPortColor		operator /  (const KxPortColor &) const;
    KxPortColor &	operator *= (const KxPortColor &);
    KxPortColor &	operator /= (const KxPortColor &);
    KxPortColor &	operator ^= (const KxPortColor &);
    KxPortColor		operator ^  (const KxPortColor &);

    KxPortColor		operator +  (float) const;
    KxPortColor		operator -  (float) const;
    KxPortColor &	operator += (float);
    KxPortColor &	operator -= (float);
    KxPortColor		operator *  (float) const;
    KxPortColor		operator /  (float) const;
    KxPortColor &	operator *= (float);
    KxPortColor &	operator /= (float);
    friend	inline	KxPortColor operator * (float f, const KxPortColor &c);
    friend			KxPortColor operator / (float f, const KxPortColor &c);

    float &		operator []	(int i);
    float	 		operator []	(int i) const;

public:	//	Data
    float			red, green, blue, alpha;
    static			const KxPortColor kTransparent;

private:
    void			toHSV(float &h, float &s, float &v) const;
    void			fromHSV(float h, float s, float v);
    void			toRGBA (float rgba[4]) const;
    void			fromRGBA (const float rgba[4]);
    void			toCMYK (float &c, float &m, float &y, float &k) const;
    void			fromCMYK (float c, float m, float y, float k);

    static int	fsDepth;
    static int		fsTransparentIndex;
};


//////////////////////////////////////////////////////////////////////
//	Some simple colors (mainly for debugging)
//////////////////////////////////////////////////////////////////////

extern KX_DLLEXPORT const KxPortColor kCtransparent;
extern KX_DLLEXPORT const KxPortColor kCblack;
extern KX_DLLEXPORT const KxPortColor kCblue;
extern KX_DLLEXPORT const KxPortColor kCgreen;
extern KX_DLLEXPORT const KxPortColor kCdarkgreen;
extern KX_DLLEXPORT const KxPortColor kCcyan;
extern KX_DLLEXPORT const KxPortColor kCred;
extern KX_DLLEXPORT const KxPortColor kCmagenta;
extern KX_DLLEXPORT const KxPortColor kCyellow;
extern KX_DLLEXPORT const KxPortColor kCwhite;
extern KX_DLLEXPORT const KxPortColor kCgrey;

enum
{
    kItransparent,
    kIred,
    kIblack,
    kIwhite
};

//////////////////////////////////////////////////////////////////////
//	Inlines
//////////////////////////////////////////////////////////////////////

inline KxPortColor::KxPortColor (KxPortColorType t,
                               int c1, int c2, int c3, int a)
{
    set(t, c1, c2, c3, a);
}


inline KxPortColor::KxPortColor (KxPortColorType t,
                               float c1, float c2, float c3, float a)
{
    set(t, c1, c2, c3, a);
}

inline KxPortColor::KxPortColor (KxPortColorType t,
                               double c1, double c2, double c3, double a)
{
    set(t, c1, c2, c3, a);
}

inline void KxPortColor::get (float &r, float &g, float &b) const
{
    r = red; g = green; b = blue;
}

inline void KxPortColor::get (float &r, float &g, float &b, float &a) const
{
    r = red; g = green; b = blue; a = alpha;
}

inline void KxPortColor::set (float r, float g, float b, float a)
{
    red= r; green = g; blue = b; alpha = a;
}

// Helper methods
//

inline bool KxPortColor::operator == (const KxPortColor &rhs) const
//
//	Description:
//		Returns true if the colours are equal. (ignoring alpha)
//
{
    return (red == rhs.red
        && green == rhs.green
        && blue == rhs.blue
        && alpha == rhs.alpha);
}

inline KxPortColor & KxPortColor::operator = (const KxPortColor &c)
{
    red = c.red; green = c.green; blue = c.blue; alpha = c.alpha;
    return *this;
}

inline bool KxPortColor::operator != (const KxPortColor &c) const
{ 
    return !(*this == c);
}

inline int KxPortColor::transparentIndex()
{
    return fsTransparentIndex;
}

inline void KxPortColor::transparentIndex(int idx)
{
    fsTransparentIndex = idx;
}

inline float & KxPortColor::operator [] (int i)
{
    return (i == 0 ? red : (i == 1 ? green : (i == 2 ? blue : alpha)));
}

inline float KxPortColor::operator [] (int i) const
{
    return (i == 0 ? red : (i == 1 ? green : (i == 2 ? blue : alpha)));
}

inline KxPortColor KxPortColor::operator + (const KxPortColor &c) const
{
    KxPortColor result(*this); result += c; return result;
}

inline KxPortColor KxPortColor::operator - (const KxPortColor &c) const
{
    KxPortColor result(*this); result -= c; return result;
}

inline KxPortColor KxPortColor::operator * (const KxPortColor &c) const
{
    KxPortColor result(*this); result *= c; return result;
}

inline KxPortColor KxPortColor::operator / (const KxPortColor &c) const
{
    KxPortColor result(*this); result /= c; return result;
}

inline KxPortColor KxPortColor::operator ^  (const KxPortColor &c)
{
    KxPortColor result(*this); result ^= c; return result;
}

inline KxPortColor KxPortColor::operator + (float c) const
{
    KxPortColor result(*this); result += c; return result;
}

inline KxPortColor KxPortColor::operator - (float c) const
{
    KxPortColor result(*this); result -= c; return result;
}

inline KxPortColor KxPortColor::operator * (float c) const
{
    KxPortColor result(*this); result *= c; return result;
}

inline KxPortColor KxPortColor::operator / (float c) const
{
    KxPortColor result(*this); result /= c; return result;
}



inline /*friend*/ KxPortColor operator * (float f, const KxPortColor &c)
{
    return (c * f);
}

inline void KxPortColor::setDepth(int depth)
{
    fsDepth = depth;
}

#endif