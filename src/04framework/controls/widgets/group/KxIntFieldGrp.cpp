#include "KxIntFieldGrp.h"

#include "../input/KxIntField.h"

// Description:  Constructor
//
KxIntFieldGrp::KxIntFieldGrp(QWidget *parent)
: KxWidgetGrp(parent)
{
}

// Description:  This procedure add a field to the group.
//
void KxIntFieldGrp::addField(KxIntField *newField)
{
    // Put the widget in the layout.
    //
    addWidgetToGrp(newField);

    // Keep a pointer to it on the list.
    //
    fFieldList.append(newField);
}

// Description:  This procedure returns the integer fields in the
//               group.
//
void KxIntFieldGrp::getFields(KxIntField *(&X),
                                 KxIntField *(&Y),
                                 KxIntField *(&Z),
                                 KxIntField *(&W))
{
    X = NULL; Y = NULL; Z = NULL; W = NULL;
    int numOfFields = fFieldList.count();
    if (numOfFields >= 1) X = fFieldList[0];
    if (numOfFields >= 2) Y = fFieldList[1];
    if (numOfFields >= 3) Z = fFieldList[2];
    if (numOfFields >= 4) W = fFieldList[3];
}

// Description:  This procedure is called to set the value of
//               a field, where the field is specified by the
//               given index.
//
void KxIntFieldGrp::setValue(int index, int newValue)
{
    int numOfFields = fFieldList.count();
    if (index < 0 || index > numOfFields)
    {
        return;
    }
    KxIntField * field = fFieldList[index];
    if (field != NULL)
    {
        field->setValue(newValue);
    }
}

// Description:  This procedure returns the value in the i_th
//               field, where i = index.
//
int KxIntFieldGrp::value(int index) const
{
    int numOfFields = fFieldList.count();
    if (index < 0 || index > numOfFields)
    {
        return 0;
    }
    KxIntField * field = fFieldList[index];
    if (field != NULL)
    {
        return field->value();
    }
    return 0; 
}
