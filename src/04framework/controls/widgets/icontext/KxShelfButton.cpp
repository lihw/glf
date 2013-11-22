#include "KxShelfButton.h"

#include "../../drag/KxDragManager.h"

KxShelfButton::KxShelfButton(QWidget *parent)
: KxIconTextButton(parent)
//,	fDragItem() 
{
    KxDragManager::addDragHandler( this, this );
}

//void KxShelfButton::setCommand(Taction *action)
////
////  Description:
////		Sets the command script for the button, but does not change
////		its Taction.
////
//{
//    /*TscriptAction * scriptAction = NULL;
//
//    if (NULL != (scriptAction = action->asScriptAction())) {
//        scriptAction->setEcho(true);
//        fDragItem.setScriptAction(*scriptAction);
//    }*/
//
//    KxIconTextButton::setCommand(action);
//}

void KxShelfButton::getDragData(
                                   QWidget*, QMouseEvent*, KxDragData* dragData
                                   )
{
   // dragData->addDragItem((KxDragItem*)&fDragItem);
}
