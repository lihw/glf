#include "KxLayoutWidget.h"

void KxLayoutWidget::setObjectName(const QString &name)
{
	QWidget::setObjectName(name);
	if(layout()) {
		layout()->setObjectName(name);
	}
}

void KxLayoutWidget::childEvent(QChildEvent * event)
{
	QLayout *l = layout();
	// If there is no layout attached, there is nothing to do here
	if(l == NULL) return;

	if(event->added()) {
		QObject *child = event->child();
		// If object is widget...
		if(child && child->isWidgetType()) {
			// ... add it to layout
			l->addWidget((QWidget*)child);
		}
	}
}

KxLayoutWidget::~KxLayoutWidget()
{
}

KxLayoutWidget::KxLayoutWidget(QWidget *parent)
: QWidget(parent)
{
}
