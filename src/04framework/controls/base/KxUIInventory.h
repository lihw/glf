#ifndef KxUIInventory_H_
#define KxUIInventory_H_

const char kHierarchicalNameDivider = '|';
#include "HQtGui.h"

class KX_DLLEXPORT KxUIInventory : public QObject 
{
    Q_OBJECT
public:
    KxUIInventory();
    virtual ~KxUIInventory();

    void fullName(const QObject *, QString  &) const;
    
    bool isObjFullName(const QObject *, const QString &) const;
    bool isHierarchicalName(const QString &) const;
    QString validateName(const QString &) const;
    void getChildName( const QString &fullName, QString &childName) const;
    bool getParentName(const QString &fullName, QString &parentName) const;

    QObject * getParent(const QObject *) const;
    bool  isUniqueName(const QObject *obj);

    bool hasInternalName(const QObject *obj) const;
    QList<QObject*> getLayoutChildren(const QLayout *layout) const;
    QObject * findChild(const QObject *parent, const QString &childName, const char *className, bool report = true) const;
    QObject * findUnder(const QObject *parent, const QString &childName, const char *className, bool report = true) const;
    QObject * find(const QString &name, const char *className, bool report = true, bool considerMenus = false) const;
    QObject * findWindow(const QString &name, bool report = true) const;
    QObject * findWidget(const QString &name, bool report  = true) const;
    QObject * findLayout(const QString &name, const char *className = NULL, bool report = true) const;
    QObject * findLayoutUnder(const QObject *parent, const QString &name, const char *className = NULL, bool report = true) const;
    QAction * findMenuItem(const QString &name, bool report = true) const;
    QList<QObject*> findByType(const QString &name, const char *typeName, bool isUIType = false, bool singleObjSearch = false);


    bool isUITypeAlias(const QString &);
    void reportObjectNotFound(const QString &name) const;

    /// for debugging purposes only 
    void dumpObjectInfo(QString comment, const QObject *obj) const;


public:
    int  numWidgets() const;
    void dumpWidgets(QStringList &) const;
    static QList<QWidget *> & allWidgets();
    static QList<QWidget*>    allWindows();

private:

    bool  isObjFullNameHelper(const QObject *, const QString &) const;

    static QList<QWidget*>  sfWidgetList;
};

extern KX_DLLEXPORT KxUIInventory * gUIInventory;


#endif