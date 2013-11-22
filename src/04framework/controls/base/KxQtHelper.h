// KxQtHelper.h
// The helper class for KxDarkStyle
//
// Copyright 2012 - 2013 by Hongwei Li. All rights reserved.
//
// Hongwei Li lihw81@gmail.com

#ifndef KxQtHelper_H_
#define KxQtHelper_H_

#include "HQtGui.h"

class KX_DLLEXPORT KxQtHelper
{
public:
    static void getMouseEventQtButtonInfo(const QInputEvent *ev, Qt::MouseButton &button, Qt::KeyboardModifiers &mods);

    static Qt::KeyboardModifiers getKeyboardModifiers(const QInputEvent *ev);
    static Qt::KeyboardModifiers getKeyboardModifiers(const QKeyEvent *ev);
    static Qt::KeyboardModifiers getKeyboardModifiers(const QDropEvent *ev);

public:
    static int		getBaseControlWidth(QWidget*);
    static int		getBaseControlHeight(QWidget*);
    static QSize	getFixedSizeWindowLayoutSizeHint(const QLayout*);

    static void installPopupEventFilter(QWidget *widget);

    static QPixmap * createPixmap(const QString &path);
    static QIcon *   createIcon(const QString &path);
    static QCursor  createCursor(const QString &stc);
    static void overlayTextOnPixmap(QPixmap *pixmap, const QString &overlayText, const QColor &backgroundColor, const QColor &textColor);
	static QPixmap getCachedPixmap(const QString &name);

public:
    static bool handlePopupMenu(QObject *object, QEvent *ev);
    static void setAllowPopupMenu(bool toggle);
    static bool allowPopupMenu();
    static bool hasHandledPopupMenu();
    static void clearHandledPopupMenuFlag();

    static void execMenu(QWidget* on, QMenu* menu, QMouseEvent* mouseEvent, bool asMarkingMenu);

private:
    // prevent popup menus , initially used by some contexts to stop RMB menu's while 
    // they are active 
    static bool         fsAllowPopupMenu;
    // Prevent hot key actions destroying menus during execution
    static bool								fsTrackingMenu;
    // prevent contexutual menu event sright after menu handling 
    static bool fsHandledPopupMenu;
    static const char * const  widgetHandlesPopupProperty;
};

#endif // !KxQtHelper_H_
