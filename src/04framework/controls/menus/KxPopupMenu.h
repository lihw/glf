#ifndef KxPopupMenu_H_
#define KxPopupMenu_H_

#include "../base/HQtGui.h"

class KX_DLLEXPORT KxPopupMenu : public QMenu
{
    Q_OBJECT
public:
    KxPopupMenu(QWidget* parent = NULL);
    ~KxPopupMenu();

    void						setDisplayAsMarkingMenu(bool mode);
    bool						displayAsMarkingMenu() const;

    void						setPopupMenuButton(Qt::MouseButton button);
    Qt::MouseButton				popupMenuButton() const;

    void						setPopupMenuModifiers(Qt::KeyboardModifiers modifiers);
    Qt::KeyboardModifiers		popupMenuModifiers() const;

    static void					setSwapMetaKeys(bool swap);

private:
    bool						fDisplayAsMarkingMenu;
    Qt::MouseButton				fButton;
    Qt::KeyboardModifiers		fModifiers;

    // This is a global (i.e. static) setting that specifies that key
    // bindings for all popup menus be swapped. Currently we only support
    // swapping the Control and Alt keys: specify true to cause the
    // Control and Alt keys to be swapped, and false for the default
    // behaviour. In the future the boolean parameter could be expanded to
    // an enum to encode more complex hotkey swapping. 
    //
    static bool					fsSwapMetaKeys;
};

inline void		KxPopupMenu::setSwapMetaKeys(bool swap)
{
    fsSwapMetaKeys = swap;
}
#endif