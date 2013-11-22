#include "KxIntSliderGrp.h"

// Description:  Constructor
//
KxIntSliderGrp::KxIntSliderGrp(
                                     QWidget*        parent,
                                     bool            hasLabel,
                                     KxIntField*  newField,
                                     KxIntSlider* newSlider,
                                     bool            hasExtraLabel
                                     )
                                     : KxSliderGrp(parent, hasLabel, newField, newSlider, hasExtraLabel)
                                     , fIntField(newField)
                                     , fIntSlider(newSlider)
{
    //  Whenever the field or the slider emit a 'newValueForConnections'
    //  signal, have us emit one as well so that anyone connected directly
    //  to the group will get it.
    if (fIntField)
    {
        connect(
            fIntField,
            SIGNAL(newValueForConnections(const QVariant&, bool)),
            this,
            SIGNAL(newValueForConnections(const QVariant&, bool))
            );
    }

    if (fIntSlider)
    {
        connect(
            fIntSlider,
            SIGNAL(newValueForConnections(const QVariant&, bool)),
            this,
            SIGNAL(newValueForConnections(const QVariant&, bool))
            );
    }
}


void KxIntSliderGrp::newValueFromConnection(const QVariant& value)
{
    if (fIntField) fIntField->setValue(value.value<int>());
    if (fIntSlider) fIntSlider->setSliderValue(value);
}
