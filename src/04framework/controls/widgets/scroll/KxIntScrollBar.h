#ifndef KxIntScrollBar_H_
#define KxIntScrollBar_H_

#include "../../base/HQtGui.h"
#include "../slider/KxSliderFloatDataAbstraction.h"
#include "../../base/KxIconnectableControl.h"

class KX_DLLEXPORT KxIntScrollBar : public QScrollBar, public KxSliderDataAbstraction, public KxIconnectableControl
{
    Q_OBJECT

public:
    KxIntScrollBar(QWidget *parent = 0);

    private slots:
        void dragComplete();
        void flagUserChange(int action);
        void sendUserChangeToConnections(int newValue);

        //---------------------------------
        //    KxIconnectableControl Interface
        //
        public slots:
            //  Invoked by the entity at the other end of the connection whenever
            //  it has a new value for the control.
            virtual void newValueFromConnection(const QVariant& value);

signals:
            //  Emitted whenever the value changes while dragging the scroll bar.
            void transientSliderChange();

            //  Emitted by the control whenever the user changes its value, so that
            //  connected entities can immediately be updated.
            void newValueForConnections(
                const QVariant& newValue, bool isInterim
                );

            //---------------------------------

private:
    bool    fDragInProgress;
    bool    fIsUserChange;
    int     fOldValue;
};
#endif