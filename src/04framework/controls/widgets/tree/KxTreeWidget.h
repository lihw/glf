#ifndef KxTreeWidget_H_
#define KxTreeWidget_H_
#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxTreeWidget : public QTreeWidget
{
    Q_OBJECT

public:
    KxTreeWidget(QWidget *parent = 0);
    virtual ~KxTreeWidget();
    void			setDragFeedbackVisible (bool visible) {fDragFeedbackVisible = visible;}
    bool		dragFeedbackVisible() const {return (fDragFeedbackVisible);}

protected:
    virtual void mousePressEvent ( QMouseEvent * event );
    bool				fDragFeedbackVisible;
};
#endif