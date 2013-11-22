#include "KxIconTextRadioButton.h"
#include "../../base/KxQtHelper.h"


// Description:  Constructor
//
KxIconTextRadioButton::KxIconTextRadioButton(QWidget *parent)
: QPushButton(parent)
{
    setFlat(true);
    setCheckable(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, sizePolicy().controlType()));
    fIconTextControl = new KxIconTextControl(this);
    connect(this, SIGNAL(toggled(bool)), this, SLOT(onOffChanged()));
}

void KxIconTextRadioButton::onOffChanged()
{
    iconTextControl()->setSelected(isChecked());
    if(isChecked()) {
        emit checkStateChangedToOn();
    } else {
        emit checkStateChangedToOff();
    }
}

QSize KxIconTextRadioButton::sizeHint() const
{
    if(fIconTextControl->style() == KxIconTextControl::textOnly) {
        return QSize(fIconTextControl->sizeHint().width(), QPushButton::sizeHint().height());
    } else {
        return fIconTextControl->sizeHint();
    }
}

void KxIconTextRadioButton::resizeEvent(QResizeEvent * event)
{
    fIconTextControl->resize(size());
}

//void KxIconTextRadioButton::setChangedCommand(Taction * action)
//{
//    KxQtHelper::linkControlSignalToScriptAction(SIGNAL(toggled(bool)), action, "changedCommand", this);
//}
//
//Taction *KxIconTextRadioButton::changedCommand() const
//{
//    return (Taction *)KxQtHelper::commandScript("changedCommand", const_cast<KxIconTextRadioButton*>(this));
//}
//
//void KxIconTextRadioButton::setOnCommand(Taction * action)
//{
//    KxQtHelper::linkControlSignalToScriptAction(SIGNAL(checkStateChangedToOn()), action, "onCommand", this);
//}
//
//Taction *KxIconTextRadioButton::onCommand() const
//{
//    return (Taction *)KxQtHelper::commandScript("onCommand", const_cast<KxIconTextRadioButton*>(this));
//}
//
//void KxIconTextRadioButton::setOffCommand(Taction * action)
//{
//    KxQtHelper::linkControlSignalToScriptAction(SIGNAL(checkStateChangedToOff()), action, "offCommand", this);
//}
//
//Taction *KxIconTextRadioButton::offCommand() const
//{
//    return (Taction *)KxQtHelper::commandScript("offCommand", const_cast<KxIconTextRadioButton*>(this));
//}

// ------ Event processing : single vs double click ----------------
void KxIconTextRadioButton::mouseReleaseEvent(QMouseEvent * event)
{
    QPushButton::mouseReleaseEvent(event);
}

// -----------------------------------------------------------------

KxIconTextControl *KxIconTextRadioButton::iconTextControl()
{
    return fIconTextControl;
}
