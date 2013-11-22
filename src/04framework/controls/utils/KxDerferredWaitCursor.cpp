#include "KxDeferredWaitCursor.h"
                          /*!
	\class KxDeferredWaitCursor
	\brief Helper for TwaitCursor.

	When a wait cursor change is requested, the request is deferred until later. 
	We keep track of the stack of requests and only take action when there is an actual
	change in the override cursor.  This is to avoid the Qt override operations which are quite slow.
*/


KxDeferredWaitCursor* KxDeferredWaitCursor::fsInstance = NULL;
static const int kWaitRequestTimeout = 200; // milliseconds

KxDeferredWaitCursor& KxDeferredWaitCursor::instance()
{
	if (!fsInstance) fsInstance = new KxDeferredWaitCursor(qApp);
	return *fsInstance;
}

void KxDeferredWaitCursor::start()
{
	++fRequestStack;

	if (!fIsOverriden)
	{
		if (fRequestStack == 1) 
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			fIsOverriden = true;
		}
		else if(!fTimer->isActive())
		{
			fTimer->start();
		}
	}
}

void KxDeferredWaitCursor::stop()
{
	--fRequestStack;

	if(fIsOverriden && fRequestStack == 0 && !fTimer->isActive())
	{
		fTimer->start();
	}
}

void KxDeferredWaitCursor::onTimeout()
{
	if (fRequestStack > 0) 
	{
		if (!fIsOverriden)
		{
			QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
			fIsOverriden = true;
		}
	}
	else if (fIsOverriden)
	{
		QApplication::restoreOverrideCursor();
		fIsOverriden = false;
	}
}

KxDeferredWaitCursor::KxDeferredWaitCursor(QObject* parent)
:QObject(parent)
, fRequestStack(0)
, fIsOverriden(false)
{
	fTimer = new QTimer(this);
	fTimer->setInterval(kWaitRequestTimeout);
	fTimer->setSingleShot(true);
	connect(fTimer, SIGNAL(timeout()), this, SLOT(onTimeout()));
}

