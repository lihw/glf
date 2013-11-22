#include "KxIntField.h"

#define kDeltaValue  10

// Description:  Constructor
//
KxIntField::KxIntField(QWidget *parent)
: KxNumericField(parent)
, fDelta(kDeltaValue)
{
    QIntValidator *v = new QIntValidator(this);
    if (v != NULL)
    {
        v->setTop(kIntMax);
        v->setBottom(kIntMin);
        setValidator(v);
    }
    setValue(value());
}

// Description:  Set the field to the given integer value. 
//
void KxIntField::setValue(int n)
{
    if (!text().isEmpty() && value() == n) 
    {
        return;
    }
    QString intString;
    intString.setNum(n);
    setText(intString);
    emit numericValueChanged();
}

// Description:  Return the integer value in the field.
//               If the input field is empty, then 0 is returned.
//
int KxIntField::value() const
{
    return text().toInt(); 
}

// Description:  This procedure sets the min valid value.
//
void KxIntField::setMinValue(int min)
{
    QIntValidator *v = (QIntValidator *)validator();
    if (v != NULL)
    {
        v->setBottom(min);
    }
}

// Description:  This procedure return the min value.
//
int KxIntField::minValue() const
{
    const QIntValidator *v = (const QIntValidator *)validator();
    if (v != NULL)
    {
        return v->bottom();
    }
    return kIntMin;
}

// Description:  This procedure sets the max valid value.
//
void KxIntField::setMaxValue(int max)
{
    QIntValidator *v = (QIntValidator *)validator();
    if (v != NULL)
    {
        v->setTop(max);
    }
}

// Description:  This procedure return the max value.
//
int KxIntField::maxValue() const
{
    const QIntValidator *v = (const QIntValidator *)validator();
    if (v != NULL)
    {
        return v->top();
    }
    return kIntMax;
}

// Description:  This procedure sets the delta for invisio-slider.
//
void KxIntField::setDelta(int newDelta)
{
    if (newDelta <= 0)
    {
        return;
    }

    int max = maxValue(), min = minValue();
    int range = max - min;
    if (newDelta > range)
    {
        newDelta = range;
    }
    fDelta = newDelta;
}


void KxIntField::newValueFromConnection(const QVariant& value)
{
    if (value.canConvert<int>())
        setValue(value.value<int>());
}


