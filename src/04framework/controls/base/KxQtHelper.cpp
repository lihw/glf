// KxQtHelper.cpp
// The helper class for KxDarkStyle
//
// Copyright 2012 - 2013 by Hongwei Li. All rights reserved.
//
// Hongwei Li lihw81@gmail.com


#include "KxQtHelper.h"

#include <QtSvg/QtSvg>
#include "KxUIInventory.h"
#include "../menus/KxPopupMenu.h"

bool KxQtHelper::fsAllowPopupMenu = true;
bool KxQtHelper::fsTrackingMenu = false;
bool KxQtHelper::fsHandledPopupMenu = false;
const char * const KxQtHelper::widgetHandlesPopupProperty = "widgetHandlesPopup";

void KxQtHelper::getMouseEventQtButtonInfo(const QInputEvent *ev, Qt::MouseButton &button, Qt::KeyboardModifiers &mods)
{
    button = Qt::NoButton;
    mods = Qt::NoModifier;
    if(ev == NULL) return;

    const QMouseEvent *mouseEvent = dynamic_cast<const QMouseEvent *>(ev);
    if(mouseEvent != NULL){
        // There are couple of problems this method deals with:
        //	- For mouse release event buttons() will exclude the actual button caused the event, so we have to use button() instead.
        //  - On MacOS X, in order to make code more cross platform, Control and Command modifiers are swapped by Qt framework,
        //    Kiwi doesn't need it, because we deal with this on differnt level.
        if(mouseEvent->type() == QEvent::MouseButtonRelease){
            button = mouseEvent->button();
        }else{
            Qt::MouseButtons buttons = mouseEvent->buttons();
            if(buttons & Qt::LeftButton)  button = Qt::LeftButton;
            if(buttons & Qt::RightButton) button = Qt::RightButton;
            if(buttons & Qt::MidButton)   button = Qt::MidButton;
        }
    }
    mods = getKeyboardModifiers(ev);
    //switch(KxResourceQuery::getNumberOfMouseButtons()){
    //    case 1:
    //        {
    //            if(mods & Qt::ControlModifier){
    //                // ctrl + LMB  --> RMB
    //                button = Qt::RightButton;
    //                mods &= ~Qt::ControlModifier;
    //            }else if(mods & Qt::MetaModifier){
    //                // cmd +LMB --> MMB 
    //                button = Qt::MidButton;
    //                mods &= ~ Qt::MetaModifier;
    //            }
    //        }
    //        break;
    //    case 2:
    //        {
    //            if(button == Qt::LeftButton && (mods & Qt::MetaModifier)){
    //                // cmd + LMB -->MMB
    //                button = Qt::MidButton;
    //                mods &= ~ Qt::MetaModifier;
    //            }
    //        }
    //        break;
    //}
}
 /*!
  QInputEvent version: returns previous modifiers before the event occured.

  Translate the Qt input event keyboard modifiers.  This handles the MacOS-specific issues.
  On MacOS X, in order to make code more cross platform, Control and Command modifiers are swapped,
  Kiwi doesn't need it, because we deal with this on differnt level.
 */
Qt::KeyboardModifiers  KxQtHelper::getKeyboardModifiers(const QInputEvent *ev)
{
    Q_ASSERT(ev);
    Qt::KeyboardModifiers mods;
    if (ev) {
        mods = ev->modifiers();
#if defined(OSMac_)
        if(ev->modifiers() & Qt::ControlModifier) {
            mods |= Qt::MetaModifier;
        } else {
            mods &= ~Qt::MetaModifier;
        }
        if(ev->modifiers() & Qt::MetaModifier) {
            mods |= Qt::ControlModifier;
        } else {
            mods &= ~Qt::ControlModifier;
        }
#endif
    }
    return mods;
}

 /*!
	QKeyEvent version: returns current modifiers for the event passed in.

  Translate the Qt input event keyboard modifiers.  This handles the MacOS-specific issues.
  On MacOS X, in order to make code more cross platform, Control and Command modifiers are swapped,
  Kiwi doesn't need it, because we deal with this on differnt level.
 */
Qt::KeyboardModifiers  KxQtHelper::getKeyboardModifiers(const QKeyEvent *ev)
{
    Q_ASSERT(ev);
    Qt::KeyboardModifiers mods;
    if (ev) {
        mods = ev->modifiers();
#if defined(OSMac_)
        if(ev->modifiers() & Qt::ControlModifier) {
            mods |= Qt::MetaModifier;
        } else {
            mods &= ~Qt::MetaModifier;
        }
        if(ev->modifiers() & Qt::MetaModifier) {
            mods |= Qt::ControlModifier;
        } else {
            mods &= ~Qt::ControlModifier;
        }
#endif
    }
    return mods;
}

Qt::KeyboardModifiers  KxQtHelper::getKeyboardModifiers(const QDropEvent *ev)
{
    Q_ASSERT(ev);
    Qt::KeyboardModifiers mods;
    if (ev) {
        mods = ev->keyboardModifiers();
#if defined(OSMac_)
        if(ev->keyboardModifiers() & Qt::ControlModifier) {
            mods |= Qt::MetaModifier;
        } else {
            mods &= ~Qt::MetaModifier;
        }
        if(ev->keyboardModifiers() & Qt::MetaModifier) {
            mods |= Qt::ControlModifier;
        } else {
            mods &= ~Qt::ControlModifier;
        }
#endif
    }
    return mods;
}


/*static*/
int KxQtHelper::getBaseControlWidth(QWidget *w)
{
    int widgetWidth = 0;
    QVariant v = w->property("explicitWidth");
    if(v.isValid() && v.canConvert<int>()) {
        // If the user explicitly set a width for this widget, use it.
        widgetWidth = v.value<int>();
    } else {
        // Otherwise, we want the width to be at least the widget's minimum width.
		widgetWidth = KxFoundation::max(w->sizeHint().width(), w->minimumWidth());
    }
    return widgetWidth;

}

/*static*/
int KxQtHelper::getBaseControlHeight(QWidget *w)
{
    int widgetHeight = 0;
    QVariant v = w->property("explicitHeight");
    if(v.isValid() && v.canConvert<int>()) {
        // If the user explicitly set a height for this widget, use it.
        widgetHeight = v.value<int>();
    } else {
        // Otherwise, we want the height to be at least the widget's minimum height.
		widgetHeight = KxFoundation::max(w->sizeHint().height(), w->minimumHeight());
    }
    return widgetHeight;
}

/*static*/
QSize KxQtHelper::getFixedSizeWindowLayoutSizeHint(const QLayout *layout)
// Description:  If the given layout is the primary (i.e. top-level) layout
//               of a fixed-size window, return a modified size hint which
//               reflects any width and/or height setting that was given
//               directly to the window. Otherwise, return an invalid size
//               value.
//
{
    QSize size;
    QWidget* parentWindow = NULL;
    if (layout->parentWidget() && layout->parentWidget()->isWindow()) {
        parentWindow = layout->parentWidget();
    }

    if(parentWindow) {
        QVariant v = parentWindow->property("fixedSize");
        if(v.isValid() && v.toBool()) {
            // Just to be safe, don't do it for the main window.
            v = qApp->property("mainWindow");
            QWidget* mainWindow = NULL;
            if(v.isValid()) {
                mainWindow = (QWidget*)(v.value<void*>());
            }
            if (parentWindow != mainWindow) {
                // Check if the width and height of the window
                // were explicitly set, and if so, set the size
                // according to those values.
                v = parentWindow->property("explicitWidthSet");
                if (v.isValid()) {
                    size.setWidth(v.toInt());
                }
                v = parentWindow->property("explicitHeightSet");
                if (v.isValid()) {
                    size.setHeight(v.toInt());
                }
            }
        }
    }

    return size;
}


QPixmap * KxQtHelper::createPixmap(const QString & path)
{
   QString ext = path.mid(path.lastIndexOf("."));
   QPixmap * pixmap = NULL;
   // svg file must be renderered onto a pixmap 
   if(ext == "svg"){
       QSvgRenderer svgRender(path);
       const QRectF box = svgRender.viewBoxF();
       if(svgRender.isValid() && box.isValid()){
           pixmap = new QPixmap(box.size().toSize());
           pixmap->fill(Qt::transparent);
           QPainter pixPainter(pixmap);
           svgRender.render(&pixPainter);
       }
   }
   if(NULL == pixmap){
       pixmap = new QPixmap(path);
   }
   if(pixmap->isNull()){
       delete pixmap;
       return NULL;
   }
   return pixmap;
}

/*!
  \brief Overlay the given text on top of the given pixmap. Used
  by buttons and toolButtons.

  \param[in] pixmap The pixmap to overlay with text
  \param[in] overlayText The text to overlay
  \param[in] backgroundColor The color of the shadow behind the text
  \param[in] textColor The text color
 */
void KxQtHelper::overlayTextOnPixmap(QPixmap* pixmap, const QString& overlayText, const QColor& backgroundColor, const QColor& textColor)
{
	if (pixmap == NULL) return;

	QPainter p(pixmap);
	QFont f = p.font();
	f.setBold(true);
	p.setFont(f);
	// get bounding rect for text
	QRect textRect = p.boundingRect(pixmap->rect(),
									Qt::AlignVCenter | Qt::AlignHCenter,
									overlayText);

	// Left-align the text if it is wider than the pixmap
	// so that we can see the start of the text
	int alignmentFlags = Qt::AlignVCenter | Qt::AlignHCenter;
	if(textRect.width() > pixmap->rect().width()) {
		alignmentFlags = Qt::AlignVCenter | Qt::AlignLeft;
	}

	// Add 1 pixel above and under the text, center it.
	const int margin = 1;
	int height = textRect.height();
	textRect.setTop(pixmap->rect().bottom() - height - margin);
	textRect.setLeft(margin);
	textRect.setHeight(height);
	textRect.setWidth(pixmap->rect().width() - margin*2);

	// Draw a transparent rounded area (20%)
	const int radius = 2;
	QPainterPath path;
	path.addRoundedRect(textRect, radius, radius);
	p.fillPath(path, backgroundColor);

	// Draw the text
	p.setPen(textColor);
	p.drawText(textRect, alignmentFlags, overlayText);
}


QIcon * KxQtHelper::createIcon(const QString &path)
{
    QIcon *icon = NULL;
    icon = new QIcon(path);
    if(icon != NULL && icon->pixmap(1).isNull()){
        delete icon;
        return NULL;
    }
    return icon;
}

QCursor KxQtHelper::createCursor(const QString &str)
{
    //expect format "cusorName:intHitX,intHixY"
    QStringList cursorElements = str.split(',');
    QString fileName = cursorElements[0];
    int hitX = cursorElements[1].toInt();
    int hitY = cursorElements[2].toInt();
    QPixmap pmap(":/" + fileName); // gets the image file out of the QT resource 
    QCursor cursor(pmap,hitX,hitY);
    return cursor;
}


//======================= Popup menu handling =========================

// This event filter object will be installed on widgets with popup menu(s)
class PopupEventFilterObject : public QObject
{
protected:
    virtual bool	eventFilter(QObject *, QEvent *);
};

bool PopupEventFilterObject::eventFilter(QObject *object, QEvent *event)
{
    if(event->type() == QEvent::MouseButtonPress) {
        // Handle popup menus only if one button is pressed
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        Qt::MouseButtons buttons = mouseEvent->buttons();
        if(buttons == Qt::LeftButton || buttons == Qt::MidButton || buttons == Qt::RightButton) {
            return KxQtHelper::handlePopupMenu(object, event);
        }
    }
    // If not handled, pass for default processing
    return false;
}


// -- Create single static instance of event fiter object
PopupEventFilterObject *sPopupEventFilter = new PopupEventFilterObject();

void KxQtHelper::installPopupEventFilter(QWidget *widget)
{
    // Install event filter on the widget, so popup menu could be handled
    if(widget && sPopupEventFilter) {
        // If the widget handles popup menus itself then don't install the
        // event filter. Currently used by treeView.
        if (!(widget->property(widgetHandlesPopupProperty)).isValid()) {
            widget->installEventFilter(sPopupEventFilter);
        }

        // Now, if this widget has a layout, install filter on children as well
        if(widget->layout()) {
            QObjectList children = gUIInventory->getLayoutChildren(widget->layout());
            foreach(QObject *obj, children) {
                if(obj->isWidgetType()) {
                    installPopupEventFilter((QWidget*)obj);
                }
            }
        }
    }
}

void KxQtHelper::setAllowPopupMenu(bool toggle)
{
    fsAllowPopupMenu = toggle;
}

bool KxQtHelper::allowPopupMenu()
{
    return fsAllowPopupMenu;
}

bool KxQtHelper::hasHandledPopupMenu()
{
    return fsHandledPopupMenu;
}

void KxQtHelper::clearHandledPopupMenuFlag()
{
    fsHandledPopupMenu = false;
}


/*!
  \brief Invoke any popup menu as appropriate
  \return true if the event was handled, false otherwise
*/
bool KxQtHelper::handlePopupMenu(QObject *object, QEvent *event)
{
	if (!allowPopupMenu())
		return false;

	if(object->isWidgetType() && !((QWidget*)object)->isEnabled()) {
		return false;
	}

	bool isHandled = false;

	// Try popup menus attached to this object
	QList<QObject *> childList = object->children();
	if(!childList.isEmpty()) {
		QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
		Qt::MouseButton button;
		Qt::KeyboardModifiers mods;
		getMouseEventQtButtonInfo(mouseEvent, button, mods);

		foreach(QObject *child, childList) {
			KxPopupMenu *menu = qobject_cast<KxPopupMenu*>(child);
			if(NULL == menu) continue;

			if(menu->popupMenuButton() == button && menu->popupMenuModifiers() == mods) {
                QMouseEvent ev(mouseEvent->type(),
                               mouseEvent->pos(),
                               mouseEvent->globalPos(),
                               button,
                               mouseEvent->buttons(),
                               mods);
				fsHandledPopupMenu = true;
                execMenu(qobject_cast<QWidget*>(object), menu, &ev, menu->displayAsMarkingMenu());
				// Stop processing
				isHandled = true;
				break;
			}
		}
	}

	// Try parent
	if(!isHandled && object->isWidgetType()) {
		QObject *parent = gUIInventory->getParent(object);
		if(parent) {
			QWidget *parentWidget = NULL;
			if(parent->isWidgetType()) {
				parentWidget = (QWidget *)parent;
			} else if(parent->inherits("QLayout")) {
				parentWidget = ((QLayout *)parent)->parentWidget();
			}
			if(parentWidget) {
				isHandled = handlePopupMenu(parentWidget, event);
			}
		}
	}

	return isHandled;
}

/*! 
  \brief Invoke a menu
  \param[in] on The widget that this menu is being invoked by (can be null)
  \param[in] menu the menu to be posted
  \param[in] mouseEvent used to determine initial position
  \param[in] asMarkingMenu true means the menu is a marking menu
*/
void KxQtHelper::execMenu(QWidget* on, QMenu* menu, QMouseEvent* mouseEvent, bool asMarkingMenu)
{
    // Check if we're already tracking a menu (could be the
	// case if a menu action spawns a separate menu - see bug
	// #360030)
	bool alreadyTrackingMenu = fsTrackingMenu;
	fsTrackingMenu = true;

 //   if(asMarkingMenu) {
 //       // temporarily set the marking menu parent to the supplied widget
 //       // and post the marking menu
 //       QPointer<QWidget> mmparent =  KxMarkMenuController::markMenuParentWidget();
 //       KxMarkMenuController::setMarkMenuParentWidget(on);
 //       KxMarkMenuController::execMarkingMenu(menu, mouseEvent);
 //       KxMarkMenuController::setMarkMenuParentWidget(mmparent);
 //   } else {
 //       menu->exec(mouseEvent->globalPos());
 //   }
	//foreach(KxQtHelper::DelayedHotkeyAction delayedAction, fsDelayedHotkeyActions) {
	//	if (delayedAction.first->doIt(Tevent()) && delayedAction.second) {
	//		gTheRepeatCommandManager->addCommand(delayedAction.first);
	//	}
	//	delete delayedAction.first;
	//}
 //   fsDelayedHotkeyActions.clear();

	// Set fsTrackingMenu back to the tracking state
	// of the previous menu, if any
	//fsTrackingMenu = alreadyTrackingMenu;
}



QPixmap KxQtHelper::getCachedPixmap(const QString &name)
{
	QPixmap pix;
	if(!QPixmapCache::find(name,pix)){
		QPixmap *newPix = new QPixmap(name);
		if(newPix){
			QPixmapCache::insert(name,*newPix);
			pix = *newPix;
			delete newPix;
			Q_ASSERT(!pix.isNull());
		}
	}
	return pix;
}
