#ifndef KxLabel_H_
#define KxLabel_H_
#include "../../base/HQtGui.h"
#include "../../base/KxIcontextMenuRequestor.h"
class KX_DLLEXPORT KxLabel : public QLabel, public KxIcontextMenuRequestor
{
    Q_OBJECT

public:
    KxLabel(QWidget *parent = 0);
    KxLabel(const QString &text);


    //----------------------------------------------
    //    KxIcontextMenuRequestor Interface
    //
signals:
    //	Emitted by the control whenever the user requests a context menu
    //	while over the control.
    void	contextMenuRequested(QWidget* control, QMouseEvent* event);

    //    End of KxIcontextMenuRequestor Interface
    //----------------------------------------------

protected:
    virtual void mousePressEvent(QMouseEvent* event);
};
#endif