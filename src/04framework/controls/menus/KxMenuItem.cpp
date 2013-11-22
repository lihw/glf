#include "KxMenuItem.h"
#include "../base/KxQtHelper.h"

#if defined(OSMac_)
# include <KxMacMainMenuBar.h>
#endif

#define kShelfMenuItemCmd		"menuItemToShelf \""
#define kBackSlashDoubleQuote			"\""

enum {
    kShortcutRightMargin = 5
};

// ============================ Widget ============================
const int KxMenuItemWidget::sNumTabletMovesToSkip = 7;

KxMenuItemWidget::KxMenuItemWidget(KxMenuItem *item, QWidget *parent)
: QWidget(parent)
, fMenuItem(item)
, fTabletMoveSkipCount(0)
, fSkippingTabletMoves(false)
{
    setMouseTracking(true);
}

QSize KxMenuItemWidget::sizeHint() const
{
    // Do not give invisible menu items any space
    if(!fMenuItem->isVisible()) return QSize(0,0);

    QSize sz;

    QWidget *q = parentWidget();
    QStyleOptionMenuItem opt = getStyleOption();
    KxMenuItem *action = fMenuItem;

    const int hmargin = q->style()->pixelMetric(QStyle::PM_MenuDesktopFrameWidth, 0, q),
        iconWidth = q->style()->pixelMetric(QStyle::PM_SmallIconSize, 0, q);

    QFontMetrics fm(action->font().resolve(q->font()));
    if (action->isSeparator()) {
        sz = QSize(2, 2);
    } else {
        // Text
        QString s = action->text();
        int w = fm.width(s);
        QKeySequence shortcutSeq = fMenuItem->shortcut();
        sz.setWidth(w);
        sz.setHeight(fm.height());
        // Keyboard shortcut
        if(!shortcutSeq.isEmpty()) {
            QString shortcut = shortcutSeq.toString(QKeySequence::NativeText);
            w = fm.width(shortcut);
            sz.rwidth() += w + kShortcutRightMargin;
        }
        // Checkbox
        if(action->isCheckable()) {
            sz.rwidth() += iconWidth;
        }
        // option boxes
        sz.rwidth() += iconWidth;
        // Margins
        sz.rwidth() += 2 * hmargin;
    }

    opt.rect = q->rect();
    sz = q->style()->sizeFromContents(QStyle::CT_MenuItem, &opt, sz, q);

    return sz;
}

void KxMenuItemWidget::paintEvent(QPaintEvent *event)
{
    // Do not draw invisible menu items
    if(!fMenuItem->isVisible()) return;

    QStylePainter painter(this);
    QStyleOptionMenuItem opt = getStyleOption();
    QRect boxRect;
    bool inOptionBox = false;

    QWidget *q = parentWidget();

    const int hmargin = q->style()->pixelMetric(QStyle::PM_MenuDesktopFrameWidth, 0, q),
        iconWidth = q->style()->pixelMetric(QStyle::PM_SmallIconSize, 0, q);

    if(!fMenuItem->isSeparator()) {
        if(fMenuItem->hasOptionBox()) {
            boxRect.setRect(rect().right() - hmargin - iconWidth, rect().top(), iconWidth, rect().height());
            QPoint p = QCursor::pos();
            p = mapFromGlobal(p);
            if(boxRect.contains(p)) {
                inOptionBox = true;
            } else {
                // Subtract option box rect from style option rect
                int newRight = opt.rect.right() - iconWidth - hmargin;
                opt.rect.setRight(newRight);
            }
        }
    }
    // Draw general menu item
    opt.rect.adjust(0, 0, -1, 0);
    painter.drawControl(QStyle::CE_MenuItem, opt);
    // Draw shortcut
    QKeySequence shortcutSeq = fMenuItem->shortcut();
    if(!shortcutSeq.isEmpty()) {
        // shortcut bounds
        QRect scRect = opt.rect;
        QString shortcut = shortcutSeq.toString(QKeySequence::NativeText);
        QMenu *menu = qobject_cast<QMenu *>(parentWidget());
        Q_ASSERT(menu != NULL);
        int shortcutWidth = 12;	// default value in case there is no font
        if ( menu ) {
            QFontMetrics metrics(fMenuItem->font().resolve(menu->font()));
            shortcutWidth = metrics.width(shortcut);
        }
        scRect.setLeft(scRect.right() - shortcutWidth);
        if(inOptionBox || !fMenuItem->hasOptionBox()) {
            scRect.translate(-iconWidth, 0);
        }
        scRect.translate(-kShortcutRightMargin, 0);
        painter.drawItemText(scRect, Qt::AlignRight | Qt::AlignVCenter, palette(), true, shortcut);
    }
    // Draw option box
    if(!fMenuItem->isSeparator() && fMenuItem->hasOptionBox()) {
        QIcon* boxIcon = NULL;
        QVariant v = fMenuItem->optionBoxAction()->property( "optionBoxIcon" );
        if ( v.isValid() ) {
            QString optionBoxIcon;
            optionBoxIcon = v.toString();
            boxIcon = KxQtHelper::createIcon( optionBoxIcon );
        }
        if ( boxIcon == NULL ) {
            boxIcon = new QIcon(":/optionBox.png");
        }
        boxRect.setRect(rect().right() - hmargin - iconWidth, rect().top()+(rect().height()-iconWidth)/2, iconWidth, iconWidth);
        boxIcon->paint(&painter, boxRect, Qt::AlignCenter, fMenuItem->isEnabled() ? QIcon::Normal : QIcon::Disabled);
        delete boxIcon;
    }
}

void KxMenuItemWidget::tabletEvent(QTabletEvent* event)
{
    // On press of a menu item that is actually a submenu, the submenu is
    // activated to be shown with a 20ms delay, just like a mouse press.
    // However, due to the extra jitteriness of a tablet, tablet move events
    // come through before the delay finishes and Qt resets the show timer to be
    // 96ms, which is the standard show delay for hovering on a submenu. So, we
    // need to skip a few move events after a click to allow the menu enough
    // time to show (few has been empirically determined to be 7).
    //
    if (fMenuItem && fMenuItem->menu()) {
        if (event->type() == QEvent::TabletPress) {
            // press event has come in, start skipping
            fSkippingTabletMoves = true;
            fTabletMoveSkipCount = 0;

        } else if (fSkippingTabletMoves && event->type() == QEvent::TabletMove) {
            // handle move event if we're skipping moves
            if (fTabletMoveSkipCount >= sNumTabletMovesToSkip) {
                // we've skipped enough, stop skipping
                fSkippingTabletMoves = false;
            } else {
                // We haven't skipped enough yet; accept the event and
                // return early to prevent further processing
                ++fTabletMoveSkipCount;
                event->accept();
                return;
            }
        }
    }

    QWidget::tabletEvent(event);
}

void KxMenuItemWidget::mouseMoveEvent(QMouseEvent * event)
{
    QWidget::mouseMoveEvent(event);
    if(fMenuItem->hasOptionBox()) {
        update();
    }
}

void KxMenuItemWidget::mouseReleaseEvent(QMouseEvent * event)
{
    bool triggerOptionBoxAction = false;

    Qt::KeyboardModifiers mods = event->modifiers();

    bool optionBoxModifier = (mods == Qt::ShiftModifier);
    bool saveToShelfModifier = (mods == (Qt::ShiftModifier|Qt::ControlModifier));

    if(!fMenuItem->isSeparator() && fMenuItem->isEnabled()) {
        if(fMenuItem->hasOptionBox()) {
            QRect boxRect;
            QWidget *q = parentWidget();
            const int hmargin = q->style()->pixelMetric(QStyle::PM_MenuDesktopFrameWidth, 0, q),
                iconWidth = q->style()->pixelMetric(QStyle::PM_SmallIconSize, 0, q);
            boxRect.setRect(rect().right() - hmargin - iconWidth, rect().top(), iconWidth, rect().height());
            if(boxRect.contains(event->pos()) || optionBoxModifier) {
                triggerOptionBoxAction = true;
                QMenu *menu = qobject_cast<QMenu *>(parentWidget());
                if(menu && !saveToShelfModifier) {
                    menu->setActiveAction(NULL);
                }
            }
        }
    }

    if(saveToShelfModifier) {
        if(fMenuItem->saveToShelf(triggerOptionBoxAction ? KxMenuItem::kOptionBoxAction : KxMenuItem::kMainAction)) {
            return;
        }
    }

    QWidget::mouseReleaseEvent(event);

    if(triggerOptionBoxAction) {
        fMenuItem->optionBoxAction()->activate(QAction::Trigger);
    }
}

QStyleOptionMenuItem KxMenuItemWidget::getStyleOption() const
{
    QWidget *q = parentWidget();
    QMenu *menu = (QMenu *)q;
    KxMenuItem *action = fMenuItem;
    QStyleOptionMenuItem opt;

    opt.initFrom(this);
    opt.palette = q->palette();
    opt.state = QStyle::State_None;

    if (q->window()->isActiveWindow())
        opt.state |= QStyle::State_Active;
    if (q->isEnabled() && action->isEnabled()
        && (!action->menu() || action->menu()->isEnabled()))
        opt.state |= QStyle::State_Enabled;
    else
        opt.palette.setCurrentColorGroup(QPalette::Disabled);

    opt.font = action->font();

    if (menu->activeAction() == action && !action->isSeparator()) {
        opt.state |= QStyle::State_Selected;
        // 					 | (mouseDown ? QStyle::State_Sunken : QStyle::State_None);
    }

    if (!action->isCheckable()) {
        opt.checkType = QStyleOptionMenuItem::NotCheckable;
    } else {
        opt.checkType = (action->actionGroup() && action->actionGroup()->isExclusive())
            ? QStyleOptionMenuItem::Exclusive : QStyleOptionMenuItem::NonExclusive;
        opt.checked = action->isChecked();
    }
    if (action->menu())
        opt.menuItemType = QStyleOptionMenuItem::SubMenu;
    else if (action->isSeparator())
        opt.menuItemType = QStyleOptionMenuItem::Separator;
    else if (menu->defaultAction() == action)
        opt.menuItemType = QStyleOptionMenuItem::DefaultItem;
    else
        opt.menuItemType = QStyleOptionMenuItem::Normal;
    opt.icon = action->icon();
    opt.text = action->text();
    opt.menuRect = q->rect();
    return opt;
}

// ============================ Action ============================
KxMenuItem::KxMenuItem(QObject *parent)
: QWidgetAction(parent)
, fRadialPosition(kMMPositionNone)
, fOptionBoxAction(NULL)
, fIsRepeatable(true)
{
#if defined(OSMac_)
    connect(this, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool))); 
#endif
}

KxMenuItem::KxMenuItem(const QString &text, QObject *parent)
: QWidgetAction(parent)
, fRadialPosition(kMMPositionNone)
, fOptionBoxAction(NULL)
, fIsRepeatable(true)
{
    setText(text);

#if defined(OSMac_)
    connect(this, SIGNAL(toggled(bool)), this, SLOT(onToggled(bool))); 
#endif
}

void KxMenuItem::onToggled(bool /*checked*/)
{
#if defined(OSMac_)
    // keep the Mac platform-menuitem up to date
    QMenu *parentMenu = qobject_cast<QMenu*>(parent());
    if (parentMenu) {
        if(KxMacMainMenuBar::isMenuInMainMenuBar(parentMenu)) {
            KxMacMainMenuBar::syncMenuItem(parentMenu, this);
        }
    }
#endif
}

QWidget * KxMenuItem::createWidget(QWidget *parent)
{
    QWidget *newWidget = new KxMenuItemWidget(this, parent);	
    return newWidget;
}

void KxMenuItem::deleteWidget(QWidget *widget)
{
    // If this item has a submenu we need to clean it up.
    // The submenu is not parented to this action (but to the widget it
    // is associated with) because this is a QObject (QAction) and not a
    // QWidget. Delete the menu explicitly
    if (menu()) {
        // Do not delete menu if action is used by more than one widget.
        QList<QWidget *>list = this->associatedWidgets();
        if(list.count() < 2) {
            delete menu();
        }
    }
    delete widget;
}

bool KxMenuItem::saveToShelf(int whichAction)
{
    // First check that this is not a submenu item or separator
    if(menu() != NULL || isSeparator()) return false;

    // Only menu items from main menu bar are allowed to be saved to shelf.

    // Get topmost parent menu
    QMenu *menu = qobject_cast<QMenu *>(parent());
    QMenu *parentMenu = qobject_cast<QMenu *>(menu->parentWidget());
    while(parentMenu) {
        menu = parentMenu;
        parentMenu = qobject_cast<QMenu *>(menu->parentWidget());
    }

    // Now get the menu bar
    QMenuBar *mbar = qobject_cast<QMenuBar *>(menu->parentWidget());

    // Bail if parentWidget() is not a QMenuBar
    //
    if( mbar != NULL ){
        // Check if this is main menu bar
        QVariant v = qApp->property("mainWindow");
        if(v.isValid()) {
            QWidget *mainWindow = (QWidget *)(v.value<void *>());
            if(mbar->parentWidget() == mainWindow) {
                //QString menuName;
                //QString cmdStr(kShelfMenuItemCmd);
                QAction *a = whichAction == kMainAction ? this : this->optionBoxAction();
                if(NULL != a) {
                   // gUIInventory->fullName(a, menuName);
                    //cmdStr.append(menuName);
                   // cmdStr.appendChar(kBackSlashDoubleQuote);
                    //theCommandEngine->executeCommand( cmdStr );
                    return true;
                }
            }
        }
    }

    return false;
}

