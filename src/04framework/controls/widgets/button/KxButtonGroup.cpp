#include "KxButtonGroup.h"


KxButtonGroup::KxButtonGroup(QObject* parent)
:	QButtonGroup(parent)
{
    connect(this, SIGNAL(buttonClicked(int)), this, SLOT(valueChanged(int)));
}


void KxButtonGroup::setEnabled(bool enable)
{
    QList<QAbstractButton*> buttonList = buttons();
    QList<QAbstractButton*>::iterator iter;

    for (iter = buttonList.begin(); iter != buttonList.end(); ++iter)
        (*iter)->setEnabled(enable);
}


void KxButtonGroup::setReadOnly(bool readOnly)
{
    //	At the moment buttons don't have a readOnly state, so we treat it
    //	the same as disabled.
    setEnabled(!readOnly);
}


void KxButtonGroup::valueChanged(int newValue)
{
    QVariant	value(newValue);
    emit newValueForConnections(value, false);
}


void KxButtonGroup::newValueFromConnection(const QVariant& newValue)
{
    if (newValue.canConvert<int>())
    {
        //	Find the button with the specified value and set it to be
        //	the active (checked) one.
        QAbstractButton* button = QButtonGroup::button(newValue.value<int>());
        if (button) {
            bool blocked = button->blockSignals(true);
            button->setChecked(true);
            button->blockSignals(blocked);
        }
    }
}
