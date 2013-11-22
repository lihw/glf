#include "KxProgressBar.h"
#include "../../utils/KxUtils.h"
//
//
//  Class:		KxProgressBar
//	
//	Description: 
//		The KxProgressBar control allows processes that take a long
//		time to give visual progress feedback to the user.
//
//		(Oct 10, 2000) The main Maya window has a progressBar control
//		that can be made visible through Tcomputation (see Tcomputation.h).
//		
//
//+

#define kELFprogressBarMainWidth 130


// Moved this variable from Tcontrol.cpp (temporarily?)
//Treporter * KxProgressBar::fsHelpReporter = NULL; 


QString KxProgressBar::fsCancelMessage("progress cancelled");

//////////////////////////////////////////////////////////////////////
//	Public methods.
//////////////////////////////////////////////////////////////////////

KxProgressBar::KxProgressBar(QWidget *parent)
: QProgressBar(parent)
{
    this->setFont(gKxUtils->schemeQFont (KxUtils::kSmallPlainLabelFont));
    connect(this, SIGNAL(valueChanged(int)), this, SLOT(onValueChanged(int)));
}


KxProgressBar::~KxProgressBar()
//
//	Description:
//		Destructor.
//
{
}

void KxProgressBar::setRange( int min,  int max)
//
//	Description:
//		Set the range used to calculate the 
//		percentage of progress between start and end.
//
{
    Q_ASSERT(max > min);
    QProgressBar::setRange(min, max);
}

int KxProgressBar::minValue() const
//
//	Description:
//		Return the current minimum of the progress range
//
{
    return minimum();
}



int KxProgressBar::maxValue() const
//
//	Description:
//		Return the current maximum of the progress range
//
{
    return maximum();
}


void KxProgressBar::setProgress( int progress)
//
//	Description:
//		Set the absolute progress value. Note that this is NOT the
//		percentage of progress between start and end, which is what
//		the control is drawing.
//
{
    setValue(progress);

    //	Update the control immediately
    //
    if(showPercentText()) {
        //	If we're showing % in status, then we need to update
        //	our status label too.
        //
        setStatus(status());
    }
}


int KxProgressBar::progress() const
//
//	Description:
//		Return the absolute progress value (as opposed to a percentage 
//		progress value)
//
{
    return value();
}

void KxProgressBar::onValueChanged(int v)
{
    // Make sure we get a repaint
#if defined(OSMac_)
    // Mac repaint events are not being flushed while a script is running
    // (which is the expected case) so we make sure it gets repainted anytime 
    // it might have changed
    KxQtHelper::forceImmediatePaint(this);
#else
    update();
#endif
}

void KxProgressBar::setIsInterruptable( bool value) 
//
//	Description:
//		Sets internal flag indicating if the progressBar should actually
//		check for the ESC key being pressed.
//
{
    if(isMainProgressBar() && isInterruptable() != value) {
        KxProgressIndicator::setIsInterruptable(value);
        setStatus(status());
    }
}


void KxProgressBar::setStatus(const QString & status) 
//
//	Description:
//		If this progressBar is not in the main window it sets the
//		text in the help-line to indicate status.
//		For normal progress bars it does nothing.
//
//		NOTE: The status-line could easily be updated direcly
//		      from Tcomputation, but the FORCEUPDATE on Win32
//		      will need to come from somewhere.
//
{
    //	Only show a status if this is the progressBar next to the helpLine
    //	in the main Maya window.
    //
    if(isMainProgressBar()) {
        KxProgressIndicator::setStatus(status);
        QString message;

        if(showPercentText()) {
            // Append a string of the form "(20%) " in front.
            message = QString("% 1").arg(percent());
        }

       message += status;

        //	Indicate to the user that the ongoing computation is interruptable
        //
        if(isInterruptable())
            message += fsCancelMessage;

        //	Update the helpLine control with the current status string.
        //

        //if(fsHelpReporter != NULL) {
        //    //fsHelpReporter->reportMsg(&message);
        //    //	see if the text has changed and only 
        //    //	redraw if it has, rather than on every call 
        //    if ( fProgressBarString.compare(message) != 0 ) {
        //        fProgressBarString.set(message);
        //    }
        //}
    }
}


void KxProgressBar::setShowPercentText(bool show)
//
//	Description:
//		Sets whether the "(XX%) " text is inserted in front of
//		the status text when the status updates.
//
{
    if(showPercentText() != show) {
        KxProgressIndicator::setShowPercentText(show);
        setStatus(status());
    }
}


void KxProgressBar::setIsMainProgressBar( bool value) 
//
//	Description:
//		Sets whether Tcomputation will use this progressBar as the one used
//		to show progress of basic operations.
//
{
 /*   if(!gBatchMode) {
        if(value) {
            Tcomputation::registerProgressIndicator(Tcomputation::kProgressGauge,this);
        } else if(isMainProgressBar()) {
            Tcomputation::registerProgressIndicator(Tcomputation::kProgressGauge,NULL);
        }
    }*/
}


bool KxProgressBar::isMainProgressBar() const 
//
//	Description:
//		Returns whether or not this progressBar is the one used by
//		Tcomputation to indicate ongoing progress in basic operations.
//
{
   // KxProgressIndicator* mainBar = Tcomputation::progressIndicator(Tcomputation::kProgressGauge);
    //return (mainBar == (KxProgressIndicator*)this);
    return false;
}


bool KxProgressBar::isCancelled() const
//
//	Description:
//		Returns true if the user has tried to cancel the operation
//
{
    return isInterruptable() ;//&& TeventHandler::interruptRequested();
}

void KxProgressBar::startWithCursor()
//
//	Description:
//		Starts a new computation (like start()) but also starts the
//		wait cursor. Intended to be used when a MEL script is starting
//		a large computation.
//
{
    if(!isInUse())
        fWaitCursor.start();

    start();
}


void KxProgressBar::start()
//
//	Description:
//		Resets the progress bar to empty (0% progress)
//		If this is the progressBar next to the helpLine
//		in the main window, then it will make itself visible.
//
{
    if(!isInUse()) {

        //if(isInterruptable())
        //    TeventHandler::clearInterrupt();

        if(isMainProgressBar()) {
            KxProgressIndicator::setShowPercentText(false);
            setStatus("");
            setMinimumWidth(kELFprogressBarMainWidth);
            setMaximumWidth(kELFprogressBarMainWidth);
            show();
        }
    }

    KxProgressIndicator::start();
}

void KxProgressBar::finishWithCursor()
//
//	Description:
//		Makes the progressBar stop a computation that used the wait cursor
//		(same as finish() only it stops the cursor)
//
{
    if(isInUse())
        fWaitCursor.stop();

    finish();
}


void KxProgressBar::finish()
//
//	Description:
//		Resets the progress bar to empty (0% progress)
//		If this is the progressBar next to the helpLine
//		in the main window, then it will hide itself.
//
{

    if(isInUse()) {

        //if(isInterruptable())
        //    TeventHandler::clearInterrupt();

        KxProgressIndicator::setIsInterruptable(false);

        //	If this is the main one, hide ourselves
        //	
        if(isMainProgressBar()) {
            KxProgressIndicator::setShowPercentText(false);
            setStatus("");
            setMinimumWidth(1);
            setMaximumWidth(1);
            hide();
        }
    }

    KxProgressIndicator::finish();
}

/*static*/ void KxProgressBar::setCancelMessage(const QString & message)
//
//	Description:
//		Set the cancel message string. A default message is used if this
//		method is not called. Calling this method simply allows you to
//		modify or replace the default message.
//
{
    fsCancelMessage = message;
}

//// Moved these 2 functions from Tcontrol.cpp (temporarily?)
///*static*/ void KxProgressBar::setHelpReporter(Treporter* reporter)
////
////	Description:
////		Set the reporter that will display the help string.
////
//{
//    fsHelpReporter = reporter;
//}
//
//