#include "KxWaitCursor.h"
#include "KxDeferredWaitCursor.h"


bool		KxWaitCursor::fBatchMode = false;
int			KxWaitCursor::stackCount = 0;

KxWaitCursor::KxWaitCursor()
//
// Description: 
//		Constructor.
//
:	fIsStarted(false)
{
}

KxWaitCursor::~KxWaitCursor() 
//
// Description: 
//		Destructor.  Stops the cursor if it is still spinning.
//
{
    // If the cursor is spinning, stop it.
    if (fIsStarted) stop();
}

void KxWaitCursor::start()
//
// Description: 
//		Called before a time consuming task is started to initialize
//		the wait cursor.  Changes the cursor shape and prevents the
//		cursor from changing when passing over different cursor
//		regions.
//
{
    //	Do not call start() twice on the same KxWaitCursor.
    //
    Q_ASSERT(!fIsStarted);
    if (fIsStarted) {
        return;
    }

    //	Only change the cursor if the application is run in GUI mode. 
    if (!fBatchMode) {
        KxDeferredWaitCursor::instance().start();
    }

    fIsStarted = true;
}


void KxWaitCursor::pause()
//
// Description: 
//		Empties the stack. Note that this function
//		must have a corresponding resume function to
//		restore the cursor stack.
//
{
    if(!fBatchMode)
    {
        QCursor* activeCursor = QApplication::overrideCursor(); 
        while(activeCursor != NULL)
        {
            if(activeCursor->shape() == Qt::WaitCursor)
            {
                stackCount++;
                QApplication::restoreOverrideCursor();
                activeCursor = QApplication::overrideCursor(); 
            }
            else
            {
                break;
            }
        }
    }
}

void KxWaitCursor::resume()
{
    if (!fBatchMode){
        int i;
        for(i=0; i < stackCount; i++)
        {
            QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        }

        stackCount = 0;
    }
}

void KxWaitCursor::stop()
//
// Description: 
//		Called after a time consuming task is started.  Returns
//		the cursor shape to its original shape.  
//
{
    if (!fIsStarted) {
        return;
    }
    fIsStarted = false;

    if (!fBatchMode) {
        KxDeferredWaitCursor::instance().stop();
    }
}

void KxWaitCursor::setInBatchMode()
{
    fBatchMode = true;
}