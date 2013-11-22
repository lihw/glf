#include "KxToolRadioButton.h"

#include "../../drag/KxDragManager.h"

KxToolRadioButton::KxToolRadioButton(QWidget *parent /* = 0 */)
:QToolButton(parent)
{
    setIconSize(QSize(34,34));
    setCheckable(true);
    setPopupMode(QToolButton::DelayedPopup);
    KxDragManager::addDragHandler(this,this);
}

QSize KxToolRadioButton::sizeHint() const
{
    return QSize(36,36);
}

QSize KxToolRadioButton::minimumSize() const
{
    return QSize(36,36);
}


void KxToolRadioButton::getDragData(QWidget *source, QMouseEvent *, KxDragData *dragData)
{
    //dragData->addDragItem(fDragItem);
    //
}