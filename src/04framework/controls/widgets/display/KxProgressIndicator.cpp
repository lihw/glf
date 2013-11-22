#include "KxProgressIndicator.h"

KxProgressIndicator::KxProgressIndicator()
: fIsInterruptable(false)
, fShowPercentText(false)
, fIsInUse(false)
{

}

KxProgressIndicator::~KxProgressIndicator()
{

}

void KxProgressIndicator::setIsInterruptable(bool isInterruptable)
{
    fIsInterruptable = isInterruptable;
}

bool KxProgressIndicator::isInterruptable() const
{
    return fIsInterruptable;
}

void KxProgressIndicator::setStatus(const QString & status)
{
    fStatus = status;
}

void KxProgressIndicator::setShowPercentText(bool show)
{
    fShowPercentText = show;
}

float KxProgressIndicator::percent() const 
{
    if(maxValue() != minValue()){
        return 100 * progress() / (maxValue() - minValue()) ;
    }else 
        return 100;
}

const QString & KxProgressIndicator::status() const 
{
    return fStatus;
}

void KxProgressIndicator::start()
{
    fIsInUse = true;
}

void KxProgressIndicator::finish()
{
    fIsInUse = false;
    setProgress(minValue());
    setIsInterruptable(false);
}

