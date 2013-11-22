#include "KxIntScrollBar.h"

//
// Class:  KxIntScrollBar
// Description:  Implement float scroll bar as a subclass of QScrollBar.
//

KxIntScrollBar::KxIntScrollBar(QWidget *parent)
: QScrollBar(parent)
, KxSliderDataAbstraction(this)
, fDragInProgress(false)
, fIsUserChange(false)
, fOldValue(0)
{
    // don't use Qt context menus
    setContextMenuPolicy( Qt::PreventContextMenu );

    // When the slider changes value due to user action, we need to pass
    // the new value to our connections.
    connect(
        this, SIGNAL(valueChanged(int)),
        this, SLOT(sendUserChangeToConnections(int))
        );

    // We need to distinguish between slider changes initiated by the user
    // and those initiated by code, because only the latter should be
    // passed on to the connections.
    connect(
        this, SIGNAL(actionTriggered(int)),
        this, SLOT(flagUserChange(int))
        );

    // When the slider is being dragged the release may not emit a
    // valueChanged() signal.  By watching for the slider release we can
    // ensure that connections are informed when a drag has completed.
    connect(this, SIGNAL(sliderReleased()), this, SLOT(dragComplete()));
}


// Description:  This function is called when the user interact
//      with the scroll bar, we need to change the float value 
//      in the float data abstraction.
//
void KxIntScrollBar::sendUserChangeToConnections(int newValue)
{
    //	If the value has changed and it resulted from user action, pass it
    //	on to our connections.
    if (fIsUserChange && (newValue != fOldValue))
    {
        fOldValue = newValue;

        QVariant	value(newValue);

        fDragInProgress = isSliderDown();
        emit newValueForConnections(value, fDragInProgress);
        if(fDragInProgress) {
            emit transientSliderChange();
        }
    }

    fIsUserChange = false;
}


// Description: If a drag was in progress, let connections know that it
//				now complete.
void KxIntScrollBar::dragComplete()
{
    int newValue = value();

    //	If the value has changed or there was previously a drag in
    //	progress, let our connections know. (In other words, we are
    //	excluding the case where the user clicked and released the slider
    //	without moving it at all.)
    if (fDragInProgress || (newValue != fOldValue))
    {
        fOldValue = newValue;

        QVariant	value(newValue);

        emit newValueForConnections(value, false);
        fDragInProgress = false;
    }
}


// Description: Update the scroll bar to reflect a new value sent to us by
//				one of our connections.
void KxIntScrollBar::newValueFromConnection(const QVariant& newValue)
{
    if (newValue.canConvert<int>())
    {
        fOldValue = newValue.value<int>();
        setValueF(fOldValue);
    }
}


// Description: Set a flag to indicate that the next value change is the
//				result of user action (as opposed to code internally
//				changing the slider value).
void KxIntScrollBar::flagUserChange(int action)
{
    fIsUserChange = (action != QAbstractSlider::SliderNoAction);
}
