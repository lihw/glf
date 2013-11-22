#include "KxScrollLayout.h"

#include "../widgets/scroll/KxScrollArea.h"
#include "../base/KxUIInventory.h"
#include "../layouts/KxColumnLayout.h"


// Description:  Constructor.
//
KxScrollLayout::KxScrollLayout(QWidget *parent, bool isChildResizable)
: QVBoxLayout(parent)
, fUnderConstruction(true)
{
    // This KxScrollLayout is associated to parent widget.
    // The QScrollArea is an internal structure which helps
    // scroll layout to provide the scrolling feature. 
    //
    fScrollArea = new KxScrollArea(parent);
    if (fScrollArea != NULL)
    {
        fScrollArea->setObjectName(kKxLayoutInternalWidgetName);
		fScrollArea->setWidgetResizable(true);
        QVBoxLayout::addWidget(fScrollArea);

        // QScrollArea can take only one widget.  This widget and
        // its layout will help us manage what is in the scroll layout. 
        // 
        fInternalWidget = new QWidget(fScrollArea);
        if (fInternalWidget)
        {
            KxColumnLayout * internalWidgetLayout = new KxColumnLayout(fInternalWidget);
			internalWidgetLayout->setContentsMargins(1,1,1,1);
			if(isChildResizable) internalWidgetLayout->setColumnAdjustable(isChildResizable);
			fInternalWidget->setObjectName(kKxLayoutInternalWidgetName);
            fInternalWidget->setLayout(internalWidgetLayout);
            fScrollArea->setWidget(fInternalWidget);
        }
    } 
	fUnderConstruction = false;
}

// Description:  This procedure is called when someone wants to
//               add a widget to the layout (uses addItem()).
//
void KxScrollLayout::addWidget(QWidget *widget)
{
    QLayout::addWidget(widget);
}

// Description:  This procedure is called when someone wants to
//               add an item to the layout.
//
void KxScrollLayout::addItem(QLayoutItem *item)
{
    if(!fUnderConstruction) {
		if (fInternalWidget == NULL)
	    {
	        return;
	    }
		
		QLayout *internalWidgetLayout = fInternalWidget->layout();
	    if (internalWidgetLayout == NULL)
	    {
	        return;
	    }

	    QWidgetItem *wi = dynamic_cast<QWidgetItem *>(item);
	    if (wi == NULL)
	    {
	        return;
	    }
	    
	    // Make sure the item is not fScrollArea or fInternalWidget.
	    //
	    QWidget *widget = wi->widget();
	    if (widget == fScrollArea || widget == fInternalWidget)
	    {
	        return;
	    }

	    // Add the item to the internal widget.  The internal widget
	    // is used to organize what is in the scroll layout.
	    //
	    widget->setParent(fInternalWidget);
	    internalWidgetLayout->addWidget(widget);
	}
}

// Description:  This procedure returns the width of the scroll area.
//
int KxScrollLayout::scrollAreaWidth()
{
    if (fScrollArea == NULL)
    {
        return 0;
    }
    return fScrollArea->width();
}

// Description:  This procedure returns the height of the scroll area.
//
int KxScrollLayout::scrollAreaHeight()
{
    if (fScrollArea == NULL)
    {
        return 0;
    }
    return fScrollArea->height();
}

// Description:  This procedure returns the number of direct
//               none-internal child widgets in the scroll layout.
//
int KxScrollLayout::count() const
{
    if (fInternalWidget == NULL || fInternalWidget->layout() == NULL)
    {
        return 0;
    }
    return fInternalWidget->layout()->count();
}

// Description:  This procedure gets the value of the
//               top left corner of the scroll area.
//
void KxScrollLayout::getScrollAreaTopLeftCornerValue(
        int &top, int &left)
{
    top = 0;
    left = 0;

    if (fScrollArea == NULL)
    {
        return;
    }

    QScrollBar *verticalScrollBar = fScrollArea->verticalScrollBar();
    if (verticalScrollBar != NULL)
    {
        top = verticalScrollBar->value();
    }

    QScrollBar *horizontalScrollBar = fScrollArea->horizontalScrollBar();
    if (horizontalScrollBar != NULL)
    {
        left = horizontalScrollBar->value();
    } 
}

// Description:  This procedure lets the user scroll the scroll area
//               in the given direction by the given pixel amount. 
//
void KxScrollLayout::scrollByPixel(
        const QString &direction,
        int nPixels)
{
    if (fScrollArea == NULL || nPixels == 0)
    {
        return;
    }

    // Figure out which scroll bar we wanted.
    //
    QScrollBar *scrollBar = NULL;
    int delta = nPixels;
    if (direction == "up" || direction == "down")
    {
        scrollBar = fScrollArea->verticalScrollBar();
    }
    else if (direction == "left" || direction == "right")
    {
        scrollBar = fScrollArea->horizontalScrollBar();
    }
    else
    {
        // No such direction.
        //
        return;
    }

    // Figure out how much to move. 
    //
    if (direction == "up" || direction == "left")
    {
        delta = -delta;
    } 
    if (scrollBar == NULL)
    {
        return;
    }
    scrollBar->setValue(scrollBar->value()+delta);

}

// Description:  This procedure lets the user scroll the scroll area
//               by one page in the given direction.
//
void KxScrollLayout::scrollPage(const QString &direction)
{
    if (fScrollArea == NULL)
    {
        return;
    }
    
    // Figure out which scroll bar is relevant.
    //
    QScrollBar *scrollBar = NULL;
    if (direction == "up" || direction == "down")
    {
        scrollBar = fScrollArea->verticalScrollBar();
    }
    else if (direction == "left" || direction == "right")
    {
        scrollBar = fScrollArea->horizontalScrollBar();
    } 
    else
    {
        // No such direction.
        //
        return;
    }

    if (scrollBar == NULL)
    {
        return;
    }

    // Figure out how much the scroll bar value needs to be changed by.
    //
    int delta = scrollBar->pageStep();
    if (direction == "up" || direction == "left")
    {
        delta = -delta;
    }
    scrollBar->setValue(scrollBar->value()+delta);
}

// Description:  This procedure sets the command script for
//               this layout.
//
//void KxScrollLayout::setCommandScript(
//        QObject *script,
//        const char *commandType)
//{
//    if (fScrollArea == NULL)
//    {
//        return;
//    }
//    //KxQtHelper::setCommandScript(script, commandType, fScrollArea);
//}

QObjectList KxScrollLayout::getChildren() const
{
	QObjectList children;

    if(fInternalWidget != NULL && fInternalWidget->layout() != NULL) {
		QLayout *layout = fInternalWidget->layout();
		for (int index = 0; index < layout->count(); index++) {
			QLayoutItem *item = layout->itemAt(index);
			if(item && item->widget() && !gUIInventory->hasInternalName(item->widget())) { 
				children.append(item->widget());
			}
		}
	}
	
	return children;
}
