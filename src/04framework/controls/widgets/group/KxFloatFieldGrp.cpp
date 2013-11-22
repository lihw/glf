#include "KxFloatFieldGrp.h"

#include "../input/KxFloatField.h"


// Description:  Constructor
//
KxFloatFieldGrp::KxFloatFieldGrp(QWidget *parent)
: KxWidgetGrp(parent)
{}

// Description:  This procedure add a field to the group.
//
void KxFloatFieldGrp::addField(KxFloatField *newField)
{
    // Put the widget in the layout.
    //
    addWidgetToGrp(newField);

    // Keep a pointer to it on the list.
    //
    fFieldList.append(newField);
}

// Description:  This procedure returns the float fields in the
//               group.
//
void KxFloatFieldGrp::getFields( KxFloatField *(&X),
                                   KxFloatField *(&Y),
                                   KxFloatField *(&Z),
                                   KxFloatField *(&W))
{
    X = NULL; Y = NULL; Z = NULL; W = NULL;
    int numOfFields = fFieldList.count();
    if (numOfFields >= 1) X = fFieldList[0];
    if (numOfFields >= 2) Y = fFieldList[1];
    if (numOfFields >= 3) Z = fFieldList[2];
    if (numOfFields >= 4) W = fFieldList[3];
}
