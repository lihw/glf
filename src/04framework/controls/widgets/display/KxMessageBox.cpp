#include "KxMessageBox.h"

//  Global flag to ignore posting of message boxes.
bool gIgnoreMessageBoxes = false;

// Constructor
//
KxMessageBox::KxMessageBox(Icon icon, const QString & title,
                                 const QString & text,
                                 StandardButtons buttons ,
                                 QWidget * parent ,
                                 Qt::WindowFlags f) 
                                 : QMessageBox(icon, title, text, buttons, parent, f)
{
}

QPushButton* KxMessageBox::addButton(const QString & text, ButtonRole role)
{
    // create button
    QPushButton* theButton = QMessageBox::addButton(text, role);
    theButton->setShortcut( QKeySequence::mnemonic(text) );
    // we want buttons in message boxes to always allow tab to focus
    theButton->setFocusPolicy(Qt::StrongFocus);

    return theButton;
}

QPushButton* KxMessageBox::addButton(StandardButton button)
{
    QPushButton* theButton = QMessageBox::addButton(button);
    // we want buttons in message boxes to always allow tab to focus
    theButton->setFocusPolicy(Qt::StrongFocus);
    return theButton;
}

// Description:
//	This function overrides the closeEvent function in the parent 
//  class.  When the user closes the message box window using the
//  "Close" item of the window's manager menu.  We simply close
//  the window.
//
void KxMessageBox::closeEvent(QCloseEvent *e)
{
    QDialog::closeEvent(e);
}

// Description:
//  This function overrides the exec function in the parent class.
//  It checks for the variable 'gIgnoreDialogs' before creating 
//  the message box.  This is similar to what is done in the Tdialog
//  class.
//
int KxMessageBox::exec()
{
    if( gIgnoreMessageBoxes ) {
        // do not create message box
        return 0;
    }
    else {
        if (defaultButton()) {
            defaultButton()->setFocus();
        }
        return QMessageBox::exec();
    }
}
