#include "KxIconTextCheckBox.h"
#include "../../base/KxQtHelper.h"


// Description:  Constructor
//
KxIconTextCheckBox::KxIconTextCheckBox(QWidget *parent)
: QPushButton(parent)
{
    setFlat(true);
    setCheckable(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, sizePolicy().controlType()));
    fIconTextControl = new KxIconTextControl(this);
    connect(this, SIGNAL(clicked(bool)), this, SLOT(onOffChanged()));
}

void KxIconTextCheckBox::onOffChanged()
{
    iconTextControl()->setSelected(isChecked());

    emit newValueForConnections(isChecked(), false);

    if(isChecked()) {
        emit checkStateChangedToOn();
    } else {
        emit checkStateChangedToOff();
    }
}

void KxIconTextCheckBox::newValueFromConnection(const QVariant& newValue)
{
    bool blocked = blockSignals(true);
    bool isOn = newValue.toBool();
    setChecked(isOn);
    iconTextControl()->setSelected(isOn);
    blockSignals(blocked);
}

QSize KxIconTextCheckBox::sizeHint() const
{
    if(fIconTextControl->style() == KxIconTextControl::textOnly) {
        return QSize(fIconTextControl->sizeHint().width(), QPushButton::sizeHint().height());
    } else {
        return fIconTextControl->sizeHint();
    }
}

void KxIconTextCheckBox::resizeEvent(QResizeEvent * event)
{
    fIconTextControl->resize(size());
}

//void KxIconTextCheckBox::setChangedCommand(Taction * action)
//{
//    KxQtHelper::linkControlSignalToScriptAction(SIGNAL(clicked(bool)), action, "changedCommand", this);
//}
//
//Taction *KxIconTextCheckBox::changedCommand() const
//{
//    return (Taction *)KxQtHelper::commandScript("changedCommand", const_cast<KxIconTextCheckBox*>(this));
//}
//
//void KxIconTextCheckBox::setOnCommand(Taction * action)
//{
//    KxQtHelper::linkControlSignalToScriptAction(SIGNAL(checkStateChangedToOn()), action, "onCommand", this);
//}
//
//Taction *KxIconTextCheckBox::onCommand() const
//{
//    return (Taction *)KxQtHelper::commandScript("onCommand", const_cast<KxIconTextCheckBox*>(this));
//}
//
//void KxIconTextCheckBox::setOffCommand(Taction * action)
//{
//    KxQtHelper::linkControlSignalToScriptAction(SIGNAL(checkStateChangedToOff()), action, "offCommand", this);
//}
//
//Taction *KxIconTextCheckBox::offCommand() const
//{
//    return (Taction *)KxQtHelper::commandScript("offCommand", const_cast<KxIconTextCheckBox*>(this));
//}

// ------ Event processing : single vs double click ----------------
void KxIconTextCheckBox::mouseReleaseEvent(QMouseEvent * event)
{
    QPushButton::mouseReleaseEvent(event);
}

// -----------------------------------------------------------------

KxIconTextControl *KxIconTextCheckBox::iconTextControl()
{
    return fIconTextControl;
}

