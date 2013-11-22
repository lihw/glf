#include "KxLabel.h"
#include "../../utils/KxUtils.h"



KxLabel::KxLabel(QWidget *parent)
: QLabel(parent)
{
    // force RMB press event on context menu so that we can show the
    // context menu on press instead of on release
    setContextMenuPolicy(Qt::PreventContextMenu);

    setFont(gKxUtils->schemeQFont(KxUtils::kPlainLabelFont));
}

KxLabel::KxLabel(const QString &text)
: QLabel(text)
{
    // force RMB press event on context menu so that we can show the
    // context menu on press instead of on release
    setContextMenuPolicy(Qt::PreventContextMenu);

    setFont(gKxUtils->schemeQFont(KxUtils::kPlainLabelFont));
}

// Description:  Invoke context menu on RMB press
//
void KxLabel::mousePressEvent(QMouseEvent* event)
{
    if ((event != NULL) && (event->button() == Qt::RightButton))
    {
        emit contextMenuRequested(this, event);
    }

    QLabel::mousePressEvent(event);
}
