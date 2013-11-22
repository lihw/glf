#include "KxTextFieldGrp.h"

#include "../input/KxField.h"

#define kFieldName 						"field"

// Description:  Constructor
//
KxTextFieldGrp::KxTextFieldGrp(QWidget *parent, bool hasLabel)
: KxWidgetGrp(parent)
{
    if (hasLabel)
    {
        // Add a label.
        //
        addLabel();
    } 

    // Add a text field.
    // 
    fField = new KxField(this);
    fField->setObjectName(kFieldName);
    addWidgetToGrp(fField);
}
