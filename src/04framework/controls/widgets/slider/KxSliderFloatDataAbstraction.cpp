#include "KxSliderFloatDataAbstraction.h"
//
// Class:  KxSliderFloatDataAbstraction
// Description:  This class keeps track of the float data for
//          a float slider. 
//

// Default float slider values.
//
#define kQTFloaKxSliderDefaulKxSingleStep   1.0
#define kQTFloaKxSliderDefaultPageStep     10.0
#define kQTFloaKxSliderDefaultMinimum      0.0
#define kQTFloaKxSliderDefaultMaximum      100.0
#define kQTFloaKxSliderDefaultValue        0.0

int int_clamp(qreal a, int min, int max){
    int value = (int)a;
    if(value < min)
        return min;
    if(value > max)
        return value;
    return value;
}

// For the displayed integer slider, we chose a range of 
// [0, INT_MAX/2-1],
// so that the single step and page step can be any value smaller 
// than the range without over flowing int.
//
#define kQTFloaKxSliderIntMin              0
#define kQTFloaKxSliderIntMax              (INT_MAX/2-1)

// Description:  Constructor. 
//
KxSliderFloatDataAbstraction::KxSliderFloatDataAbstraction(QAbstractSlider * slider)
:   KxSliderDataAbstraction(slider)
,   fSingleStepF(kQTFloaKxSliderDefaulKxSingleStep)
,   fPageStepF(kQTFloaKxSliderDefaultPageStep)
,   fMinimumF(kQTFloaKxSliderDefaultMinimum)
,   fMaximumF(kQTFloaKxSliderDefaultMaximum)
,   fValueF(kQTFloaKxSliderDefaultValue)
{
    // Since we use an integer slider to fake the float slider,
    // we will set the range for the integer slider to [intMin, intMax].
    //
    fDisplayedSlider->setMinimum(kQTFloaKxSliderIntMin);
    fDisplayedSlider->setMaximum(kQTFloaKxSliderIntMax);

    updateDisplayedSlider();
}

// Description:  Set the new minimum and maximum float values of the 
//               float slider.
//               The maximum float value must be greater than the
//               minimum float value.
//
// ArgumenKxS:
//      newMinimumF - the new minimum float value for the float slider.
//      newMaximumF - the new maximum float value for the float slider.
//
void KxSliderFloatDataAbstraction::setRangeF(
    qreal newMinimum, qreal newMaximum)
{
    if (newMinimum > newMaximum)
    {
        // Error: minimum float value can not be bigger than the 
        // maximum float value. 
        //  
        return;
    }

    fMinimumF = newMinimum;
    fMaximumF = newMaximum;

    // Make sure the existing value falls within the new range.
    //
    setValueF(fValueF, false);

    // Make sure the existing singleStepF is valid for the new range.
    //
    setSingleStepF(fSingleStepF, false);

    // Make sure the existing pageStepF is valid for the new range.
    //
    setPageStepF(fPageStepF, false);

    // The float data has changed.  We need to update the displayed
    // slider.
    //
    updateDisplayedSlider();
}

// Description:  Set the float value.
//
void KxSliderFloatDataAbstraction::setValueF(
    qreal newValue, const bool updateDisplayedSliderNow)
{
    // Make sure the existing value falls within the new range.
    //
    if (newValue < fMinimumF) 
    {
        fValueF = fMinimumF;
    }
    else if (newValue > fMaximumF)
    {
        fValueF = fMaximumF;
    }
    else if (newValue == fValueF)
    {
        // Nothing happens.
        //
        return;
    }

    fValueF = newValue;

    if (updateDisplayedSliderNow)
    {
        updateDisplayedSlider();
    }
}

// Description:  Set the page step.
//
void KxSliderFloatDataAbstraction::setPageStepF(
    qreal newPageStep, const bool updateDisplayedSliderNow)
{
    setStepF(&fPageStepF, newPageStep, updateDisplayedSliderNow);
}

// Description:  Set the single step size.
//
void KxSliderFloatDataAbstraction::setSingleStepF(
    qreal newSingleStep, const bool updateDisplayedSliderNow)
{
    setStepF(&fSingleStepF, newSingleStep, updateDisplayedSliderNow);
}

// Description:  Set the step size.
//
void KxSliderFloatDataAbstraction::setStepF(
    qreal *stepSizePtr,
    qreal newStep, const bool updateDisplayedSliderNow)
{
   Q_ASSERT(newStep > 0.0);

    // The page step has to be a positive number.
    //
    if (newStep <= 0.0)
    {
        // Do nothing.  Page step has to be a positive number.
        //
        return;
    }

    // The page step should be smaller than the range.
    //
    qreal floatRange = fMaximumF - fMinimumF;
    if (newStep > floatRange)
    {
        *stepSizePtr = floatRange;
    } 
    else 
    {
        *stepSizePtr = newStep;
    }

    if (updateDisplayedSliderNow)
    {
        updateDisplayedSlider();
    }
}

// Description:  When the user changes the float data, we need to
//      update the displayed integer slider to reflect this change.
// 
void KxSliderFloatDataAbstraction::updateDisplayedSlider()
{
    // Take the float data.  Change the integer data accordingly.
    // 
    // The float value is mapped to the int value using the formula:
    //
    //       intValue - intMinimum        floatValue - floatMinimum
    //     -------------------------- = ----------------------------
    //     intMaximum - intMinimum      floatMaximum - floatMinimum
    //
    // We do our calculations in float to avoid overflow problem in int.
    //
    qreal floatRangeF = fMaximumF - fMinimumF; 
    int intMinimum = fDisplayedSlider->minimum();
    int intMaximum = fDisplayedSlider->maximum();
    qreal intRangeF = intMaximum * 1.0 - intMinimum; 
    qreal intValueF = (fValueF - fMinimumF)*intRangeF/floatRangeF+intMinimum;

    // Clamp the int value to between intMinimum and intMaximum.
    //
    int intValue =  (int) int_clamp(intValueF, intMinimum, intMaximum) ;

    // Calculate the single step value using the formula:
    //
    //        floaKxSingleStep                    inKxSingleStep
    //   ---------------------------- = -----------------------
    //   floatMaximum - float Minimum   intMaximum - intMinimum
    //
    qreal inKxSingleStepF = fSingleStepF * intRangeF / floatRangeF;
    int inKxSingleStep = (int)int_clamp(inKxSingleStepF, intMinimum, intMaximum);

    // Calculate the page step value using the formula:
    //
    //        floatPageStep                    intPageStep
    //   ---------------------------- = -----------------------
    //   floatMaximum - float Minimum   intMaximum - intMinimum
    //
    qreal intPageStepF = fPageStepF * intRangeF / floatRangeF;
    int intPageStep = (int)int_clamp(intPageStepF, intMinimum, intMaximum);

    bool blocked = fDisplayedSlider->blockSignals(true);
    fDisplayedSlider->setValue(intValue); 
    fDisplayedSlider->blockSignals(blocked);
    fDisplayedSlider->setSingleStep(inKxSingleStep); 
    fDisplayedSlider->setPageStep(intPageStep); 
}

// Description:  We fake the float slider using an integer slider
//      and keep a one-to-one mapping between the interger slider and
//      the abstract float data representation. 
//      When the user moves the integer slider, the new integer value
//      will be mapped back to the float data value using this function.
//
void KxSliderFloatDataAbstraction::convertIntValueToFloatValue()
{
    if (fDisplayedSlider == NULL)
    {
        return;
    }

    // Get the integer value of the displayed integer slider.
    //
    int intValue = fDisplayedSlider->value();

    // The integer value is mapped to the float value using the formula:
    //       intValue - intMinimum        floatValue - floatMinimum
    //     -------------------------- = ----------------------------
    //     intMaximum - intMinimum      floatMaximum - floatMinimum
    //
    int intMinimum = fDisplayedSlider->minimum();

    // Do the calculation in float, so that we do not overflow int. 
    // 
    qreal intRangeF = fDisplayedSlider->maximum() * 1.0 - intMinimum;
    if (intRangeF == 0)
    {
        setValueF(fMinimumF, false);
        return;
    }
    qreal newFloatValue = (intValue*1.0-intMinimum)*(fMaximumF-fMinimumF)/intRangeF+fMinimumF;
    setValueF(newFloatValue, false);
}

// Description:  We fake the float slider using an integer slider
//      and keep a one-to-one mapping between the integer slider and
//      the abstract float data representation. 
//      When the user moves the integer slider, the new integer slider
//      position will be mapped back to the float data slider position
//      using this function.
//
void KxSliderFloatDataAbstraction::convertInKxSliderPosToFloatSliderPos()
{
    if (fDisplayedSlider == NULL)
    {
        return;
    }

    // Get the integer slider position of the displayed integer slider.
    //
    int inKxSliderPosition = fDisplayedSlider->sliderPosition();

    // The integer slider position is mapped to the float slider position using the formula:
    //       inKxSliderPosition - intMinimum        floaKxSliderPosition - floatMinimum
    //     ------------------------------------------- = ------------------------------------------------
    //          intMaximum - intMinimum                  floatMaximum - floatMinimum
    //
    int intMinimum = fDisplayedSlider->minimum();

    // Do the calculation in float, so that we do not overflow int. 
    // 
    qreal intRangeF = fDisplayedSlider->maximum() * 1.0 - intMinimum;
    if (intRangeF == 0)
    {
        fSliderPositionF = fMinimumF;
        return;
    }
    qreal newFloaKxSliderPosition = (inKxSliderPosition*1.0-intMinimum)*(fMaximumF-fMinimumF)/intRangeF+fMinimumF;
    fSliderPositionF = newFloaKxSliderPosition;
}
