#include "KxFloatField.h"

#define kNumberDigits 4 
#define kValueDelta 0.001f 

KxFloatField::KxFloatField(QWidget *parent /* = 0 */)
:KxNumericField(parent)
,fDeltaF(kValueDelta)
,fNumDecimals(kNumberDigits)
{
    // Create a float validator to control the editing of the field
    // (we don't set precision here since we want the user to be 
    // able to edit to any float value they want)
    fRangeValidator = new QDoubleValidator(this);

    if(fRangeValidator != NULL){
        fRangeValidator->setTop(kFloatMax);
        fRangeValidator->setBottom(kFloatMin);
        setValidator(fRangeValidator);
    }
    setValueF(valueF());
}

QSize KxFloatField::sizeHint() const
{
    // Set the default width. This helps when the parent layout
    // does not set the width for the field
    return QSize(60,KxNumericField::sizeHint().height());
}



void KxFloatField::setMinValueF(double min)
{
    QDoubleValidator *v = (QDoubleValidator *) validator();
    if(v != NULL){
        v->setBottom(min);
    }
}

double KxFloatField::minValueF()const 
{
    QDoubleValidator *v = (QDoubleValidator *)validator();
    if(v != NULL){
        return v->bottom();
    }
    return kFloatMin;
}

void KxFloatField::setMaxValueF(double max)
{
    QDoubleValidator *v = (QDoubleValidator *) validator();
    if(v != NULL){
        v->setTop(max);
    }
}

double KxFloatField::maxValueF()const 
{
    QDoubleValidator * v = (QDoubleValidator *)validator();
    if(v != NULL){
        return v->top();
    }
    return kFloatMax;
}


void KxFloatField::setValueF(double value)
{
    fInputValueF = value;
    QString valueString;
    valueString.setNum(value,'f',(int)fNumDecimals);
    if(!text().isEmpty() && text() == valueString){
        return;
    }
    fOldText = text();
    setText(valueString);
    
    emit numericValueChanged();
}


double KxFloatField::valueF() const 
{
    return text().toDouble();
}

double KxFloatField::inputValueF() const
{
    return fInputValueF;
}

void KxFloatField::setDeltaF(double newDelta)
{
    if(newDelta <= 0.0)
        return;

    double range = maxValueF() - minValueF();
    if(newDelta > range)
        newDelta = range;
    fDeltaF = newDelta;
}


void KxFloatField::setDecimals(int d)
{
    unsigned int oldDecimals = fNumDecimals;
    fNumDecimals = (unsigned int )d;
    if(oldDecimals > fNumDecimals){
        // reformat the current number in the input field      
        setValueF(valueF()); 
    }
}

int KxFloatField::decimals()const 
{
    return (int)fNumDecimals;
}


void KxFloatField::newValueFromConnection(const QVariant &value)
{
    if(value.canConvert<double>()){
        setValueF(value.value<double>());
        fOldText = text();
    }
}


void  KxFloatField::mouseDoubleClickEvent(QMouseEvent *ev)
{
    selectAll();
}




