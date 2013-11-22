#include "KxIconTextButton.h"
// Description:  Constructor
//
KxIconTextButton::KxIconTextButton(QWidget *parent)
: QPushButton(parent)
, fDoubleClickTimer(this)
, fGotDoubleClickEvent(false)
, fIsRepeatable(false)
{
    setFlat(true);
    setSizePolicy(QSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum, sizePolicy().controlType()));
    fIconTextControl = new KxIconTextControl(this);
    // initialize double-click timer
    fDoubleClickTimer.setInterval(qApp->doubleClickInterval() / 2);
    fDoubleClickTimer.setSingleShot(true);
    QObject::connect(&fDoubleClickTimer, SIGNAL(timeout()), this, SLOT(executeSingleClickCommand()));
}

QSize KxIconTextButton::sizeHint() const
{
    if(fIconTextControl->style() == KxIconTextControl::textOnly) {
        return QSize(fIconTextControl->sizeHint().width(), QPushButton::sizeHint().height());
    } else {
        return fIconTextControl->sizeHint();
    }
}

void KxIconTextButton::resizeEvent(QResizeEvent * event)
{
    fIconTextControl->resize(size());
}

//void KxIconTextButton::setCommand(Taction * action)
//{
//    KxQtHelper::setCommandScript(action, "command", const_cast<KxIconTextButton*>(this));
//}
//
//Taction *KxIconTextButton::command() const
//{
//    return (Taction *)KxQtHelper::commandScript("command", const_cast<KxIconTextButton*>(this));
//}
//
//void KxIconTextButton::setDoubleClickCmd(Taction * action)
//{
//    KxQtHelper::setCommandScript(action, "doubleClickCommand", const_cast<KxIconTextButton*>(this));
//}
//
//Taction *KxIconTextButton::doubleClickCmd() const
//{
//    return (Taction *)KxQtHelper::commandScript("doubleClickCommand", const_cast<KxIconTextButton*>(this));
//}
//
//void KxIconTextButton::setLabelEditingCallback(TscriptAction *callback)
//{
//    if (NULL != fLabelEditingCallback) {
//        if (fLabelEditingCallback->requiresDeletion()) {
//            delete fLabelEditingCallback;
//        }
//    }
//    fLabelEditingCallback = callback;
//}
//
// ------ Event processing : single vs double click ----------------
bool KxIconTextButton::executeSingleClickCommand()
{
   /* if (isRepeatable())
    {
        if (KxQtHelper::executeCommandScript("command", const_cast<KxIconTextButton*>(this))) 
        {
            gTheRepeatCommandManager->addCommand(this);
            return true;
        }
    }
    else
    {
        return KxQtHelper::executeCommandScript("command", const_cast<KxIconTextButton*>(this));
    }*/

    return false;
}

void KxIconTextButton::executeLabelEditingCallback(QString newLabel)
{
    QObject::disconnect(iconTextControl(), SIGNAL(labelChanged(QString)), this, SLOT(executeLabelEditingCallback(QString)));
    //if (fLabelEditingCallback) {
    //    Tstring label;
    //    Tstring_fromQString(newLabel, label);
    //    fLabelEditingCallback->addArgument(label);
    //    fLabelEditingCallback->doIt();
    //    fLabelEditingCallback->clearArguments();
    //}
}

void KxIconTextButton::mouseReleaseEvent(QMouseEvent * event)
{
    QPushButton::mouseReleaseEvent(event);
    if(event->button() == Qt::LeftButton) {
//#if defined(WANT_PYTHON)
//        // For a shelf button, <SHIFT> + double click brings up the shelf
//        // editor at the command editing window for this button.
//        //
//        bool doEditButton = (qobject_cast<KxShelfButton*>(this) != NULL) &&
//            (event->modifiers() == Qt::ShiftModifier);
//#endif
//        if(fGotDoubleClickEvent) {
//            fGotDoubleClickEvent = false; // reset double-click flag
//            // this is a mouse release after double-click - execute double-click command
//#if defined(WANT_PYTHON)
//            if( doEditButton ){
//                // Shift+double click: open shelfEditor for this button
//                //
//                Tstring action = "import maya.app.general.shelfEditorWindow as myTempSEW\nmyTempSEW.doIt(selectedShelfButton='";
//                QString qName = objectName();
//                Tstring tName;
//                Tstring_fromQString( qName, tName );
//                action += tName;
//                action += "')\ndel myTempSEW";
//                //action += "')";
//                TscriptAction script(action);
//                script.setLanguageContext( kPython_LanguageContext );
//                script.doIt(Tevent());
//            } else
//#endif
//                if(doubleClickCmd() != NULL) {
//                    KxQtHelper::executeCommandScript("doubleClickCommand", const_cast<KxIconTextButton*>(this));
//                } else if(fLabelEditingCallback) {
//                    QObject::connect(iconTextControl(), SIGNAL(labelChanged(QString)), this, SLOT(executeLabelEditingCallback(QString)));
//                    iconTextControl()->editLabel();
//                }
//        } else {
//            if(doubleClickCmd() != NULL
//#if defined(WANT_PYTHON)
//                || doEditButton
//#endif
//                ) {
//                    // this is mouse release after mouse press - start double-click timer
//                    fDoubleClickTimer.start();
//            } else {
                if (rect().contains(event->pos())) {
                    executeSingleClickCommand();
                }
//            }
//        }
   }
}

void KxIconTextButton::mouseDoubleClickEvent(QMouseEvent * event)
{
    if( event->button() == Qt::LeftButton){/*&& ( */
       /* doubleClickCmd()*/
//#if defined(WANT_PYTHON)
//        ||
//        ((qobject_cast<KxShelfButton*>(this) != NULL) &&
//        (event->modifiers() == Qt::ShiftModifier))
//#endif
        /*)*/ 
            // Bug 357300: we only care about the double click timer if the button
            // has a double click command. In particular, KxSymbolButton, which
            // derives from KxIconTextButton, has no double click command, and
            // no way to set one. This will avoid the problem of converting two
            // quick single click commands to a non-existant double click command
            // (e.g., quickly clicking on the forward frame button resulted in some
            // events being treated as double click events, and thus users had
            // to click twice to advance one frame).
            //
            // For a shelf button, <SHIFT> + double click brings up the shelf
            // editor at the command editing window for this button, so we also
            // need the double click timer in such a case.
            //
            fDoubleClickTimer.stop();
            fGotDoubleClickEvent = true; // set double-click flag
    }

    QPushButton::mouseDoubleClickEvent(event);
}
// -----------------------------------------------------------------

KxIconTextControl *KxIconTextButton::iconTextControl() const
{
    return fIconTextControl;
}
