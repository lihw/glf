#ifndef KxIconTextCheckBox_H_
#define KxIconTextCheckBox_H_
#include "../../base/HQtGui.h"
#include "../../base/KxIconnectableControl.h"
#include "KxIconTextControl.h"

class KX_DLLEXPORT KxIconTextCheckBox : public QPushButton, public KxIconnectableControl
{
    Q_OBJECT
public:
    KxIconTextCheckBox(QWidget *parent=0);

    virtual QSize		sizeHint() const;

  /*  virtual void		setChangedCommand(Taction * action);
    Taction *			changedCommand() const;
    void				setOnCommand(Taction * action);
    Taction *			onCommand() const;
    void				setOffCommand(Taction * action);
    Taction *			offCommand() const;*/

    KxIconTextControl *	iconTextControl();

signals:
    void checkStateChangedToOn();
    void checkStateChangedToOff();

    //----------------------------------------------
    //    KxIconnectableControl Interface
    //
    public slots:
        //  Invoked by the entity at the other end of the connection whenever
        //  it has a new value for the control to display.
        virtual void newValueFromConnection(const QVariant& value);

signals:
        //  Emitted by the control whenever the user changes its value, so that
        //  connected entities can immediately be updated.
        void newValueForConnections(
            const QVariant& newValue, bool isInterim
            );
        //
        //----------------------------------------------

        private slots:
            void onOffChanged();

protected:
    void mouseReleaseEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);

private:
    KxIconTextControl *	fIconTextControl;
};

#endif