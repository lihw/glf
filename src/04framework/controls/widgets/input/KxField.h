#ifndef KxField_H_
#define KxField_H_

#include "../../base/HQtGui.h"
#include "../../base/KxIconnectableControl.h"
#include "../../base/KxIcontextMenuRequestor.h"
#include "../../drag/KxDropHandler.h"
#include "../../drag/KxDragHandler.h"
#include "../../drag/dragitem/KxPrimitiveDragItem.h"

class KX_DLLEXPORT KxField : public QLineEdit , 
                   public KxIconnectableControl,
                   public KxIcontextMenuRequestor,
                   public KxDragHandler
{
    Q_OBJECT
public:
     KxField(QWidget *parent = 0);
     
     void setAlwaysInvokeCmdOnReturn(bool status);
     bool alwaysInvokeEnterCmdOnReturn();
     void setPreferredSize(int width = -1, int height = -1);

     virtual void dragDone(QWidget *source, QMouseEvent *, Qt::DropAction action);
     virtual void getDragData(QWidget *source, QMouseEvent *, KxDragData *dragData);

     virtual QSize sizeHint()const;
#if defined (OSLinux_)
     virtual bool canPaste(const QMouseEvent *event)const;
     virtual void doPaste(const QMouseEvent *event, const QString &text);
#endif
     
signals:
     void changeCompleted();

public slots:
    virtual void handleReturnPressed();
    virtual void handleEditingFinished();
    virtual void handleSelectionChanged();
    virtual void setReadOnly(bool);
    virtual void setText(const QString &);

protected:
    virtual void makeValueInvalid(){}

public slots:
    virtual void newValueFromConnection(const QVariant &value);
signals:
    void newValueForConnections(const QVariant &newValue, bool isInterim);

public:
    //  This is not really part of the KxIconnectableControl interface. It
    //  is a helper method provided to make it easier for derived classes
    //  and other interested parties
    //  to provide connection data in formats which are more natural for
    //  them than text.
    //
    //  For example, a KxFloatField will typically be connected to
    //  attributes of type 'double' so KxFloatField would override this
    //  method to set 'value' as a double rather than text.
    virtual void getValueForConnections(QVariant& value) const;

signals:
    void contextMenuRequested(QWidget *control, QMouseEvent *event );

protected:
    virtual void focusInEvent(QFocusEvent *ev);
    virtual void focusOutEvent(QFocusEvent *ev);
    virtual void keyPressEvent(QKeyEvent *ev);
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void changeEvent(QEvent *ev);
    //	QLineEdit's built-in drop handling provides us with some nice
    //	features:
    //	
    //	- a cursor which shows where the dragged text will be inserted as
    //	  it is being dragged over the field, even when the field does not 
    //	  have the focus
    //	- automatic handling of the tricky business of moving text within a
    //	  field.
    //	- hiliting of the dropped text
    //
    //	For these reasons, we use QLineEdit's drop handling rather than a
    //	KxDropHandler. If for some reason you find that you need to add
    //	a KxDropHandler then you will need to ensure that your code
    //	provides at least the first two of the features listed above.
    //
    virtual void dropEvent(QDropEvent* event);

    //  Called whenever the field has a new value which must be passed on
    //  to connected entities.
    virtual void updateConnections(bool isInterim = false);

    QString     fOldText;


private:
    void updateFieldColor();

    bool fAlwaysInvokeEnterCmdOnReturn;
    bool fDoingDrag;
    bool fDragAndDropInSameField;
    bool fReturnKeyPressed;
    KxPrimitiveDragItem  fDragItem;
    QSize fPreferredSize;
    bool fUsePreferredSize;
};

inline void KxField::setAlwaysInvokeCmdOnReturn(bool status)
{
    fAlwaysInvokeEnterCmdOnReturn = status;
}

inline bool KxField::alwaysInvokeEnterCmdOnReturn(){
    return fAlwaysInvokeEnterCmdOnReturn;
}

#endif 