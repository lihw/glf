#ifndef KxButtonGroup_H_
#define KxButtonGroup_H_

#include "../../base/HQtGui.h"
#include "../../base/KxIconnectableControl.h"

class KX_DLLEXPORT KxButtonGroup : public QButtonGroup, public KxIconnectableControl
{
    Q_OBJECT

public:
    KxButtonGroup(QObject* parent = 0);
    virtual ~KxButtonGroup() {}

    //  Override to force button 'id' to the Kiwi default of 0 rather than
    //  the Qt default of -1.
    void    addButton(QAbstractButton* button);
    void    addButton(QAbstractButton* button, int id);


    public slots:
        //  Set the enabled state for all the buttons within the group.
        void setEnabled(bool);

        //  Set the readOnly state for all the buttons within the group.
        void setReadOnly(bool);

        //  Handle a change in the group's "value", which is the id value of
        //  the currently checked button.
        void valueChanged(int newValue);

        //---------------------------------
        //    KxIconnectableControl Interface
        //
        public slots:
            //  Invoked by the entity at the other end of the connection whenever
            //  it has a new value for the control to display.
            void newValueFromConnection(const QVariant& value);

signals:
            //  Emitted by the control whenever the user changes its value, so that
            //  connected entities can immediately be updated.
            void newValueForConnections(
                const QVariant& newValue, bool isInterim
                );
            //
            //---------------------------------
};

inline void KxButtonGroup::addButton(QAbstractButton* button)
{  addButton(button, 0);                    }

inline void KxButtonGroup::addButton(QAbstractButton* button, int id)
{  QButtonGroup::addButton(button, id);    }


#endif