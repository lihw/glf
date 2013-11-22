#include "KxSlider.h"


// Description:  Constructor.
//
KxSlider::KxSlider(Qt::Orientation orientation, QWidget *parent)
: QSlider(orientation, parent) 
, fDragInProgress(false)
, fIsUserAction(false)
, fOldRawValue(0)
{
    // The sliderChangeCompleted signal is triggered by the following
    // events cases: 
    //
    // case I:
    //     a user released a slider after dragging it or
    //
    connect(this, SIGNAL(sliderReleased()), this, SLOT(dragComplete())); 
    //
    // case II:
    //     non-dragging slider value changing event, i.e.,
    //
    //     pageUp/pageDown key      |
    //     up_arrow/down_arrow key  | + valueChanged signal
    //     home/end key             |
    //     mouse click on slider bar|
    //     mouse_wheel              |
    //
    //     The first half of this action triggers actionTriggered signal.
    //     We use the flagUserAction slot to determine whether the
    //     action was initiated by the user (in which case we will later
    //     send out signals) or internally (in which case we won't).
    //     At that point, the value of the slider is still the
    //     old value.  The new value has not been propagated yet. 
    //     We wait for valueChanged signal, then handle the
    //     non-drag user interaction in handleSliderValueChanged.
    //     This allows us to use the correct new slider value.
    //
    connect(this, SIGNAL(actionTriggered(int)), this, SLOT(flagUserAction(int)));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(handleSliderValueChanged(int)));
}

// Description:  If a drag has completed, let everyone know about it.
//
void KxSlider::dragComplete()
{
    if (fDragInProgress)
    {
        fDragInProgress = false;

        QVariant newValue;
        getSliderValue(newValue);

        emit newValueForConnections(newValue, false);
        emit sliderChangeCompleted();
    }
}

// Description:  This slot handles valueChanged signal. 
//               
void KxSlider::handleSliderValueChanged(int newRawValue)
{
    //  If the user has changed the value, let everyone know.
    if (fIsUserAction && (newRawValue != fOldRawValue))
    {
        fOldRawValue = newRawValue;

        QVariant  newValue;
        getSliderValue(newValue);

        fDragInProgress = isSliderDown();

        if (fDragInProgress)
        {
            //  The user is in the middle of a drag so the current value is
            //  just a transient one.
            emit newValueForConnections(newValue, true);
            emit transientSliderChange();
        }
        else
        {
            //  The user either completed a drag or else performed some
            //  atomic non-drag action such as clicking in the slider
            //  groove or using the keyboard. So this is a final value.
            emit newValueForConnections(newValue, false);
            emit sliderChangeCompleted();
        }
    }

    fIsUserAction = false;
}


// Description: Update the slider to reflect a new value sent to us by
//              one of our connections.
void KxSlider::newValueFromConnection(const QVariant& newValue)
{
    setSliderValue(newValue);
    fOldRawValue = value();
}

// Description: Set the slider to be pressed before handling a mouse press
//              event, as QSlider::mousePressEvent will set the value before
//              setting the slider to be pressed, causing it to always be
//              counted as a final value, instead of as a potential precursor
//              to a drag action.
void KxSlider::mousePressEvent(QMouseEvent * event)
{
    setSliderDown(true);
    QSlider::mousePressEvent(event);
}

// Description: Set the slider to be unpressed if it is still set as pressed
//              after handling QSlider::mouseReleaseEvent (just in case it
//              didn't undo our setSliderDown(true) call above).
void KxSlider::mouseReleaseEvent(QMouseEvent * event)
{
    QSlider::mouseReleaseEvent(event);
    if(isSliderDown() && event->buttons() == Qt::NoButton) {
        setSliderDown(false);
    }
}

// Description: Ignore mouse wheel events, this causes unwanted slider value changes
//              when slider is under a scroll layout (e.g. AE)
void KxSlider::wheelEvent(QWheelEvent * event)
{
    event->ignore();
    return;
}

// Description: Set a flag to indicate that the next value change is the
//              result of user action (as opposed to code internally
//              changing the slider value).
void KxSlider::flagUserAction(int action)
{
    fIsUserAction = (action != QAbstractSlider::SliderNoAction);
}

