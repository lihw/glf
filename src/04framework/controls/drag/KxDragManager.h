#ifndef KxDragManager_H_
#define KxDragManager_H_

#include "../base/HQtGui.h"
#include "KxDragItemDataType.h"

class KxDragHandler;
class KxDropHandler;

class KX_DLLEXPORT KxDragManager : public QObject
{
    Q_OBJECT
public:
    KxDragManager(QWidget *parent);
    virtual ~KxDragManager();

    static bool addDragHandler(QObject *target, KxDragHandler *handler, bool requiresDeletion = false);

    static void removeDragHandler(QObject *target, KxDragHandler *handler);

    static QList<KxDragHandler*> * getDragHandlers(const QObject *target);

    static bool addDropHandler(QObject *target, KxDropHandler *handler, bool requiresDeletion = false);

    static void removeDropHandler(QObject *target, KxDropHandler *handler);
    static void removeAllDropHandlers(QObject *target);
    static QList<KxDropHandler*>* getDropHandlers(const QObject *target);

protected:
    template <class T>
    class HandlerEntry 
    {
    public:
        T * handler;
        bool requiresDeletion;
    };

    template <class T>
    class HandlerList :  public QList<HandlerEntry<T> >
    {
    public:
        typedef QList<HandlerEntry<T> > parentClass;
        void append(T *handler, bool requiresDeletion)
        {
              HandlerEntry<T> entry;
              entry.handler = handler;
              entry.requiresDeletion = requiresDeletion;
              parentClass::append(entry);
        }

        bool remove(T *handler)
        {
            bool requiresDeletion = false;
            for(int i=0;i < parentClass::length(); ++i){
                if(parentClass::value(i).handler == handler){
                    requiresDeletion = parentClass::value(i).requiresDeletion;
                    parentClass::removeAt(i);
                    break;
                }
            }
            return requiresDeletion;
        }

        QList<T*> * getHandlers(){
            QList<T*> * handlers = new QList<T*>;
            for(int i=0;i < parentClass::length(); ++i){
                handlers->append(parentClass::value(i).handler);
            }
            return handlers;
        }
    };

    void addDragHandler(KxDragHandler * handler, bool requiresDeletion = false);
    void removeDragHandler(KxDragHandler *handler);

    void addDropHandler(KxDropHandler *handler, bool requiresDeletion = false);
    void removeDropHandler(KxDropHandler *handler);

    bool checkDropTypes(QDragMoveEvent *ev);
    bool eventFilter(QObject *obj, QEvent *ev);
    bool handleDrop(QDropEvent *ev);
    void handleLeave(QDragLeaveEvent *ev);
    bool handleMousePress(QMouseEvent *ev);

    static KxDragManager * getDragManager(QObject *target, bool create);
    static QWidget * getRealTarget(QObject *target);
    static KxDragItemDataType getDragType(QDropEvent *ev);

    Qt::MouseButtons  fDragButtons;
    HandlerList<KxDragHandler> fDragHandlers;
    HandlerList<KxDropHandler> fDropHandlers;
    bool                          fOrigDropState;
    bool                          fPositionMatters;
    QPointer<QWidget>             fTarget;
    QPointer<QRubberBand>         fRubberBand;
#if defined(OSLinux_)
    bool                          fMousePressWasPaste;
#endif
};


#endif
