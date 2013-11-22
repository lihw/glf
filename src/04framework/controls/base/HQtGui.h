
#ifndef HQtGui_H_
#define HQtGui_H_

#include <QtGui/QtGui>
#include <QtCore/QtCore>
#include "KxExport.h"
#include "KxFoundation.h"


enum KxAttachmentType 
{ 
    kAttachNone, 
    kAttachForm, 
    kAttachOppositeForm, 
    kAttachPosition, 
    kAttachControl, 
    kAttachOppositeControl 
};

enum KxAttachmentEdge 
{ 
    kAttachTop, 
    kAttachLeft, 
    kAttachBottom, 
    kAttachRight 
};

#define kKxLayoutInternalWidgetName "kxLayoutInternalWidgetName"

#endif // !HQtGui_H
