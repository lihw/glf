#include "KxSliderDataAbstraction.h"
//
// Class:  KxSliderDataAbstraction
// Description:  This class keeps track of the float data for
//          a float slider. 
//

#include "../../base/HQtGui.h"
#include "KxSliderDataAbstraction.h"

// Description:  Constructor. 
//
KxSliderDataAbstraction::KxSliderDataAbstraction(QAbstractSlider * slider)
:   fDisplayedSlider(slider)
{
}

// Description:  Set the float value.
//
void KxSliderDataAbstraction::setValueF(
                                       qreal newValue, const bool updateDisplayedSliderNow)
{
    bool blocked = fDisplayedSlider->blockSignals(true);
    fDisplayedSlider->setValue((int)newValue);
    fDisplayedSlider->blockSignals(blocked);
}
