#include "KxCheckBox.h"

KxCheckBox::KxCheckBox(const QString & text, QWidget *parent)
: QCheckBox(text, parent)
{
    // force RMB press event on context menu so that we can show the
    // context menu on press instead of on release
    setContextMenuPolicy(Qt::PreventContextMenu);

    // When such checkBox's state changed, we need to use the 
    // onOffChanged() function to trigger the
    // checkStateChangedToOn or checkStateChangedToOff signal,
    // which will in turn calls the onCommand or offCommand, respectively.
    // These two signals are newly defined in this class.  So we need 
    // specify how these signals are triggered. 
    //
    // Set up the on/off signal mechanism.
    //
    connect(this, SIGNAL(stateChanged(int)), this, SLOT(onOffChanged()));
}

// Description:
//
// QAbstractButton eats all mouse events on disabled buttons.  The comment
// with that code is that, unlike other widgets, disabled buttons accept
// mouse events and eating the events avoids surprising click-through scenarios.
//
// We do want to watch for one specific case here and that is when a user
// RMB clicks on a disabled button.  In this case we want to emit the menu
// requested signal.  One case this solves is when a check box attribute in the
// AE is locked.  The control becomes locked but we still want the signal to
// go out...which should pop up a menu that allows the user to unlock the
// attribute.
//
bool KxCheckBox::event(QEvent * e)
{
    if (!isEnabled() && 
        ((e != NULL) && 
        (e->type() == QEvent::MouseButtonPress)))
    {
        QMouseEvent * me = (QMouseEvent *)e;
        if (me->button() == Qt::RightButton)
        {
            emit contextMenuRequested(this, me);
            return false;
        }
    }

    return QCheckBox::event(e);
}

// Description:  Invoke context menu on RMB press
//
void KxCheckBox::mousePressEvent(QMouseEvent* event)
{
    if ((event != NULL) && (event->button() == Qt::RightButton))
    {
        emit contextMenuRequested(this, event);
    }

    QCheckBox::mousePressEvent(event);
}


// Description:
//      This function is called when the state of the check box is changed.
//      We want to look at the new state, and 
//      emit checkStateChangedToOn()/checkStateChangedToOff() signal.
//
void KxCheckBox::onOffChanged()
{
    Qt::CheckState state = checkState();
    QVariant       value = (state == Qt::Checked);

    emit newValueForConnections(value, false);

    if (state == Qt::Checked)
    {
        emit checkStateChangedToOn();
    }
    else if (state == Qt::Unchecked)
    {
        emit checkStateChangedToOff();
    }
}

void KxCheckBox::newValueFromConnection(const QVariant& newValue)
{
    bool blocked = blockSignals(true);
    setCheckState(newValue.toBool() ? Qt::Checked : Qt::Unchecked);
    blockSignals(blocked);
}

bool KxCheckBox::isReadOnly() const
{
    //  For now we treat readOnly the same as disabled.
    return !isEnabled();
}

void KxCheckBox::setReadOnly(bool readOnly)
{
    //  For now we treat readOnly the same as disabled.
    setEnabled(!readOnly);
}
