#include "KxOptionMenu.h"
#include "KxMenuItem.h"


KxOptionMenu::KxOptionMenu(QWidget *parent)
: KxLabelledComboBox(parent)
, fMenu(NULL)
{
    fMenu = new QMenu(this);
    fMenu->setProperty("parentOptionMenu", qVariantFromValue((void *)this));
}

KxOptionMenu::~KxOptionMenu()
{
    delete fMenu;
    fMenu = NULL;
}

void KxOptionMenu::setItemEnabled(int index, bool enabled)
{
    QModelIndex itemIndex = model()->index(index, 0);
    if (!itemIndex.isValid())
        return;
    QStandardItemModel *itemModel = qobject_cast<QStandardItemModel *>(model());
    if (itemModel)
        itemModel->item(index)->setEnabled(enabled);
}

void KxOptionMenu::newValueFromConnection(const QVariant& newValue)
{
    int	newIntValue = newValue.value<int>();
    QList<QAction*> actions = fMenu->actions();

    //	Search the list of actions for one which has the exact same value.
    //	If we find it then set the comboBox to the corresponding item.
    int	itemIndex = 0;
    foreach(QAction* a, actions)
    {
        KxMenuItem* item = qobject_cast<KxMenuItem*>(a);

        if (item && (item->getValue().value<int>() == newIntValue))
        {
            setCurrentIndex(itemIndex);
            break;
        }

        ++itemIndex;
    }

    //	Even if we didn't find a match, we still want to record the value
    //	that the connection sent us. That way if the user re-selects the
    //	same item from the comboBox we will recognize it as a new value as
    //	far as connections are concerned and send it back out to them.
    fOldValue = newValue;
}


void KxOptionMenu::newValueFromUser(int newItemIndex)
{
    //	fMenu contains KxMenuItems for each of the items in the comboBox,
    //	and in the same order. So get the KxMenuItem for the comboBox
    //	item selected by the user and retrieve its value.
    QList<QAction*> actions = fMenu->actions();
    QVariant		newValue(0);

    if (newItemIndex < actions.size())
    {
        const KxMenuItem* item =
            qobject_cast<KxMenuItem*>(actions.at(newItemIndex));

        if (item) newValue = item->getValue();
    }

    //	If the new value is different from the old value, let our
    //	connections know about it.
    bool informConnections = (newValue != fOldValue);
    fOldValue = newValue;

    if (informConnections) emit newValueForConnections(newValue, false);
}
