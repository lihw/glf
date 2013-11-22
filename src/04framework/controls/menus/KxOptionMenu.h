#ifndef KxOptionMenu_H_
#define KxOptionMenu_H_

#include "../base/HQtGui.h"

#include "../widgets/display/KxLabelledComboBox.h"

class KX_DLLEXPORT KxOptionMenu :	public KxLabelledComboBox
{
    Q_OBJECT
public:

    KxOptionMenu(QWidget* parent = NULL);
    ~KxOptionMenu();

    QMenu *		menu()			{ return fMenu; }
    void		setItemEnabled(int index, bool enabled);

    public slots:
        //	Override these two slots from KxLabelledComboBox so that we can
        //	use the QMenu as the source of values for combo box items rather
        //	than the combo box itself.
        virtual void newValueFromUser(int newItemIndex);
        virtual void newValueFromConnection(const QVariant& value);

private:
    QMenu *		fMenu;
};

#endif