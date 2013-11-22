#ifndef KxToolRadioButton_H_
#define KxToolRadioButton_H_

#include "../../base/HQtGui.h"
#include "../../drag/KxDragHandler.h"

class KX_DLLEXPORT KxToolRadioButton : public QToolButton , public KxDragHandler
{
    Q_OBJECT
public:
    KxToolRadioButton(QWidget *parent = 0);

    QSize sizeHint() const;
    QSize minimumSize() const;

public:
    virtual void getDragData(QWidget *source, QMouseEvent *, KxDragData *dragData);
};

#endif