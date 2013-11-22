#ifndef KxSliderDataAbstraction_H_
#define KxSliderDataAbstraction_H_

#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxSliderDataAbstraction 
{
public:
    KxSliderDataAbstraction(QAbstractSlider * slider);
    virtual ~KxSliderDataAbstraction() {};

    virtual void setValueF(qreal newValue, const bool updateDisplayedSliderNow = true);

protected:
    QAbstractSlider * fDisplayedSlider;
};
#endif