#ifndef KxIcontextMenuRequestor_H_
#define KxIcontextMenuRequestor_H_
#include "../base/HQtGui.h"

class KX_DLLEXPORT KxIcontextMenuRequestor 
{
public:
    virtual ~KxIcontextMenuRequestor(){}

signals:
    // Emitted by the control whenever the user requests a context menu 
    // while over the control 
    virtual void contextMenuRequested(QWidget *control, QMouseEvent *event ) = 0;
};

#endif