#ifndef  KxScrollArea_H_
#define  KxScrollArea_H_

#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxScrollArea : public QScrollArea
{
    Q_OBJECT
public:
    KxScrollArea(QWidget *parent=0);

protected:
    virtual void    resizeEvent(QResizeEvent *event);
};

#endif