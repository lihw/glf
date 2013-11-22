#include "KxFloatScrollBar.h"

// Description:  For float scroll bar, we
//      . store the float information in a structure,
//      . and create a corresponding physical integer scroll bar.
//
KxFloatScrollBar::KxFloatScrollBar(QWidget *parent)
: QScrollBar(parent)
, KxSliderFloatDataAbstraction(this)
, fDragInProgress(false)
, fIsUserAction(false)
, fOldValue(0.0)
{
    // don't use Qt context menus
    setContextMenuPolicy( Qt::PreventContextMenu );

    // When the displayed integer slider changes value, we need to
    // update the corresponding float value in the float data
    // representation.
    //
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(updateFloatDataAbstractionValue()));

    // We need to distinguish between slider changes initiated by the user
    // and those initiated by code, because only the latter should be
    // passed on to the connections.
    connect(this, SIGNAL(actionTriggered(int)), this, SLOT(flagUserAction(int)));

    // When the slider is released it may not emit a valueChanged() signal.
    // By watching for the slider release we can ensure that connections
    // are informed when a drag has completed.
    connect(this, SIGNAL(sliderReleased()), this, SLOT(dragComplete()));
}

// Description:  This function is called when the user interact
//               with the scroll bar, we need to change the float value 
//               in the float data abstraction.
//
void KxFloatScrollBar::updateFloatDataAbstractionValue()
{
    qreal  newValue = valueF();

    //  If the value has changed and it resulted from user action, pass it
    //  on to our connections.
    if (fIsUserAction && (newValue != fOldValue))
    {
        fOldValue = newValue;

        QVariant    value(newValue);

        fDragInProgress = isSliderDown();
        emit newValueForConnections(value, fDragInProgress);
        if(fDragInProgress) {
            emit transientSliderChange();
        }
    }

    fIsUserAction = false;
}


// Description: If a drag was in progress, let connections know that it
//              now complete.
void KxFloatScrollBar::dragComplete()
{
    qreal    newValue = valueF();

    //  If the value has changed or there was previously a drag in
    //  progress, let our connections know.
    if (fDragInProgress || (newValue != fOldValue))
    {
        fOldValue = newValue;

        QVariant    value(newValue);

        emit newValueForConnections(value, false);
        fDragInProgress = false;
    }
}


// Description: Update the scroll bar to reflect a new value sent to us by
//              one of our connections.
void KxFloatScrollBar::newValueFromConnection(const QVariant& newValue)
{
    if (newValue.canConvert<qreal>())
    {
        setValueF(newValue.value<qreal>(), true);
        fOldValue = valueF();
    }
}


// Description: Set a flag to indicate that the next value change is the
//              result of user action (as opposed to code internally
//              changing the slider value).
void KxFloatScrollBar::flagUserAction(int action)
{
    fIsUserAction = (action != QAbstractSlider::SliderNoAction);
}
