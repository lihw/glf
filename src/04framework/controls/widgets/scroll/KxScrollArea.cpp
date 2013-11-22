#include "KxScrollArea.h"
#include "../../base/KxQtHelper.h"

// Description:  Constructor
//
KxScrollArea::KxScrollArea(QWidget *parent)
: QScrollArea(parent)
{
    setFocusPolicy(Qt::NoFocus);
}

// Description:  This procedure handles the resize event.
//
void KxScrollArea::resizeEvent(QResizeEvent *event)
{
    //KxQtHelper::executeCommandScript("resizeCommand", this);
    QScrollArea::resizeEvent(event);
}
