#ifndef KxItemDelegate_H_
#define KxItemDelegate_H_
#include "../../base/HQtGui.h"
class KX_DLLEXPORT KxItemDelegate : public QItemDelegate
{
    Q_OBJECT
public:
    typedef QItemDelegate ParentClass;

    KxItemDelegate(QObject* parent = 0);
    virtual ~KxItemDelegate();

    virtual void setEditorData(QWidget *editor, const QModelIndex &) const;
    virtual void selectCurrent();

protected:
    virtual bool eventFilter(QObject* object, QEvent* event);

    QPointer<QObject> fLastEditor;

private:
    void doTextSelection(QObject* editor) const;
};
#endif