#include "KxFloatSlider.h"


// Description:  For float slider, we 
//      . store the float information in a structure
//      . and create a corresponding physical integer slider. 
//
KxFloatSlider::KxFloatSlider(Qt::Orientation orientation, QWidget * parent)
: KxSlider(orientation, parent)
, KxSliderFloatDataAbstraction(this)
{
}

