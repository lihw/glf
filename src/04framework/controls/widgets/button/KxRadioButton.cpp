#include "KxRadioButton.h"

// Description:  Constructor
//
KxRadioButton::KxRadioButton(const QString &text, QWidget *parent)
: QRadioButton(text, parent)
{
    connect(this, SIGNAL(toggled(bool)), this, SLOT(onOffChanged(bool)));
}

// Description:
//  This function is called when the state of the radio button 
//  is changed.  We want to look at the new state, and
//  emit checkStateChangedToOn()/checkStateChangedToOff() signal.
//
void KxRadioButton::onOffChanged(bool checked)
{
    if (checked)
    {
        emit checkStateChangedToOn();
    }
    else
    {
        emit checkStateChangedToOff();
    }
}


bool KxRadioButton::isReadOnly() const
{
    //  For now we treat readOnly the same as disabled.
    return !isEnabled();
}


void KxRadioButton::setReadOnly(bool readOnly)
{
    //  For now we treat readOnly the same as disabled.
    setEnabled(!readOnly);
}
