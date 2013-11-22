#include "KxUIInventory.h"

#include "KxGlobalState.h"
#include "../widgets/button/KxButtonGroup.h"
#include "../menus/KxMenuItem.h"
#include "../menus/KxOptionMenu.h"


KxUIInventory theGlobalUIInventory;
KxUIInventory * gUIInventory = &theGlobalUIInventory;

KxUIInventory::KxUIInventory()
{

}

KxUIInventory::~KxUIInventory()
{

}

bool filterUnfiltered(const QObject *obj)
{
    return true;
}

bool filterWidgets(const QObject *obj)
{
    return obj->isWidgetType();
}

bool filterWindows(const QObject *obj)
{
    return obj->isWidgetType() && (static_cast<const QWidget *>(obj))->isWindow();
}

bool filterLayouts(const QObject *obj)
{
    return (dynamic_cast<const QLayout*>(obj)) != NULL;
}

bool filterWidgetsNoMenuBars(const QObject * obj)
{
    return obj->isWidgetType() && qobject_cast<const QMenuBar *>(obj) == NULL;
}


void KxUIInventory::fullName(const QObject *obj, QString &longName) const
{
    //
    //  Description:
    //		This method returns in  longName the name that uniquely describes
    //		the object.  For example a menuItem's long name would be of the 
    //		form:  
    //				window/menu/submenuItem/menu/menuItem
    //		Or a control's full name would be of the form: 
    //				window/controlLayout/controlLayout/control
    //
    //  Note:
    //		The window's menubar and default controlLayout are both
    //		ignored in building the object's full name as there is
    //		always a one to one mapping of these objects to their
    //		parent's names.
    QObject *parentObj = getParent(obj);
    QStack<QString> names;
    names.reserve(10);
    while(NULL != parentObj){
        names.push(parentObj->objectName());
        parentObj = getParent(parentObj);
    }

    longName.clear();
    const int sz = names.size();
    longName.reserve(10 * sz);
    for(int i=0; i < sz; ++i){
        longName.append(names.pop());
        longName.append(kHierarchicalNameDivider);
    }
    longName.append(obj->objectName());
}


bool KxUIInventory::isObjFullName(const QObject *obj, const QString & testName) const
{
    //  Description:
    //		This method compares the testName passed in and returns true if it
    //		matches the full name of the obj supplied. Full names are of the form:
    //		For menu items:
    //				window/menu/submenuItem/menu/menuItem
    //		Or for controls: 
    //				window/controlLayout/controlLayout/control
    bool matches = isObjFullNameHelper(obj,testName);
    if(!matches){
        // check against orginal name as well 
        QVariant v = obj->property("orginalName");
        if(v.isValid()){
            QString name = v.value<QString>();
            if(name == testName)
                matches = true;
        }
    }
    return matches;
}


bool  KxUIInventory::isObjFullNameHelper(const QObject *obj, const QString &testName) const
{
    // check if the testName is equal to the current object's name , if so 
    // we can leave quickly 
    QString currentName = obj->objectName();
    if(currentName.size() > 0 &&testName.endsWith(currentName)){
        const int testNameSz = testName.size();
        if(testNameSz == currentName.size()){
            return true;
        }

        // check that the next char up is the seperator character 
        // then loop up the parent chain , doing a backwards 
        // character-by-character check that the testName is the same 
        // as the parent name, all failure conditions result in early 
        // return from the function 
        int testNameIx = testNameSz - 1 - currentName.size();
        const QObject *nextObj = obj;
        while (testNameIx >= 0 ){
            if(testName.at(testNameIx--) != kHierarchicalNameDivider){
                return false;
            }
            nextObj = getParent(nextObj);
            if(NULL == nextObj)
                return false;

            currentName = nextObj->objectName();
            int sz = currentName.size();
            if(testNameIx - sz + 1 < 0)
                return false;
            for(int i=sz - 1; i >= 0; --i){
                if(currentName.at(i) != testName.at(testNameIx--))
                    return false;
            }

        }

    }else 
        return false;
    return true;
}


bool KxUIInventory::isHierarchicalName(const QString & name) const
{
    return name.indexOf(kHierarchicalNameDivider)  >= 0;
}

QString KxUIInventory::validateName(const QString & name) const
{
    int startIndex = 0;
    QString qName = name.trimmed();
    for(; startIndex < qName.length(); ++startIndex){
        if( qName[startIndex].isLetter() || qName[startIndex] == '_' || qName[startIndex] == ':')
            break;
    }
    if(startIndex > 0)
        qName.remove(0,startIndex);
     // process the inernal characters 
    for(int i=0; i < qName.length(); ++i){
        // if the name has a namespace specification but no name to go with the 
        // namespace , then return an empty string 
        if(qName[i] == ':' &&  qName.length() - 1){
            qName.clear();
            break;
        }else if(qName[i] == '#'  && i == qName.length() - 1){
            // if the name ends with # character , leave it ,as we 
            // will prelace it later with the appropriate index number 
            break;
        }else if(!qName[i].isLetterOrNumber() && qName[i] != '_' && qName[i] != ':') {
            qName.replace(i,1,'_');
        }        
    }
    return qName;
}

void KxUIInventory::getChildName(const QString &longName, QString &childName) const
{
    //  Description:
    //		This method returns in  childName the name that is the last part of its
    //		hierarchical name. For example for the following hierarchical name:
    //				window/menu/submenuItem/menu/menuItem
    //		The childName would be "menuItem"
    QString name;
    int lastDivider;
    int lastChar = longName.length();
    childName.clear();
    if(lastChar-- > 0){
        lastDivider = longName.indexOf(kHierarchicalNameDivider);
        if(lastDivider >=0)
            name = longName.mid(lastDivider);
        else
            name = longName;
    }
    childName = name;
}



bool KxUIInventory::getParentName(const QString &longName, QString &parentName) const
{
    //  Description:
    //		This method returns in parent name the object's full name without its
    //		child name.  For example for the following hierarchical name:
    //				window/menu/submenuItem/menu/menuItem
    //		The parentName would be "window/menu/submenuItem/menu".  If there
    //		is only one level in the hierarchical name then this method will
    //		return false.
    int lastDivider;
    bool hasParent = false;
    parentName.clear();
    if(longName.length()  > 0){
        lastDivider = longName.indexOf(kHierarchicalNameDivider);
        if(lastDivider >0){
            parentName = longName.left(longName.length() - lastDivider);
            hasParent = true;
        }
    }
    return hasParent;
}

QObject * KxUIInventory::getParent(const QObject * child) const
{
    //  Description:
    //		This method returns a pointer to the parent of the child object
    //		supplied.  If no parent is found then NULL is returned.
   Q_ASSERT(child != NULL);
   if(child->metaObject() == &QWidget::staticMetaObject && ((QWidget *)child)->isWindow())
       return NULL;
    QObject *parent = child->parent();
    if(parent && (parent->metaObject() == &QApplication::staticMetaObject))
        return NULL;

    if(NULL != dynamic_cast<const QLayout*>(child)){
        parent = ((QLayout*)child)->parentWidget();
        if(NULL != parent && parent->isWidgetType() && !((QWidget *)parent)->isWindow())
            parent = parent->parent();
    }else
        return parent;
    
    if(parent && (parent->metaObject() ==&KxOptionMenu::staticMetaObject )){
        return parent->parent();
    }
    if(parent && (parent->metaObject() == &QMenuBar::staticMetaObject)){
        return parent->parent();
    }
    if(parent && (qobject_cast<QDockWidget*>(parent)))
        return parent->parent();
    // skip inernal layout widgets 

    while(parent && hasInternalName(parent)){
        parent = parent->parent();
    }
    if(parent && parent->isWidgetType() && ((QWidget*)parent)->layout() && (NULL == dynamic_cast<QMainWindow*>(parent)) && !hasInternalName(((QWidget*)parent)->layout()))
        parent = ((QWidget*)parent)->layout();
    return parent;
}


bool KxUIInventory::isUniqueName(const QObject *obj)
{
    Q_ASSERT(obj);

    QString longName;
    bool isUnique = true;
    fullName(obj,longName);
    QWidgetList list = allWidgets();
    foreach(QWidget *widget, list){
        if(obj != widget && isObjFullName(widget,longName)){
            isUnique = false;
            break;
        }
    }
    return isUnique;
}

bool KxUIInventory::hasInternalName(const QObject *obj) const
{
          // Note: The objectName "qt_tabwidget_stackedwidget"
    //       is an internal name for a QStackedWidget automatically
    //       created by QTabWidget in its QTabWidgetPrivate::init()
    //       function.
    // 
    //       The objectName "qt_scrollarea_viewport" 
    //       is an internal name for a QWidget automatically
    //       created by QAbstractScrollArea in the
    //          QAbstractScrollAreaPrivate::init()
    //       function.
    //
/*    const QString objectName = obj->objectName();

    static const QString in1(kKiwiLayoutInternalWidgetName),
        in2("qt_scrollarea_viewport"),
        in3("qt_tabwidget_stackedwidget");

    if ( objectName == in1 ||
         objectName == in2 ||
         objectName == in3 ) {
        return true;
    }
    return false; */
    static const QRegExp rx(QString(kKxLayoutInternalWidgetName) + QString("|qt_scrollarea_viewport|qt_tabwidget_stackedwidget"));

    QString objectName = obj->objectName();
    if ( rx.exactMatch(objectName)) {
        return true;
    }
    return false;
}

QList<QObject*> KxUIInventory::getLayoutChildren(const QLayout *layout) const
{
    QList<QObject*> children;
    // There are some special cases that need to be handled here.
    // Several layouts are "simulated" using hierarchy of internal "helper" layouts and widgets
    // These classes will have a "getChildren()" method that returns QObjectList.
    // We don't call them directly because they can be in other slices that are not yet build

    //we call indexOfMethod and invokeMethod on "getChildren()" and "getChildren" directly to save on calling 
    //QMetaObject::normalizedSignature each time. If you change the function being called, make you change these 
    // to the name that would be returned by normalizedSignature.
    if(layout->metaObject()->indexOfMethod("getChildren()") != -1){
        bool success = QMetaObject::invokeMethod(const_cast<QLayout*>(layout),"getChildren",Qt::DirectConnection,Q_RETURN_ARG(QObjectList,children));
#if !defined(DEBUG)
        success = success;
#endif
        Q_ASSERT(success);
        return children;
    }
    // for all other layouts just skip objects with internal name 
    for(int index=0; index < layout->count(); ++index){
        QLayoutItem *item = layout->itemAt(index);
        if(item && item->widget() && !hasInternalName(item->widget()))
            children.append(item->widget());
    }
    return children;
}

QObject * KxUIInventory::findChild(const QObject *parent, const QString &childName, const char *className, bool report /* = true */) const
{
    //  Description:
    //		Looks for the immediate child of the parent layout with the specified
    //		name.  Returns NULL if none found.  Note that the childName given
    //		can be a short name or a heirarchical name.
    QObject *foundObj = NULL;
    QString qName;
    const QLayout *parentLayout = NULL;
    if(parent == NULL) return NULL;
    if(parent->isWidgetType()){
        if(!dynamic_cast<const QMainWindow*>(parent) && ((QWidget*)parent)->layout())
            parentLayout = ((QWidget *)parent)->layout();
    }else {
        parentLayout = dynamic_cast<const QLayout*>(parent);
    }

    if(parentLayout == NULL)
        return NULL;

    if(gUIInventory->isHierarchicalName(childName))
        getChildName(childName,qName);
    QList<QObject*> children = getLayoutChildren(parentLayout);
    foreach(QObject *child, children){
        if(qName == child->objectName() && (className == NULL || child->inherits(className))){
            foundObj = child;
            break;
        }
    }
    if(foundObj == NULL && report)
        reportObjectNotFound(childName);
    return foundObj;
}

QObject * KxUIInventory::findUnder(const QObject *parent, const QString &childName, const char *className, bool report /* = true */) const
{
    /*!
  Recursively find child, somewhere under parent (first checks if the parent is actually the child object itself).
  \param[in] potential parent of child to be found
  \param[in] childName should not be hierarchical
  \param[in] type name
  \param[in] report true means print failure message
  \return NULL if child is not found somewhere under parent.
  */
    if(parent == NULL) return NULL;
    QString qName = childName;
    QObject * foundObj = NULL;
    const QLayout *layout = NULL;
    if(NULL != (layout =qobject_cast<const QLayout*>(parent))){
        QObject *nonConstParent = layout->parentWidget();
        parent = nonConstParent;
        if(layout->objectName() == qName && (className == NULL || strcmp(layout->metaObject()->className() , className) == 0 ))  return nonConstParent;
    }else {
        if(parent->objectName() == qName && (className == NULL || strcmp(parent->metaObject()->className(), className) == 0)) return const_cast<QObject*>(parent); 
    }
    QList<QObject*> children = parent->findChildren<QObject*>(qName);
    foreach(QObject *obj, children){
        if(className == NULL || obj->inherits(className)){
            foundObj = obj;
            break;
        }
    }
    // For layouts return parent widget 
    if(foundObj && dynamic_cast<QLayout*>(foundObj))
        foundObj = ((QLayout*)foundObj)->parentWidget();

    if(foundObj == NULL && report) 
        reportObjectNotFound(childName);
    return foundObj;
}


QObject * KxUIInventory::find(const QString &name, const char *className, bool report /* = true */, bool considerMenus /* = false */) const
{
    //  Description:
    //		This method searches for an object of the specified name and class.
    //		If no objects are found it will return NULL.
    //
    //		'name' can either be a short name or an entire hierarchical name.
    //		if 'className' is NULL, then no type check will be done.
    QObject *  foundObj = NULL;
    QString childName;
    QString qName = name;
#ifdef USE_FINDWIDGETS
    // we prefer widgets, but might also match layouts.  If we get a layout
    // we return it's parent widget
    //foundObj = TUIfindUtil::findNamedObject(qName, &filterWidgets, className);
    //QLayout *lay = qobject_cast<QLayout*>(foundObj);
    //if (lay) {
    //    if(className && (!strcmp("QActionGroup", className) || !strcmp("KxButtonGroup", className))) 
    //    {
    //        // special cases of wanting to find non-widgets
    //        // QT_TODO: get rid of them
    //    } 
    //    else 
    //    {
    //        foundObj = lay->parentWidget();
    //    }
    //}
#else
     bool isLongName = isHierarchicalName(name);
     // if short name is provided , search under caurrent layout first 
     if(!isLongName){
         QLayout *currLayout = NULL;
         QWidget *currWindow = NULL;
         if(NULL != (currLayout = KxGlobalState::currentQLayout())){
             foundObj = gUIInventory->findUnder(currLayout,name,className,false);
             if(foundObj != NULL) return foundObj;
         }else if(NULL != (currWindow = KxGlobalState::currentQWindow())){
             foundObj = gUIInventory->findUnder(currWindow,name,className,false);
             if(NULL != foundObj) return foundObj;
         }
     }
     if(className && !strcmp("QActionGroup",className)){
         if(isLongName){
             getChildName(name,childName);
             qName = childName;
         }
         QWidgetList list = allWidgets();
         foreach(QObject *obj, list){
             foundObj = obj->findChild<QActionGroup*>(qName);
             if(foundObj) return foundObj;
         }
         // still not found , try application object it owns global action groups 
         if( NULL == foundObj){
             foundObj = qApp->findChild<QActionGroup*>(qName);
             return foundObj;
         }
     }
     if(className && !strcmp("KxButtonGroup", className)) {
         if(isLongName) {
             getChildName(name, childName);
             qName = childName;
         }

         QWidgetList list = allWidgets();
         foreach( QObject *obj, list) {
             foundObj = obj->findChild<KxButtonGroup *>(qName);
             if(foundObj) return foundObj;
         }
         // Still not found, try application object - it owns global action groups
         if(NULL == foundObj) {
             foundObj = qApp->findChild<KxButtonGroup *>(qName);
             return foundObj;
         }

     }

     QWidgetList list = allWidgets();
     foreach( QObject *obj, list) {
         // Check object first
         if(className == NULL || strcmp(obj->metaObject()->className(), className) == 0) {
             if (isLongName) {
                 if( isObjFullName(obj, qName)) {
                     foundObj = obj;
                     break;
                 }
             } else {
                 if( obj->objectName() == qName) {
                     // Avoid ambiguity between KxOptionMenu's and their QMenu's when doing
                     // a class-free short-name lookup.  Always return the KxOptionMenu
                     if (NULL != className || obj->property("objectTypeUI").isValid()) {
                         foundObj = obj;
                         break;
                     }
                 }
             }
         }

         // Check menus if applicable
         if (considerMenus) {
             QMenu *menu = dynamic_cast<QMenu *>(obj);
             if(menu) {
                 QString qParentName;
                 getParentName(name, qParentName);
                 if(qParentName.isEmpty() || isObjFullName(menu, qParentName)) {
                     getChildName(name, childName);
                     QList<QAction *>actions = menu->actions();
                     foreach(QAction *a, actions) {
                         if(a->objectName() == childName) {
                             foundObj = a;
                             break;
                         }
                     }
                     if(NULL != foundObj) break;
                 }
             }
         }
     }

     if (NULL == foundObj)
     {
         foreach( QObject *obj, list)
         {
             QLayout *layout = ((QWidget *)obj)->layout();
             // Check layout, if there is one
             if(layout && (className == NULL || strcmp(layout->metaObject()->className(), className) == 0)) {
                 if (isLongName) {
                     if( isObjFullName(layout, qName)) {
                         foundObj = obj;
                         break;
                     }
                 } else {
                     if( layout->objectName() == qName) {
                         foundObj = obj;
                         break;
                     }
                 }
             }
         }
     }

     // If Qt objects search failed, resort to old methods in order to find "bridge" controls
#endif
     if (foundObj == NULL && report) {
         reportObjectNotFound(name);
     }

     return foundObj;
}

QObject* KxUIInventory::findWindow(const QString& name, bool report) const
// There is no QWindow class in Qt framework. Window is just a parentless widget.
{
    QObject*				foundObj = NULL;
    // Check current window first
    QWidget* currWindow = NULL;
    if (NULL != (currWindow = KxGlobalState::currentQWindow())) {
        if(currWindow->objectName() == name) foundObj = currWindow;
        if(foundObj != NULL) return foundObj;
    }
#ifdef USE_FINDWIDGETS
   // QObject* obj = TUIfindUtil::findNamedObject(qName, &filterWindows, NULL);
    // double check the result, we really only want windows
    //if (obj && filterWindows(obj))
        //foundObj = obj;
#else
    QWidgetList list = allWindows();
    foreach( QObject *obj, list) {
        if( obj->objectName() == name) {
            foundObj = obj;
            break;
        }
    }
#endif	
    if (foundObj == NULL && report) {
        reportObjectNotFound(name);
    }

    return foundObj;
}

QObject* KxUIInventory::findWidget(const QString& name, bool report) const
// This method will find a widget by specified name.
{
    QObject*				foundObj = NULL;

#ifdef USE_FINDWIDGETS
   // QObject* obj = TUIfindUtil::findNamedObject(qName, &filterWidgetsNoMenuBars, NULL);
    // double check the result, we only want widgets
   // if (obj && obj->isWidgetType())
      //  foundObj = obj;
#else

    bool				isLongName = isHierarchicalName(name);
    // If short name is provided, search under current layout first
    if(!isLongName) {
        QLayout* currLayout = NULL;
        QWidget* currWindow = NULL;
        if (NULL != (currLayout = KxGlobalState::currentQLayout())) {
            foundObj = gUIInventory->findUnder(currLayout, name, NULL, false);
            if(foundObj != NULL) return foundObj;
        } else if (NULL != (currWindow = KxGlobalState::currentQWindow())) {
            foundObj = gUIInventory->findUnder(currWindow, name, NULL, false);
            if(foundObj != NULL) return foundObj;
        }
    }

    QWidgetList list = allWidgets();
    foreach( QObject *obj, list) {
        if(obj->isWidgetType() && (NULL == dynamic_cast<QMenuBar*>(obj))) {
            if(isLongName) {
                if(isObjFullName(obj, name)) {
                    foundObj = obj;
                    break;
                }
            } else {
                if(obj->objectName() == name) {
                    foundObj = obj;
                    break;
                }
            }
        } 
    }
#endif
    if (foundObj == NULL && report) {
        reportObjectNotFound(name);
    }

    return foundObj;
}


QObject* KxUIInventory::findLayout(const QString& name, const char *className, bool report) const
// This method will find a widget with layout by specified name
{
    QObject*				foundObj = NULL;

#ifdef USE_FINDWIDGETS
   // QObject* obj = TUIfindUtil::findNamedObject(qName, &filterLayouts, className);
    // double check the result, we only want layouts
    //if (obj && qobject_cast<QLayout*>(obj))
        //foundObj = obj->parent();
#else
    bool				isLongName = isHierarchicalName(name);

    // If short name is provided, search under current layout first
    if(!isLongName) {
        QLayout* currLayout = NULL;
        QWidget* currWindow = NULL;
        if (NULL != (currLayout = KxGlobalState::currentQLayout())) {
            foundObj = gUIInventory->findLayoutUnder(currLayout, name, className, false);
            if(foundObj != NULL) return foundObj;
        } else if (NULL != (currWindow = KxGlobalState::currentQWindow())) {
            foundObj = gUIInventory->findLayoutUnder(currWindow, name, className, false);
            if(foundObj != NULL) return foundObj;
        }
    }

    QWidgetList list = allWidgets();
    foreach( QObject *obj, list) {
        if(((QWidget *)obj)->layout() && (className == NULL || strcmp(((QWidget *)obj)->layout()->metaObject()->className(), className) == 0)) {
            if(isLongName) {
                if(isObjFullName(((QWidget *)obj)->layout(), name)) {
                    foundObj = obj;
                    break;
                }
            } else {
                if(((QWidget *)obj)->layout()->objectName() == name) {
                    foundObj = obj;
                    break;
                }
            }
        }
    }
#endif
    if (foundObj == NULL && report) {
        reportObjectNotFound(name);
    }

    return foundObj;
}

QObject* KxUIInventory::findLayoutUnder(const QObject *parent, const QString& name, const char *className, bool report) const
//	This method will find a widget with layout by specified name under the
//	specified parent (first checks if the parent is actually the widget or the layout itself).
{
    if(parent == NULL) return NULL;

    QObject *		foundObj = NULL;
    bool 		isLongName = isHierarchicalName(name);
    QString			shortName;

    if (isLongName) {
        getChildName(name, shortName);
    } else {
        shortName = name;
    }

    const QLayout *layout = NULL;
    if(NULL != (layout = qobject_cast<const QLayout *>(parent))) {
        QObject* nonConstParent = layout->parentWidget();
        parent = nonConstParent;
        if(layout->objectName() == name && (className == NULL || strcmp(layout->metaObject()->className(), className) == 0)) return nonConstParent;
    } else {
        if(parent->isWidgetType()) {
            const QWidget *widget = qobject_cast<const QWidget *>(parent);
            if(widget && widget->layout() && widget->layout()->objectName() == name && (className == NULL || strcmp(widget->layout()->metaObject()->className(), className) == 0)) {
                return const_cast<QObject*>(parent);
            }
        }
    }

    QList<QObject*>children = parent->findChildren<QObject *>(shortName);
    foreach( QObject *obj, children) {
        if(qobject_cast<QLayout*>(obj) && (className == NULL || strcmp(obj->metaObject()->className(), className) == 0)) {
            foundObj = ((QLayout *)obj)->parentWidget();
            break;
        } else if(obj->isWidgetType() && ((QWidget *)obj)->layout() && (className == NULL || strcmp(((QWidget *)obj)->layout()->metaObject()->className(), className) == 0)) {
            if(isLongName) {
                if(isObjFullName(((QWidget *)obj)->layout(), name)) {
                    foundObj = obj;
                    break;
                }
            } else {
                if(((QWidget *)obj)->layout()->objectName() == name) {
                    foundObj = obj;
                    break;
                }
            }
        }
    }

    if (foundObj == NULL && report) {
        reportObjectNotFound(name);
    }

    return foundObj;
}

QAction* KxUIInventory::findMenuItem(const QString& name, bool report) const
{
    QAction *foundObj = NULL;

    QString parentName;
    QString shortName;
    gUIInventory->getParentName(name, parentName);
    gUIInventory->getChildName(name, shortName);

    QString actionName = shortName;
    QWidgetList list = allWidgets();
    foreach( QObject *obj, list) {
        QMenu *menu = dynamic_cast<QMenu *>(obj);
        if(menu) {
            if(!parentName.isEmpty() && !isObjFullName(menu, parentName)) {
                continue;
            }
            QList<QAction *>actions = menu->actions();
            foreach(QAction *a, actions) {
                if(a->objectName() == actionName) {
                    foundObj = a;
                    break;
                }
            }
            if(NULL != foundObj) break;
        }
    }

    if (foundObj == NULL && report) {
        reportObjectNotFound(name);
    }

    return foundObj;
}       


static bool checkUIType(const QObject *obj, const char *type)
{
    if(!obj || !type) return false;

    QVariant v = obj->property("objectTypeUI");
    if(v.isValid()) {
        QString objType = v.value<QString>();

        if(objType == type) {
            return true;
        }
        // handle aliases
        if(!strcmp(type, "window") && objType == "floatingWindow") {
            return true;
        }
    }

    // Check against "generic" type (i.e. control or layout)
    if(!strcmp(type, "control")) {
        v = obj->property("isControl");
        if(v.isValid()) return true;
    }

    if(!strcmp(type, "layout")) {
        v = obj->property("isLayout");
        if(v.isValid()) return true;
    }

    return false;
}


QList<QObject *> KxUIInventory::findByType(const QString& name,
                                          const char *typeName,
                                          bool isUIType /* = false*/,
                                          bool singleObjSearch /* = false*/)
                                          //
                                          //  Description:
                                          //		This method searches for an objects of the specified name and type.
                                          //		First argument (name) can be empty, second (typeName) is mandatory.
                                          //
                                          //		If isUIType is true, then typeName is the object UI type as returned by
                                          //		objectTypeUI command. (e.g. "button", "floatingWindow", "columnLayout", etc.)
                                          //
                                          //		If isUIType is false (default) then typeName is the class name
                                          //		(e.g. QPushButton, KxCheckBox, KxColumnLayout, etc.)
                                          //
                                          //		If singleObjSearch is true, only a single object is returned
                                          //
{
    QList<QObject*>     results;

    Q_ASSERT(typeName); // Required argument

    QString childName;
    bool isLongName = (name.isEmpty() ? false : isHierarchicalName(name));
    bool done = false;               

    QString qName = name.isEmpty() ? "" : name;

    if((isUIType && !strcmp("radioMenuItemCluster", typeName))
        || (!isUIType && !strcmp("QActionGroup", typeName))) {
            if(isLongName) {
                getChildName(name, childName);
                qName = childName;
            }

            QList<QActionGroup *> foundObjects;
            QWidgetList list = allWidgets();
            foreach( QObject *obj, list) {
                foundObjects = obj->findChildren<QActionGroup *>();
                foreach(QActionGroup *action, foundObjects) {
                    if(!name.isEmpty()) {
                        if( action->objectName() == qName) {
                            results << action;
                            if(singleObjSearch) break; 
                            else continue;
                        }
                    } else {
                        results << action;
                        if(singleObjSearch) break; 
                        else continue;
                    }
                }
            }
            // Also, try application object - it owns global action groups
            foundObjects = qApp->findChildren<QActionGroup *>();
            foreach(QActionGroup *action, foundObjects) {
                if(!name.isEmpty()) {
                    if( action->objectName() == qName) {
                        results << action;
                        if(singleObjSearch) break; 
                        else continue;
                    }
                } else {
                    results << action;
                    if(singleObjSearch) break; 
                    else continue;
                }
            }
            done = true;
    }
    if (!done)
    {
        if((isUIType && (strstr(typeName, "Cluster") || !strcmp(typeName, "collection")))
            || (!isUIType && !strcmp("KxButtonGroup", typeName))) {
                if(isLongName) {
                    getChildName(name, childName);
                    qName = childName;
                }

                QList<KxButtonGroup *> foundObjects;
                QWidgetList list = allWidgets();
                foreach( QObject *obj, list) {
                    foundObjects = obj->findChildren<KxButtonGroup *>();
                    foreach(KxButtonGroup *grp, foundObjects) {
                        if(name.isEmpty() || grp->objectName() == qName) {
                            results << grp;
                            if(singleObjSearch) break; 
                            else continue;
                        }
                    }
                }
                // Also, try application object - it owns global action groups
                foundObjects = qApp->findChildren<KxButtonGroup *>();
                foreach(KxButtonGroup *grp, foundObjects) {
                    if(name.isEmpty() || grp->objectName() == qName) {
                        results << grp;
                        if(singleObjSearch) break; 
                        else continue;
                    }
                }
                done = true;
        }
    }

    if (!done)
    {
        if((isUIType && !strcmp("commandMenuItem", typeName))
            || (!isUIType && !strcmp("KxMenuItem", typeName))) {
                if(isLongName) {
                    getChildName(name, childName);
                    qName = childName;
                }

                QList<KxMenuItem *> foundObjects;
                QWidgetList list = allWidgets();
                foreach( QObject *obj, list) {
                    foundObjects = obj->findChildren<KxMenuItem*>();
                    foreach(QAction *action, foundObjects) {
                        if(!name.isEmpty()) {
                            if( action->objectName() == qName) {
                                results << action;
                                if(singleObjSearch) break; 
                                else continue;
                            }
                        } else {
                            results << action;
                            if(singleObjSearch) break; 
                            else continue;
                        }
                    }
                }
                done = true;
        }
    }
    if (!done)
    {
        QWidgetList list = allWidgets();
        foreach( QObject *obj, list) {
            QLayout *layout = NULL;
            if(obj->isWidgetType()) {
                layout = ((QWidget *)obj)->layout();
            }
            // Check object first
            if((isUIType && checkUIType(obj, typeName))
                || (!isUIType && strcmp(obj->metaObject()->className(), typeName) == 0)) {
                    if(!name.isEmpty()) {
                        if (isLongName) {
                            if( isObjFullName(obj, qName)) {
                                results << obj;
                                if(singleObjSearch) break; 
                                else continue;
                            }
                        } else {
                            if( obj->objectName() == qName) {
                                results << obj;
                                if(singleObjSearch) break; 
                                else continue;
                            }
                        }
                    } else {
                        results << obj;
                        if(singleObjSearch) break; 
                        else continue;
                    }
            }
            // Check layout, if there is one
            if(layout && ((isUIType && checkUIType(layout, typeName))
                || (!isUIType && strcmp(layout->metaObject()->className(), typeName) == 0))) {
                    if(!name.isEmpty()) {
                        if (isLongName) {
                            if( isObjFullName(layout, qName)) {
                                results << layout;
                                if(singleObjSearch) break; 
                                else continue;
                            }
                        } else {
                            if( layout->objectName() == qName) {
                                results << layout;
                                if(singleObjSearch) break; 
                                else continue;
                            }
                        }
                    } else {
                        results << layout;
                        if(singleObjSearch) break; 
                        else continue;
                    }
            }
        }
        done = true;
    }
    // The use of recursive find* methods on all widgets means we may have duplicates
    // so we eliminate them first
    if (results.length() > 1) {
        results = results.toSet().toList();
    }
    return results;
}

bool KxUIInventory::isUITypeAlias(const QString &typeName)
//
//  Description:
//		This method returns whether a given type name is an alias for
//		one or more "real" UI types.
//
{
    return typeName == "window"
        || typeName == "control"
        || typeName == "collection"
        || typeName == "layout"
        ;
}

void KxUIInventory::reportObjectNotFound(const QString &name) const
{
    // Report an error.
    //
    // FIXME: should print a command name
    // should called print 
}


void KxUIInventory::dumpObjectInfo(QString comment, const QObject *obj) const
{
    if(obj == NULL) {
        printf("NULL object passed into KxUIInventory::dumpObjectInfo()\n");
        return;
    }

    QString name, layoutName("no layout");
    name = obj->objectName();
    if(obj->isWidgetType() && ((QWidget*)obj)->layout()) {
        layoutName = ((QWidget*)obj)->layout()->objectName();
    }
    //printf("%s - Class: %s, Name: %s, Layout: %s\n",
      //  comment.toAscii(), obj->metaObject()->className(), name.toAscii(), layoutName.toAscii());
}

QList<QWidget *> KxUIInventory::sfWidgetList;

int KxUIInventory::numWidgets() const
{
    return( allWidgets().length() );
}

void KxUIInventory::dumpWidgets( QStringList& tList ) const
{
    QWidgetList wList = allWidgets();
    foreach( QObject *obj, wList) {
        QString tName;
        fullName( obj, tName );
        tList.append(tName);
    }
}

/*static*/
QList<QWidget *> & KxUIInventory::allWidgets()
{
    static int count = 0;

    // ...update widget list 	  	 
    sfWidgetList = QApplication::allWidgets(); 	  	

    return sfWidgetList;
}

/*static*/
QList<QWidget *> KxUIInventory::allWindows()
{
    QList<QWidget *> windowList;

    foreach (QWidget *widget, QApplication::topLevelWidgets()) {
        if(widget->isWindow() && !qobject_cast<QMenu *>(widget)) {
            windowList.append(widget);
        }
    }

    return windowList;
}
