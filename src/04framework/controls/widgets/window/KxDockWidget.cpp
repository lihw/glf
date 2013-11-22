// KxDockWidget.cpp
// Dockable widget
//
// Copyright 2012 - 2013 by Bruce Wuu. All rights reserved.
//
// Bruce Wuu brucewu1988@gmail.com

#include "KxDockWidget.h"

KxDockWidget::KxDockWidget(const QString & title, QWidget * parent, Qt::WindowFlags flags)
    : QDockWidget(title, parent, flags)
    , fBeforeUndockSize(QSize())
{
    QObject::connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(handleFloatChange(bool)));
}

KxDockWidget::KxDockWidget(QWidget * parent, Qt::WindowFlags flags)
    : QDockWidget(parent, flags)
      , fBeforeUndockSize(QSize())
{
    QObject::connect(this, SIGNAL(topLevelChanged(bool)), this, SLOT(handleFloatChange(bool)));
}

// Maintain width when being tabbed into another dock widget
// (rather than expanding to that dock widget's size if it is
// wider) 
//
void KxDockWidget::handleFloatChange(bool floating)
{
    if(floating) 
    {
        // Save the dock widget's current size
        fBeforeUndockSize = size();
    } 
    else 
    {
        // Restore saved size when docked
        if(fBeforeUndockSize.isValid()) 
        {
            int savedWidth = fBeforeUndockSize.width();
            //QString widthString;
            //widthString.setUInt32(savedWidth);
            //QString dockWidgetName;
            //QString_fromQString(objectName(), dockWidgetName);
            //QString command;
            //command.set("evalDeferred(\"dockControl -e -width ");
            //command.append(widthString);
            //command.appendSpace();
            //command.append(dockWidgetName);
            //command.append("\")");
            //theCommandEngine->executeCommand(command, false, false);
        }
    }
}
