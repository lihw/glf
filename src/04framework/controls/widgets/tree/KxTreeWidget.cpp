#include "KxTreeWidget.h"
#include "../../base/KxQtHelper.h"

KxTreeWidget::KxTreeWidget(QWidget *parent)
: QTreeWidget(parent) 
, fDragFeedbackVisible(false)
{
}

KxTreeWidget::~KxTreeWidget()
{
}

void KxTreeWidget::mousePressEvent(QMouseEvent* event)
{
	if( event->button() == Qt::MidButton ){
		// Let KxDragManager handle drag 'n drop.
		//
		event->ignore();
	} else {
		/*
		Although the eventFilter in KxQtHelper does get installed on
		this widget, QTreeWidget eats the mouse click events and only a
		QContextMenuEvent is sent to the filter. So I'm overriding the
		press event here and calling the handlePopupMenu function directly.
		*/
		if (!KxQtHelper::handlePopupMenu(this, event)) {
			QTreeWidget::mousePressEvent(event);
		}
	}
}

