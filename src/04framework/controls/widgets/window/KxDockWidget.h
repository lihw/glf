// KxDockWidget.h
// Dockable widget
//
// Copyright 2012 - 2013 by Bruce Wuu. All rights reserved.
//
// Bruce Wuu brucewu1988@gmail.com



#ifndef KxDockWidget_H_
#define KxDockWidget_H_

#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxDockWidget : public QDockWidget
{
    Q_OBJECT

public:
    KxDockWidget(const QString& title, QWidget* parent = 0, Qt::WindowFlags flags = 0);

    KxDockWidget(QWidget* parent = 0, Qt::WindowFlags flags = 0);

public slots:
    void handleFloatChange(bool floating);

private:
    QSize fBeforeUndockSize;
};

#endif // !KxDockWidget_H_
