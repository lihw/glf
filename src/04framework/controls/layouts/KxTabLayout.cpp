#include "KxTabLayout.h"

#include "../base/KxUIInventory.h"
#include "../drag/KxDragManager.h"
#include "../layouts/KxColumnLayout.h"
#include "../base/KxQtHelper.h"

// Description:  Constructor.
//
KxTabLayout::KxTabLayout( QWidget *parent, bool scrollable, bool isChildResizable )
: QVBoxLayout(parent)
, fScrollable(scrollable)
, fIsChildResizable(isChildResizable)
{
    // We associate this layout with a tab widget.
    // The tab widget is the internal structure which helps tab layout
    // to organize what is in it. 
    //
    fTabWidget = new KxTabWidget(parent);
    if (fTabWidget != NULL)
    {
        fTabWidget->setObjectName(kKxLayoutInternalWidgetName);
        QVBoxLayout::addWidget(fTabWidget);
    }
	KxDragManager::addDropHandler( this, this );
}

// Description:  This procedure is called when someone wants to
//               add a widget to the layout (uses addItem()).
//
void KxTabLayout::addWidget(QWidget *widget)
{
    QLayout::addWidget(widget);
}

// Description:  This procedure is called when someone wants to add
//               an item to the layout.
//               Instead of using this to add a tab to the layout, we
//               will use QTabWidget::addTab to add tab. 
//
void KxTabLayout::addItem(QLayoutItem *item)
{
    if (fTabWidget == NULL)
    {
        return;
    }

    QWidgetItem *wi = dynamic_cast<QWidgetItem *>(item);
    if (wi == NULL)
    {
        return;
    } 
    QWidget * widget = wi->widget();
    if (widget == NULL)
    {
        return;
    }

    // Make sure the item is not fTabWidget.
    //
    if (widget == fTabWidget)
    {
        return;
    }

	// Since version 4.4, Qt emits currentChanged signal when first tab is added to tab widget.
	// Task tracker id 159433 - http://trolltech.com/developer/task-tracker/index_html?id=159433&method=entry
	// This causing us a problem, because MEL(Pyton) callback is executing during tab creation.
	// For this reason we temporarily block signals on tab widget when adding new tabs.
	bool isBlocked = fTabWidget->blockSignals(true);

    // Add the widget to the internal tab widget.
    //
    QString tmp = "";

    if (fScrollable)
    {
        // Use an intermediate scrollable area to handle the
        // scrollable feature.
        //
        QScrollArea *scrollArea = new QScrollArea();
		scrollArea->setWidgetResizable(true);
        scrollArea->setObjectName(kKxLayoutInternalWidgetName);
		scrollArea->setFocusPolicy(Qt::NoFocus);
		
        // We need an intermediate widget to bridget between
        // the scroll area and our child widget, so that
        // the child widget's content will show up in the scroll area
        // properly.
        QWidget *intermediateWidget = new QWidget();
        intermediateWidget->setObjectName(kKxLayoutInternalWidgetName);
        KxColumnLayout *intermediateWidgetLayout = new KxColumnLayout(intermediateWidget);
		intermediateWidgetLayout->setContentsMargins(1,1,1,1);
		if(fIsChildResizable) intermediateWidgetLayout->setColumnAdjustable(fIsChildResizable);
        intermediateWidget->setLayout(intermediateWidgetLayout);
    
        // Set up the relationships 
        //   fTabWidget 
        //     --> scrollArea 
        //       --> intermediateWidgetLayout 
        //         --> widget
        //
        intermediateWidgetLayout->addWidget(widget);
        scrollArea->setWidget(intermediateWidget); 
        fTabWidget->addTab(scrollArea, tmp);

		// If the child widget is deleted, the scroll area also needs
		// to be deleted in order for the tab to be removed.
		//
		QObject::connect(widget, SIGNAL(destroyed(QObject *)), scrollArea, SLOT(deleteLater()));
    }
    else
    {
        // It is not scrollabel, just add the child widget.
        //
        fTabWidget->addTab(widget, tmp);
    }
	
	// Restore previous block signal state
	fTabWidget->blockSignals(isBlocked);

	// Add the item to our layout item list.
	fItemList.append(item);
}


// Description:  This procedure add a tab with the given
//               label to the tab layout.
//
void KxTabLayout::setTabLabel(QWidget *childWidget, const QString &label)
{
    if (fTabWidget == NULL || childWidget == NULL)
    {
        return;
    }

    int index = indexOf(childWidget);
    if (index == -1)
    {
        // Did not find the child widget in the tab layout.
        //
        return;
    }
    fTabWidget->setTabText(index, label);
}

// Description:  This procedure add a tab with the given
//               label to the tab layout.
//
void KxTabLayout::setTabLabel(int index, const QString &label)
{
    if (fTabWidget == NULL) 
    {
        return;
    }

    if (index < 0 || index >= fTabWidget->count())
    {
        // The index is out of range.
        //
        return;
    }
    fTabWidget->setTabText(index, label);
}

// Description:  This procedure returns a tab's label.
//
QString KxTabLayout::tabLabel(int index) const
{
    if (fTabWidget == NULL)
    {
        return "";
    }
    return fTabWidget->tabText(index);
}

// Description:  This procedure sets the visiblility of the tabs.
//
void KxTabLayout::setTabsVisible(bool visible)
{
    if (fTabWidget == NULL)
    {
        return;
    }
    fTabWidget->setTabsVisible(visible);
}

// Description:  This procedure returns true if the tabs are visible,
//               and false otherwise.
//
bool KxTabLayout::tabsAreVisible()
{
    if (fTabWidget == NULL)
    {
        return false;
    }
    return fTabWidget->tabsAreVisible();
}

bool KxTabLayout::setImage(const QString & image)
{
	if(NULL == fTabWidget) return false;

	bool result = false;

	if(image.length() > 0) {
		if (image == fImageName) {
			result = true;
		} else {
		    QPixmap *pixmap = KxQtHelper::createPixmap(image);
			if(pixmap != NULL) {
				QLabel *picture = dynamic_cast<QLabel*>(fTabWidget->cornerWidget());
				if(picture == NULL) {
					picture = new QLabel();
					fTabWidget->setCornerWidget(picture);
				}
				picture->setPixmap(*pixmap);
				delete pixmap;
				fImageName = image;
				result = true;
			}
		}
	}
		
	return result;
}


const QString &	KxTabLayout::image() const
{
	return fImageName;
}

void KxTabLayout::showImage()
{
	if(NULL == fTabWidget) return;

	fTabWidget->showCornerWidget();
}

void KxTabLayout::hideImage()
{
	if(NULL == fTabWidget) return;

	fTabWidget->hideCornerWidget();
}

bool KxTabLayout::imageVisible() const
{
	if(NULL == fTabWidget) return false;

	return (fTabWidget->cornerWidget() != NULL);
}

QWidget * KxTabLayout::cornerWidget()
{
	if(NULL == fTabWidget){
		return( NULL );
	} else {
		return( fTabWidget->cornerWidget() );
	}
}

// Description:  This procedure returns the currently displayed tab 
//               in the tab layout.
//
QWidget * KxTabLayout::currentWidget()
{
    if (fTabWidget == NULL)
    {
        return NULL;
    }
    
    if (fScrollable)
    {
        // If the layout is scrollable, we need to find the current 
        // widget by skipping scroll area related intermediate 
        // internal widgets.
        //
        QScrollArea * scrollArea = (QScrollArea *)(fTabWidget->currentWidget());
        if (scrollArea != NULL)
        {
            QWidget *intermediateWidget = scrollArea->widget();
            if (intermediateWidget != NULL)
            {
                // The intermediateWidget should have only one
                // child which is a widget.  That is the 
                // child widget which is currently showing in the tab.
                //
                QObjectList children = intermediateWidget->children();
                foreach (QObject *object, children)
                {
                    QWidget *childWidget = dynamic_cast<QWidget *>(object);
                    if (childWidget != NULL)
                    {
                        return childWidget;
                    }
                }
            }
        }
         
    }

    return fTabWidget->currentWidget();
}

// Description:  This procedure returns the index of the currently 
//               displayed tab in the tab layout.
//
int KxTabLayout::currentIndex()
{
    if (fTabWidget == NULL)
    {
        return -1;
    }
    return fTabWidget->currentIndex();
}

// Description:  This procedure selects the given child widget.
//
void KxTabLayout::selectWidget(QWidget *widget)
{
    if (fTabWidget == NULL)
    {
        return;
    }
    int index = indexOf(widget);
    if (index == -1)
    {
        // Can not find this child widget in the layout.
        //
        return;
    }
    fTabWidget->setCurrentIndex(index);
	
	// Refresh the selected widget's layout so that all its UI elements appear properly
	if(widget != NULL && widget->layout() != NULL) {
		widget->layout()->invalidate();
		widget->layout()->setGeometry(widget->rect());
	}
}

// Description:  This procedure selects the tab with the given index.
//
void KxTabLayout::selectWidget(int index)
{
    if (fTabWidget == NULL)
    {
        return;
    }
    fTabWidget->setCurrentIndex(index);
	
	// Refresh the selected widget's layout so that all its UI elements appear properly
	QWidget *widget = fTabWidget->widget(index);
	if(widget != NULL && widget->layout() != NULL) {
		widget->layout()->invalidate();
		widget->layout()->setGeometry(widget->rect());
	}
}

// Description:  This procedure returns the number of tabs in the
//               tab layout.
//
int KxTabLayout::count() const
{
    if (fTabWidget == NULL)
    {
        return 0;
    }
    return fTabWidget->count();
}

// Description:  This procedure finds the layout item at the given index.
//
QLayoutItem * KxTabLayout::itemAt(int index) const
{
	if(fItemList.count() > index) {
		return fItemList[index];
	} else {
		return NULL;
	}
}

// Description:  This procedure removes the layout item at the given index
//               from the item list and returns it.
//
QLayoutItem * KxTabLayout::takeAt(int index)
{
	if(fTabWidget && fItemList.count() > index) {
		fTabWidget->removeTab(index);
		return fItemList.takeAt(index);
	} else {
		return NULL;
	}
}

// Description:  This procedure returns the preferred size of the
//               tab layout.
//
QSize KxTabLayout::sizeHint() const
{
	// If the layout is the top-level layout of a fixed-size window,
	// use any size settings the window has for the size hint values.
	// Use the parent class' size hint values for any the fixed size
	// hint lacks.
	//
	QSize fixedSize = KxQtHelper::getFixedSizeWindowLayoutSizeHint(this);
	if(!fixedSize.isValid()) {
		QSize defaultSize = QVBoxLayout::sizeHint();
		if(fixedSize.width() >= 0) {
			return QSize(fixedSize.width(), defaultSize.height());
		} else if(fixedSize.height() >= 0) {
			return QSize(defaultSize.width(), fixedSize.height());
		} else {
			return defaultSize;
		}
	} else {
		return fixedSize;
	}
}

// Description:  This procedure moves the tab with the fromIndex to
//               the toIndex.
//
void KxTabLayout::moveTab(int fromIndex, int toIndex)
{
    if (fTabWidget == NULL)
    {
        return;
    }
    if (fromIndex < 0 || toIndex < 0 
        || fromIndex >= fTabWidget->count()
        || toIndex >= fTabWidget->count())
    {
        // Index out of bound.
        //
        return;
    }
 
    // Save the widget infomation.
    //
    QString label = fTabWidget->tabText(fromIndex);
    QWidget *widget = fTabWidget->widget(fromIndex);
    fTabWidget->removeTab(fromIndex);

	// Take the layout item out of the item list.
	QLayoutItem * item = NULL;
	if(fromIndex < fItemList.count()) {
		item = fItemList.takeAt(fromIndex);
	}

	fTabWidget->insertTab(toIndex, widget, label);

	// Reinsert the layout item into the item list at the new index.
	if(item && toIndex <= fItemList.count()) {
		fItemList.insert(toIndex, item);
	}
}

// Description:  This procedure is called to link a control signal
//               to a script action.
//
//void KxTabLayout::linkControlSignalToScriptAction(
//        const char* signal,
//        QObject* receiverScript,
//        const char* commandType)
//{
//    if (fTabWidget == NULL)
//    {
//        return;
//    }
//    KxQtHelper::linkControlSignalToScriptAction(signal, receiverScript, commandType, fTabWidget);
//}
//
//// Description:  This procedure is called to set the command script
////               for the given command type.
////
//void KxTabLayout::setCommandScript(
//        QObject *script,
//        const char * commandType)
//{
//    if (fTabWidget == NULL)
//    {
//        return;
//    }
//    fTabWidget->setCommandScript(script, commandType);
//}
//
//// Description:  This procedure is called to return the command script
////               for a given command type.
////
//QObject * KxTabLayout::commandScript(const char * commandType)
//{
//    if (fTabWidget == NULL)
//    {
//        return NULL;
//    }
//    return fTabWidget->commandScript(commandType);
//}
//
QObjectList KxTabLayout::getChildren() const
{
	QObjectList children;

	if(fTabWidget != NULL) {
		QWidget *child = NULL;
		for(int index = 0; index < fTabWidget->count(); index++) {
			if(fScrollable) {
				// If the layout is scrollable, we need to find the current 
				// widget by skipping scroll area related intermediate 
				// internal widgets.
				//
				QScrollArea * scrollArea = dynamic_cast<QScrollArea *>(fTabWidget->widget(index));
				if(scrollArea != NULL) {
					QWidget *intermediateWidget = scrollArea->widget();
					if(intermediateWidget != NULL) {
						// The intermediateWidget should have only one
						// child which is a widget.  That is the 
						// child widget which is currently showing in the tab.
						//
						QObjectList internalChildren = intermediateWidget->children();
						foreach(QObject *object, internalChildren) {
							QWidget *childWidget = dynamic_cast<QWidget *>(object);
							if(childWidget && !gUIInventory->hasInternalName(childWidget)) {
								children.append(childWidget);
								break;
							}
						}
					}
				}
			} else {
				child = fTabWidget->widget(index);
				if(child && !gUIInventory->hasInternalName(child)) {
					children.append(child);
				}
			}
		}
	}
	
	return children;
}


// Description:  This procedure finds the index of the child widget.
//
int KxTabLayout::indexOf(QWidget *childWidget)
{
    if (fTabWidget == NULL)
    {
        return -1;
    }

    if (fScrollable)
    {
        // In case the tab layout is scrollable, we must have placed
        // the childWidget inside of a QScrollArea and added the
        // QScrollArea to the tab.
        //
        // The relationship between scrollArea and the childWidget is
        // as shown below:
        //
        //   fTabWidget 
        //     --> scrollArea 
        //       ->+ q_scrollarea_viewport (an intermediate widget
        //                    automatically generated when QScrollAre 
        //                    was initialized)
        //         --> intermediateWidgetLayout 
        //           --> childWidget
        //
        // So to go from childWidget to scrollArea, we need to
        // trace back 3 layers of parents.
        //
        if (childWidget->parent() && childWidget->parent()->parent())
        {
            QWidget *scrollArea = (QWidget *)(childWidget->parent()->parent()->parent());
            return fTabWidget->indexOf(scrollArea);
        }
    }

    return fTabWidget->indexOf(childWidget); 
}

QRect	KxTabLayout::getDropRect(QWidget* target,
									QDropEvent* event,
									int,
									void* )
{
	QRect dropRect;
	QPoint dropPos = event->pos();
	// Check if the target is one of the tabs
	//
	QTabBar* tabBar = fTabWidget->tabBar();
	if( tabBar != NULL ){
		int numTabs = tabBar->count();
		int tabIndex = 0;
		while( tabIndex < numTabs ){
			if( tabBar->tabRect( tabIndex ).contains(dropPos) ){
				return( tabBar->tabRect( tabIndex ) );
			}
			tabIndex++;
		}
	}
	// If not, drop rect is whole target
	//
	return( target->rect() );
}

void KxTabLayout::getDropTypes(	const QWidget* target,
									const QDropEvent* event,
									KxDragItemDataTypeArray& dropTypes
) const
{
	// First, check if the target is the corner widget (e.g., trash can) --
	// we'll let it handle itself, and return no types that are valid.
	//
	if(fTabWidget->cornerWidget() != NULL){
		if( fTabWidget->cornerWidget()->geometry().contains(event->pos()) ){
			return;
		}
	}
	// Get a list of children, go throught the drop handlers, and gather all
	// the valid drop types.
	//
	QObjectList children = getChildren();
	foreach( QObject* childObject, children ){
		QWidget* childWidget = dynamic_cast<QWidget*>(childObject);
		if( childWidget != NULL ){
			const QList<KxDropHandler*>* dropH =
								KxDragManager::getDropHandlers(childWidget);
			if( dropH != NULL ){
				int numHandlers = dropH->count();
				int handlerIndex = 0;
				while( handlerIndex < numHandlers ){
					KxDragItemDataTypeArray childDropTypes;
					dropH->at(handlerIndex)->getDropTypes( target, event,
						childDropTypes );
					int numDropTypes = childDropTypes.length();
					int typeIndex = 0;
					while( typeIndex < numDropTypes ){
						KxDragItemDataType childType = childDropTypes[typeIndex];
						if( true != dropTypes.contains(childType) ){
							dropTypes.append(childType);
						}
						typeIndex++;
					}
					handlerIndex++;
				}
			}
		}
	}
}


bool KxTabLayout::dropData(
	QWidget*			target,
	const QDropEvent*	event,
	int					dataType,
	void*				data
)
{
	QWidget* dropWidget = NULL;
	QPoint dropPos = event->pos();
	// Next, check if we're dropping on the tabs
	//
	if( dropWidget == NULL ){
		QTabBar* tabBar = fTabWidget->tabBar();
		if( tabBar != NULL ){
			int numTabs = tabBar->count();
			int tabIndex = 0;
			while( tabIndex < numTabs ){
				if( tabBar->tabRect( tabIndex ).contains(dropPos) ){
					dropWidget = fTabWidget->widget( tabIndex );
					break;
				}
				tabIndex++;
			}
		}
	}
	// Otherwise, we're dropping on the current widget
	//
	if( dropWidget == NULL ){
		dropWidget = currentWidget();
	}
	bool dropHandled = false;
	if( dropWidget != NULL ){
		// Check if we have a drop handler that handles this dataType
		//
		const QList<KxDropHandler*>* dropH =
							KxDragManager::getDropHandlers(dropWidget);
		if( dropH != NULL ){
			int numHandlers = dropH->count();
			int handlerIndex = 0;
			while( handlerIndex < numHandlers ){
				KxDragItemDataTypeArray childDropTypes;
				dropH->at(handlerIndex)->getDropTypes( target, event,
					childDropTypes );
				if( childDropTypes.contains(dataType) == true ){
					if( dropH->at(handlerIndex)->dropData( target,
													event, dataType, data ) ){
						dropHandled = true;
					}
				}
				handlerIndex++;
			}
		}
	}

	return( dropHandled );
}

