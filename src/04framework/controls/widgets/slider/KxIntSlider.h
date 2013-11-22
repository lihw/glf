#ifndef  KxIntSlider_H_
#define  KxIntSlider_H_
#include "KxSlider.h"
#include "KxSliderDataAbstraction.h"

class KX_DLLEXPORT KxIntSlider : public KxSlider, public KxSliderDataAbstraction
{
    Q_OBJECT

public:
    KxIntSlider(Qt::Orientation orientation, QWidget *parent = 0) 
        : KxSlider(orientation, parent), KxSliderDataAbstraction(this) {};

    virtual void    getSliderValue(QVariant& value);
    virtual double sliderValueF() ;
    virtual void    setSliderValue(const QVariant& value);
    virtual void    setSliderValueF(double) ;
    virtual double sliderMinimumF() const ;
    virtual double sliderMaximumF() const ;
    virtual void    setSliderRangeF(double, double) ;
};

inline void KxIntSlider::getSliderValue(QVariant& outValue)
{
    outValue = value();
}

inline double KxIntSlider::sliderValueF()
{
    return value();
}

inline void KxIntSlider::setSliderValue(const QVariant& newValue)
{
    if (newValue.canConvert<int>()) {
        setValueF(newValue.value<int>());
    }
}

inline void KxIntSlider::setSliderValueF(double newValue)
{
    setValueF((int)newValue);
}

inline double KxIntSlider::sliderMinimumF() const
{
    return minimum();
}

inline double KxIntSlider::sliderMaximumF() const
{
    return maximum();
}

inline void KxIntSlider::setSliderRangeF(double min, double max)
{
    return setRange((int)min, (int)max);
}
#endif