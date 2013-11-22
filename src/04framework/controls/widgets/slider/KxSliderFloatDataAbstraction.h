#ifndef KxSliderFloatDataAbstraction_H_
#define KxSliderFloatDataAbstraction_H_
#include "KxSliderDataAbstraction.h"

class KX_DLLEXPORT KxSliderFloatDataAbstraction : public KxSliderDataAbstraction
{
public:
    KxSliderFloatDataAbstraction(QAbstractSlider * slider);

    qreal minimumF() const;
    qreal maximumF() const;
    qreal valueF();
    qreal sliderPositionF();
    qreal singleStepF() const;
    qreal pageStepF() const;

    void  setValueF(qreal newValue, const bool updateDisplayedSliderNow = true);
    void  setSingleStepF(qreal singleStep, const bool updateDisplayedSliderNow = true);
    void  setPageStepF(qreal pageStep, const bool updateDisplayedSliderNow = true);
    void  setRangeF(qreal min, qreal max);

protected:
    void  convertIntValueToFloatValue();
    void  convertInKxSliderPosToFloatSliderPos();

private:
    void  updateDisplayedSlider();
    void  setStepF(qreal *stepPtr, qreal singleStep, 
        const bool updateDisplayedSliderNow = true);

    qreal fMinimumF;
    qreal fMaximumF;
    qreal fValueF;
    qreal fSliderPositionF;
    qreal fSingleStepF;
    qreal fPageStepF;
};

// Description:  Return the minimum of the float slider.
//
inline qreal KxSliderFloatDataAbstraction::minimumF() const
{
    return fMinimumF;
}

// Description:  Return the maximum of the float slider.
//
inline qreal KxSliderFloatDataAbstraction::maximumF() const
{
    return fMaximumF;
}

// Description:  Return the value of the float slider.
//
inline qreal KxSliderFloatDataAbstraction::valueF()
{
    // Make sure the value is current.  The sliderMoved signal
    // triggers update float value action for sure, but it also
    // triggers dragCommand.  In case the dragCommand checks
    // the float value, we need to make sure it is up to date.
    //
    convertIntValueToFloatValue();
    return fValueF;
}

// Description:  Return the slider position of the float slider.
//
inline qreal KxSliderFloatDataAbstraction::sliderPositionF()
{
    // Obtain the current slider position and convert it to the
    // appropriate float slider position each time to ensure the
    // value is up to date.
    //
    convertInKxSliderPosToFloatSliderPos();
    return fSliderPositionF;
}

// Description:  Return the single step value of the float slider.
//               Typically, it corresponds to the user pressing an
//               arrow key. 
//
inline qreal KxSliderFloatDataAbstraction::pageStepF() const
{
    return fPageStepF;
}

// Description:  Return the single step value of the float slider.
//               Typically, it corresponds to the user pressing an
//               arrow key. 
//
inline qreal KxSliderFloatDataAbstraction::singleStepF() const
{
    return fSingleStepF;
}
#endif