#ifndef  KxTabBar_H_
#define KxTabBar_H_

#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxTabBar : public QTabBar
{
    Q_OBJECT
public:
    KxTabBar(QWidget *parent=0);
    virtual void    mousePressEvent(QMouseEvent *e);
    virtual void    mouseReleaseEvent(QMouseEvent *e);
    virtual void    mouseDoubleClickEvent(QMouseEvent *e);
    QSize			sizeHint() const;
    void			handleSelectCommands(QMouseEvent *e = NULL, int tabIndex = -1);
};
#endif