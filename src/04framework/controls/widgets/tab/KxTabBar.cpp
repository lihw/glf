#include "KxTabBar.h"

// Description:	 Constructor
//
KxTabBar::KxTabBar(QWidget *parent)
: QTabBar(parent)
{
    setAttribute(Qt::WA_MacSmallSize);
    setElideMode(Qt::ElideNone);
    setUsesScrollButtons(true);
}

// Description:	 This procedure handles the mouse press event.
//
void KxTabBar::mousePressEvent(QMouseEvent *e)
{
    // If tabs are selected by mouse press in current style, deal with preselectCmd and selectCmd
    if(e->type() == style()->styleHint(QStyle::SH_TabBar_SelectMouseType, 0, this)) {
        handleSelectCommands(e);
    } else {
        // Otherwise just proceed with parent class implementation
        QTabBar::mousePressEvent(e);
    }	
}
// Description:	 This procedure handles the mouse release event.
//
void KxTabBar::mouseReleaseEvent(QMouseEvent *e)
{
    // If tabs are selected by mouse release in current style, deal with preselectCmd and selectCmd
    if(e->type() == style()->styleHint(QStyle::SH_TabBar_SelectMouseType, 0, this)) {
        handleSelectCommands(e);
    } else {
        // Otherwise just proceed with parent class implementation
        QTabBar::mouseReleaseEvent(e);
    }	
}

// Description:	 Executes the tab layout's pre-select and select commands.
//
//               This function is called from two places: from KxTabBar
//               during mouse events, and from KxTabWidget when a tab is
//               selected from the popup menu. In the first case, we handle
//               the given QMouseEvent in order to change tabs (the tabIndex 
//               parameter will be the default invalid value of -1). In the 
//               latter case, we pass in the index of the selected popup menu
//               item and use it to manually change tabs (here, the QMouseEvent
//               pointer passed in is NULL).
//
void KxTabBar::handleSelectCommands(QMouseEvent *e, int tabIndex)
{
    // Disable updates on parent widget, so preSelectCommand is executed before new widget is drawn on screen
    QWidget *parent = parentWidget();
    bool currentState = true;
    if(parent) {
        // save current state and disable updates
        currentState = parent->updatesEnabled();
        parent->setUpdatesEnabled(false);
    }

    // save current index
    int index = currentIndex();

    if(e != NULL) {
        // Tab selection from mouse click - proceed with parent class event handling
        if(e->type() == QEvent::MouseButtonPress) {
            QTabBar::mousePressEvent(e);

        } else {
            QTabBar::mouseReleaseEvent(e);
        }
    } else {
        // Tab selection from the popup menu - manually change tabs
        setCurrentIndex(tabIndex);
    }

    // get new index
    int newIndex = currentIndex();

    // if index changed, execute preselect command
    /*if(newIndex != index) {
        KxQtHelper::executeCommandScript("preSelectCommand", this);
    }*/

    // restore updates state on parent widget
    if(parent) {
        // enabling updates will call update() for us
        parent->setUpdatesEnabled(currentState);
    }

    //// if index changed, execute select command
    //if (newIndex != index) {
    //    KxQtHelper::executeCommandScript("selectCommand", this);
    //}
}

// Description:	 This procedure handles the mouse double click event.
//
void KxTabBar::mouseDoubleClickEvent(QMouseEvent *event)
{
   // KxQtHelper::executeCommandScript("doubleClickCommand", this);
    QTabBar::mouseDoubleClickEvent(event);
}

QSize KxTabBar::sizeHint() const
{
    if(isVisible()) {
        return QTabBar::sizeHint();
    } else {
        return QSize(0, 0);
    }
}
