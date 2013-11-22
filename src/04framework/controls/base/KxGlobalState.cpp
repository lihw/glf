#include "KxGlobalState.h"

//  initialize static class variables
QPointer<QWidget> 		KxGlobalState::currQWindow 				= NULL;
QPointer<QLayout>		KxGlobalState::currQLayout				= NULL;
QPointer<QWidget> 		KxGlobalState::currQControl 				= NULL;
QPointer<QMenuBar> 		KxGlobalState::currQMenuBar 				= NULL;
QPointer<QMenu> 		KxGlobalState::currQMenu					= NULL;
QPointer<QButtonGroup>	KxGlobalState::currQRadioCluster			= NULL;
QPointer<QButtonGroup>	KxGlobalState::currQToolRadioCluster		= NULL;
QPointer<QButtonGroup>	KxGlobalState::currQIconTextRadioCluster	= NULL;
QPointer<QActionGroup>	KxGlobalState::currQRadioMenuItemCluster	= NULL;
//TelfCmdTemplate * 		KxGlobalState::currCmdTemplate 			= NULL;
//Tpanel *				KxGlobalState::currPanel					= NULL;
//Teditor *				KxGlobalState::currEditor					= NULL;

QWidget* KxGlobalState::currentQWindow()
//
//  Description:
//		Returns the default window.  This will implicitly be the
//		last created window or it can be explicity specified to 
//		be any created window.
//
//  Exceptions:
//		None.
//
{
    return currQWindow;
}

void KxGlobalState::setCurrentQWindow(QWidget* newWindow)
//
//  Description:
//		Sets the default window.  NULL is an acceptable value.  Also
//		will set the current menubar, menu etc to NULL.
//
//  Exceptions:
//		None.
//
{
    currQWindow = newWindow;
    currQLayout = NULL;
    currQControl = NULL;
    currQMenuBar = NULL;
    currQMenu = NULL;
    //	currPopupMenu = NULL;
}

QLayout* KxGlobalState::currentQLayout()
//
//  Description:
//		Returns the default controlLayout.  This will implicitly be the
//		last created or it can be explicity specified to 
//		be any created layout.
//
//  Exceptions:
//		None.
//
{
    return currQLayout;
}

void KxGlobalState::setCurrentQLayout(QLayout* newLayout)
//
//  Description:
//		Sets the default controlLayout. NULL is an acceptable value.  Setting
//		the default controlLayout to NULL will make the current window the
//		default parent.
//
//  Exceptions:
//		None.
//
{
    currQLayout = newLayout;
}

QWidget* KxGlobalState::currentQControl()
//
//  Description:
//		Returns the default control.  This will implicitly be the
//		last created window or it can be explicity specified to 
//		be any created control.
//
//  Exceptions:
//		None.
//
{
    return currQControl;
}

void KxGlobalState::setCurrentQControl(QWidget* newControl)
//
//  Description:
//		Sets the default control.  NULL is an acceptable value.
//
//  Exceptions:
//		None.
//
{
    currQControl = newControl;
}

QMenuBar* KxGlobalState::currentQMenuBar()
//
//  Description:
//		Returns the default menubar.  This will implicitly be the
//		last created menubar or it can be explicity specified to 
//		be any created menubar.
//
//  Exceptions:
//		None.
//
{
    return currQMenuBar;
}

void KxGlobalState::setCurrentQMenuBar(QMenuBar* newMenuBar)
//
//  Description:
//		Sets the default menubar. NULL is an acceptable value. Will
//		also set current popup menu and current menu to NULL.
//
//  Exceptions:
//		None.
//
{
    currQMenuBar = newMenuBar;
    currQMenu = NULL;
    //	currPopupMenu = NULL;
}

QMenu* KxGlobalState::currentQMenu()
//
//  Description:
//		Returns the default menu.  This will implicitly be the
//		last created menu or it can be explicity specified to 
//		be any created menu.
//
//  Exceptions:
//		None.
//
{
    return currQMenu;
}

void KxGlobalState::setCurrentQMenu(QMenu* newMenu)
//
//  Description:
//		Sets the default menu. NULL is an acceptable value. Will
//		also set current popup menu to NULL.
//
//  Exceptions:
//		None.
//
{
    currQMenu = newMenu;
    //	currPopupMenu = NULL;
}

QButtonGroup * KxGlobalState::currentQRadioCluster()
//
//  Description:
//		Returns the current cluster for radio buttons.
//
{
    return currQRadioCluster;
}

void KxGlobalState::setCurrentQRadioCluster(
    QButtonGroup * newCluster)
    //
    //  Description:
    //		Sets the current radio buttons cluster. NULL is an acceptable
    //		value.  
    //
{
    currQRadioCluster = newCluster;
}

QButtonGroup * KxGlobalState::currentQToolRadioCluster()
//
//  Description:
//		Returns the current cluster for tool radio buttons.
//
{
    return currQToolRadioCluster;
}

void KxGlobalState::setCurrentQToolRadioCluster(
    QButtonGroup * newCluster)
    //
    //  Description:
    //		Sets the current tool radio buttons cluster. NULL is an acceptable
    //		value.  
    //
{
    currQToolRadioCluster = newCluster;
}

QButtonGroup * KxGlobalState::currentQIconTextRadioCluster()
//
//  Description:
//		Returns the current cluster for icon text radio buttons.
//
{
    return currQIconTextRadioCluster;
}

void KxGlobalState::setCurrentQIconTextRadioCluster(
    QButtonGroup * newCluster)
    //
    //  Description:
    //		Sets the current icon text radio buttons cluster. NULL is an acceptable
    //		value.  
    //
{
    currQIconTextRadioCluster = newCluster;
}

QActionGroup * KxGlobalState::currentQRadioMenuItemCluster()
//
//  Description:
//		Returns the current cluster for radio menu items.
//
{
    return currQRadioMenuItemCluster;
}

void KxGlobalState::setCurrentQRadioMenuItemCluster(
    QActionGroup * newCluster)
    //
    //  Description:
    //		Sets the current radio menu item cluster. NULL is an acceptable
    //		value.  
    //
{
    currQRadioMenuItemCluster = newCluster;
}

//TelfCmdTemplate* KxGlobalState::currentCmdTemplate()
////
////  Description:
////		Returns the default command template.  This will implicitly be the
////		last created command template or it can be explicity specified to 
////		be any created template or NULL.
////
////  Exceptions:
////		None.
////
//{
//    return currCmdTemplate;
//}
//
//Tpanel* KxGlobalState::currentPanel()
////
////  Description:
////		Returns the default panel.  This will implicitly be the
////		last created panel or it can be explicity specified to 
////		be any created panel or NULL.
////
////  Exceptions:
////		None.
////
//{
//    return currPanel;
//}
//
//Teditor* KxGlobalState::currentEditor()
////
////  Description:
////		Returns the default editor.  This will implicitly be the
////		last created editor or it can be explicity specified to 
////		be any created editor or NULL.
////
////  Exceptions:
////		None.
////
//{
//    return currEditor;
//}

//void KxGlobalState::setCurrentCmdTemplate(TelfCmdTemplate* newCmdTemplate)
////
////  Description:
////		Sets the default command template. NULL is an acceptable value.  
////
////  Exceptions:
////		None.
////
//{
//    currCmdTemplate = newCmdTemplate;
//}
//
//void KxGlobalState::setCurrentPanel(Tpanel* newPanel)
////
////  Description:
////		Sets the default panel. NULL is an acceptable value.  
////
////  Exceptions:
////		None.
////
//{
//    currPanel = newPanel;
//}
//
//void KxGlobalState::setCurrentEditor(Teditor* newEditor)
////
////  Description:
////		Sets the default editor. NULL is an acceptable value.  
////
////  Exceptions:
////		None.
////
//{
//    currEditor = newEditor;
//}
//