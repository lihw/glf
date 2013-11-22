#include "KxPopupMenu.h"

#include "../base/KxQtHelper.h"
#include "../utils/KxBasicCursors.h"

bool	KxPopupMenu::fsSwapMetaKeys = false;	// Swap the CTRL & ALT keys.

KxPopupMenu::KxPopupMenu(QWidget *parent)
: QMenu(parent)
, fDisplayAsMarkingMenu(false)
, fButton(Qt::RightButton)
, fModifiers(Qt::NoModifier)
{
    // Install event filter on the object, so popup menu could be handled
    if(parent) {
        KxQtHelper::installPopupEventFilter(parent);
        parent->setCursor(kRMBMenuCursor.getCursor());
    }
}

KxPopupMenu::~KxPopupMenu()
{
}

void KxPopupMenu::setDisplayAsMarkingMenu(bool mode)
{
    fDisplayAsMarkingMenu = mode;
}

bool KxPopupMenu::displayAsMarkingMenu() const
{
    return fDisplayAsMarkingMenu;
}

void KxPopupMenu::setPopupMenuButton(Qt::MouseButton button)
{
    fButton = button;
}

Qt::MouseButton KxPopupMenu::popupMenuButton() const
{
    return fButton;
}

void KxPopupMenu::setPopupMenuModifiers(Qt::KeyboardModifiers modifiers)
{
    fModifiers = modifiers;
}

Qt::KeyboardModifiers KxPopupMenu::popupMenuModifiers() const
{
    Qt::KeyboardModifiers mods = fModifiers;

    // Swap the CTRL & ALT keys if requested on the fly.
    //
    if ( fsSwapMetaKeys ) {
        bool isControl = mods.testFlag( Qt::ControlModifier );
        bool isAlt = mods.testFlag( Qt::AltModifier );

        if ( isControl ) {
            mods &= ~Qt::ControlModifier;
        }
        if ( isAlt ) {
            mods &= ~Qt::AltModifier;
        }

        if ( isControl ) {
            mods |= Qt::AltModifier;
        }
        if ( isAlt ) {
            mods |= Qt::ControlModifier;
        }
    }

    return mods;
}
