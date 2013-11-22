#ifndef  KxFloatSlider_H_
#define  KxFloatSlider_H_
#include "../../base/HQtGui.h"
#include "KxSliderFloatDataAbstraction.h"
#include "KxSlider.h"
class KX_DLLEXPORT KxFloatSlider : public KxSlider, public KxSliderFloatDataAbstraction
{
    Q_OBJECT

public:
    KxFloatSlider(Qt::Orientation orientation = Qt::Horizontal, QWidget * parent = 0);

    virtual void    getSliderValue(QVariant& value);
    virtual double sliderValueF() ;
    virtual void    setSliderValue(const QVariant& value);
    virtual void    setSliderValueF(double) ;
    virtual double sliderMinimumF() const ;
    virtual double sliderMaximumF() const ;
    virtual void    setSliderRangeF(double, double) ;
};


inline void KxFloatSlider::getSliderValue(QVariant& value)
{
    convertIntValueToFloatValue();
    value = sliderValueF();
}

inline double KxFloatSlider::sliderValueF()
{
    return valueF();
}

inline void KxFloatSlider::setSliderValue(const QVariant& newValue)
{
    if (newValue.canConvert<qreal>())
        setValueF(newValue.value<qreal>());
}

inline void KxFloatSlider::setSliderValueF(double newValue)
{
    setValueF(newValue);
}

inline double KxFloatSlider::sliderMinimumF() const
{
    return minimumF();
}

inline double KxFloatSlider::sliderMaximumF() const
{
    return maximumF();
}

inline void KxFloatSlider::setSliderRangeF(double min, double max)
{
    return setRangeF(min, max);
}



#endif