#ifndef KxCheckBox_H_
#define KxCheckBox_H_
#include "../../base/HQtGui.h"
#include "../../base/KxIconnectableControl.h"
#include "../../base/KxIcontextMenuRequestor.h"

class KX_DLLEXPORT KxCheckBox : public QCheckBox,
    public KxIconnectableControl,
    public KxIcontextMenuRequestor
{
    Q_OBJECT

public:
    KxCheckBox(const QString &text, QWidget *parent = 0);

    bool isReadOnly() const;

    public slots:
        void setReadOnly(bool readOnly);

protected:
    bool event(QEvent *e);
    virtual void mousePressEvent(QMouseEvent* event);

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


        //----------------------------------------------
        //    KxIcontextMenuRequestor Interface
        //
signals:
        //	Emitted by the control whenever the user requests a context menu
        //	while over the control.
        void	contextMenuRequested(QWidget* control, QMouseEvent* event);

        //    End of KxIcontextMenuRequestor Interface
        //----------------------------------------------

        private slots:
            void onOffChanged();
};
#endif