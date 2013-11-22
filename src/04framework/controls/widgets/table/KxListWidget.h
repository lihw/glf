#ifndef KxListWidget_H_
#define KxListWidget_H_

#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxListWidget : public QListWidget
{
    Q_OBJECT

public:
    KxListWidget(QWidget *parent = 0);
    virtual ~KxListWidget();

    public slots:
        void executeSelectCommand();

protected:
    virtual void keyPressEvent ( QKeyEvent * event );
    virtual void mousePressEvent ( QMouseEvent * event );
    virtual void mouseMoveEvent ( QMouseEvent * event );
    virtual void mouseReleaseEvent ( QMouseEvent * event );

private:
    bool fMousePressed;
};
#endif