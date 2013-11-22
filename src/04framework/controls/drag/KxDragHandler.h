#ifndef KxDragHandler_H_
#define KxDragHandler_H_

#include "../base/HQtGui.h"
#include "KxDragData.h"

class KX_DLLEXPORT KxDragHandler 
{
public:
    KxDragHandler() {}
    virtual ~KxDragHandler(){}
    
    // Returns q mask containing the mouse buttons which this handler 
    // considers valid for dragging 
    virtual Qt::MouseButtons dragButtons() const;

    // Called when the drag has completed. This can be used, for example 
    // to delete the dragged data from the source in the case of a Qt::moveAction action 
    virtual void dragDone(QWidget *source, QMouseEvent *, Qt::DropAction action){}
    // Pure virtual method which must be overridden by derived calsses to add 
    // to 'dragData' the 'KxDragItem' -derived data avaiable for dragging from the source 
    virtual void getDragData(QWidget *source, QMouseEvent *, KxDragData *dragData) = 0;
};

inline Qt::MouseButtons KxDragHandler::dragButtons()const
{
    return Qt::MidButton;
}

#endif