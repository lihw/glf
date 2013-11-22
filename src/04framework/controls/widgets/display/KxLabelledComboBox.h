#ifndef KxLabelledComboBox_H_
#define KxLabelledComboBox_H_

#include "../../base/HQtGui.h"
#include "../../base/KxIconnectableControl.h"
//	This class provides a QComboBox control which has a built-in label and
//	supports attribute connections.

class KX_DLLEXPORT KxLabelledComboBox : public QComboBox, public KxIconnectableControl
{
    Q_OBJECT
public:

    KxLabelledComboBox(QWidget* parent = NULL);
    ~KxLabelledComboBox();

    void		setLabel(QString text);
    QString		label() const;

    QSize		sizeHint() const;
    QSize		minimumSizeHint() const;
    QSize		minimumSize() const;

protected:
    virtual void paintEvent(QPaintEvent* event);
    virtual void wheelEvent(QWheelEvent* event);

    virtual void showPopup();
    virtual void hidePopup();
    virtual bool eventFilter(QObject *obj, QEvent *e);

    public slots:
        virtual void newValueFromUser(int newItemIndex);

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

protected:
    QVariant	fOldValue;

private:
    QLabel *	fLabel;
};


#endif