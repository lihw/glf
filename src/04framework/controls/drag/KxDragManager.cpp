#include "KxDragManager.h"

#include "KxDragHandler.h"
#include "KxDropHandler.h"
#include "dragitem/KxDragItem.h"
#include "../base/KxQtHelper.h"

static const char * kDragManagerProp = "dragManager";

KxDragManager::KxDragManager(QWidget *target)
:QObject(target)
, fDragButtons(0)
, fTarget(target)
#if defined(OSLinux_)
, fMousePressWasPaste(false)
#endif
{
    fOrigDropState = target->acceptDrops();
    fRubberBand = new QRubberBand(QRubberBand::Rectangle,target);
}

KxDragManager::~KxDragManager()
{
    if(!fDragHandlers.isEmpty()){
        QList<KxDragHandler*> *dragHandlers = fDragHandlers.getHandlers();
        for(int i=0; i < dragHandlers->size(); ++i){
            if(fDragHandlers.remove((*dragHandlers)[i])){
                delete (*dragHandlers)[i];
            }
        }
    }

    if(!fDropHandlers.isEmpty()) {
        QList<KxDropHandler*> *dropHandlers = fDropHandlers.getHandlers();
        for(int i=0; i < dropHandlers->size(); ++i){
            if(fDropHandlers.remove((*dropHandlers)[i])){
                delete (*dropHandlers)[i];
            }
        }
    }
    if(fTarget){
        fTarget->removeEventFilter(this);
        QVariant empty;
        fTarget->setProperty(kDragManagerProp,empty);
        fTarget->setAcceptDrops(fOrigDropState);
    }
}

void KxDragManager::addDragHandler(KxDragHandler * handler, bool requiresDeletion /* = false */)
{
    if(handler && fTarget){
        bool filterActive = (fDragHandlers.count() || fDropHandlers.count());
        fDragHandlers.remove(handler);
        fDragHandlers.append(handler,requiresDeletion);
        fDragButtons |= handler->dragButtons();
        if(!filterActive){
            fTarget->installEventFilter(this);
        }
    }
}

bool  KxDragManager::addDragHandler(QObject *target, KxDragHandler *handler, bool requiresDeletion /* = false */)
{
    KxDragManager *dragMgr = getDragManager(target,true);
    if(dragMgr){
        dragMgr->addDragHandler(handler,requiresDeletion);
        return true;
    }
    return false;
}

void KxDragManager::removeDragHandler(KxDragHandler *handler)
{
    if(fDragHandlers.remove(handler)){
        delete handler;
    }
    if(fDragHandlers.isEmpty() && fDropHandlers.isEmpty()){
        delete this;
    }else{
        //Since we're sticking around, rebuild the drag button mask.
        QListIterator<HandlerEntry<KxDragHandler> > iter(fDragHandlers);
        fDragButtons = 0;
        while(iter.hasNext()){
            fDragButtons |= iter.next().handler->dragButtons();
        }
    }
}

void KxDragManager::removeDragHandler(QObject *target, KxDragHandler *handler)
{
    KxDragManager *dragMgr = getDragManager(target,false);
    if(dragMgr){
        dragMgr->removeDragHandler(handler);
    }
}

QList<KxDragHandler*> * KxDragManager::getDragHandlers(const QObject *target)
{
    KxDragManager *dragMgr = getDragManager(const_cast<QObject*>(target),false);
    if(dragMgr){
        return dragMgr->fDragHandlers.getHandlers();
    }

    return NULL;
}

void KxDragManager::addDropHandler(KxDropHandler *handler, bool requiresDeletion /* = false */)
{
    if(handler && fTarget){
        bool filterActive = (fDragHandlers.count() || fDropHandlers.count());
        fDropHandlers.remove(handler);
        fDropHandlers.append(handler,requiresDeletion);
        if(!filterActive){
            fTarget->installEventFilter(this);
        }
        fTarget->setAcceptDrops(true);
    }
}

bool  KxDragManager::addDropHandler(QObject *target, KxDropHandler *handler, bool requiresDeletion /* = false */)
{
    KxDragManager *dragMgr = getDragManager(target,true);
    if(dragMgr){
        dragMgr->addDropHandler(handler,requiresDeletion);
        return true;
    }
    return false;
}

void KxDragManager::removeDropHandler(KxDropHandler *handler)
{
    if(fDropHandlers.remove(handler)){
        delete handler;
    }

    if(fDragHandlers.isEmpty() && fDropHandlers.isEmpty()){
        delete this;
    }
}

void KxDragManager::removeDropHandler(QObject *target, KxDropHandler *handler)
{
    KxDragManager *dragMgr = getDragManager(target,false);
    if(dragMgr){
        dragMgr->removeDropHandler(handler);
    }
}

void KxDragManager::removeAllDropHandlers(QObject *target)
{
     const QList<KxDropHandler*> * dropHandlers = getDropHandlers(target);
     if(dropHandlers != NULL){
         int numHandlers = dropHandlers->length();
         for(int i=0; i < numHandlers; ++i){
             removeDropHandler(target,dropHandlers->first());
         }
     }
}

QList<KxDropHandler*> * KxDragManager::getDropHandlers(const QObject *target)
{
    KxDragManager * dragMgr = getDragManager(const_cast<QObject*>(target),false);
    if(dragMgr){
        return dragMgr->fDropHandlers.getHandlers();
    }
    return NULL;
}

KxDragManager * KxDragManager::getDragManager(QObject *target, bool create)
{
    // get the arget;s drag manager from a property 
    KxDragManager *dragMgr = NULL;
    QWidget *targetWidget = getRealTarget(target);
    if(targetWidget ){
        QVariant v = targetWidget->property(kDragManagerProp);
        if(v.isValid()){
            dragMgr = (KxDragManager *)v.value<void*>();
        }
        // if there is no drag manager and create is set create one 
        if((dragMgr == NULL) && create){
            dragMgr = new KxDragManager(targetWidget);
            v.setValue((void*)dragMgr);
            targetWidget->setProperty(kDragManagerProp,v);
        }
    }
    return dragMgr;
}


QWidget * KxDragManager::getRealTarget(QObject *target)
{
    // if the target is a layout then the real target is its parent 
    QLayout *layout = qobject_cast<QLayout*>(target);
    if(layout) return layout->parentWidget();
    // if the target is derivered from QTextEdit , its mouse and rag events will 
    // be going to its viewport widget 
    QTextEdit *textEdit = qobject_cast<QTextEdit*>(target);
    if(textEdit)  return textEdit->viewport();
    // it has to be a widget to pareticipate in drag and drop 
    QWidget *widget = qobject_cast<QWidget*>(target);
    if(widget) return widget;
    return NULL;
}


bool KxDragManager::eventFilter(QObject *obj, QEvent *ev)
{
    bool eventHandled = false;

    // process events for drag handlers 
    if(fDragHandlers.count() > 0 ){
        switch(ev->type()){
            case QEvent::MouseButtonPress:
                {
                    QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(ev);
                    eventHandled =  handleMousePress(mouseEvent);
                }
                break;
#if defined(OSLinux_)
            case QEvent::MouseButtonRelease:
                {
                    // disable default QLineEdit behaviour for MMB release when we
                        // do a MMB paste as we handle that explicitly in
                        // handleMousePress().
                     QMouseEvent *mouseEvent = static_cast<QMouseEvent*>(ev);
                     eventHandled = (fMousePressWasPaste && mouseEvent->button() == Qt::MidButton);
                     fMousePressWasPaste = false;
                }
                break;
#endif
            default:
                break;
        }
    }
    /// process events for drop handlers 
    if(fDropHandlers.count() > 0){
        switch(ev->type()){
            case QEvent::DragEnter:
                {
                    QDragEnterEvent *enterEvent = static_cast<QDragEnterEvent*>(ev);
                    checkDropTypes(enterEvent);
                    eventHandled = true;
                }
                break;
            case QEvent::DragMove:
                {
                    if(fPositionMatters){
                        QDragMoveEvent *moveEvent = static_cast<QDragMoveEvent *>(ev);
                        checkDropTypes(moveEvent);
                    }
                    eventHandled = true;
                }
                break;
            case QEvent::Drop:
                {
                    QDropEvent *dropEvent = static_cast<QDropEvent*>(ev);
                    handleDrop(dropEvent);
                    eventHandled = true;
                }
                break;
            case QEvent::DragLeave:
                {
                    QDragLeaveEvent *leaveEvent = static_cast<QDragLeaveEvent *>(ev);
                    handleLeave(leaveEvent);
                    eventHandled = true;
                }
                break;
            default:
                break;
        }
    }
    return  eventHandled;
}


KxDragItemDataType KxDragManager::getDragType(QDropEvent *ev)
{
    KxDragItemDataType dragType = kUnknownSystemType;
    if(ev->mimeData()->hasUrls()){
        dragType = kFileNameDragType;
    }else if(ev->mimeData()->hasText()){
        dragType = kTextDragType;
    }
    return dragType;
}

bool KxDragManager::handleDrop(QDropEvent *ev)
{
       fRubberBand->hide();
       bool dropAccepted = false;
       // does this drag include custom Kx data 
       if(ev->mimeData()->hasFormat(KxDragData::kKxDataMineType)){
           // get all of the dragged data items 
           const KxDragData *dragData = qobject_cast<const KxDragData*>(ev->mimeData());
           KxDragItemList *items = dragData->getDragItems();

           // loop through all of the drop handlers 
           for(int h = 0; h < fDropHandlers.count(); ++h){
               KxDragItemDataTypeArray dropTypes;
               fDropHandlers[h].handler->getDropTypes(fTarget,ev,dropTypes);
               // for every drgagged item which provides an 
               // acceptable type of data, pass it on to the handler's dropdata method 
               for(int i=0;i < items->size(); ++i){
                   KxDragItem *item = items->at(i);
                   for(int j=0; j < dropTypes.length(); ++j){
                       if(item->canProvideType(dropTypes[i])){
                           if(fDropHandlers[h].handler->dropData(
                               fTarget,
                               ev,
                               dropTypes[j],
                               item->getDragData(dropTypes[j])
                               )){
                                 dropAccepted = true;
                                 break;
                           }
                       }
                   }
               }
           }
       } else {
           
           //	There's no custom Kx data so this data must originate
           //	from a user-defined control or from some other application.
           //	In that case we'll pass the handler's dropItem() method
           //	the text from QMimeType::text(), or the filename(s) from
           //	QMimeType::urls().
           KxDragItemDataType dragType = getDragType(ev);
           if(dragType != kUnknownSystemType){
               for(int h=0; h < fDropHandlers.count(); ++h){
                   KxDragItemDataTypeArray dropTypes;
                   fDropHandlers[h].handler->getDropTypes(fTarget,ev,dropTypes);

                   // Does this handler accept the drag type 
                   for(unsigned int i=0; i < dropTypes.length(); ++i){
                       if(dropTypes[i] == dragType){
                           if(dragType == kTextDragType){
                               QString *text = new QString(ev->mimeData()->text());
                               if(fDropHandlers[h].handler->dropData(fTarget,ev,kTextDragType,text)){
                                   dropAccepted = true;
                                   break;
                               }
                           }else{
                               // dragType == kFileNameDragType 
                               bool breakDropTypesLoop = false;
                               QList<QUrl> listOfUrls = ev->mimeData()->urls();
                               foreach(QUrl url, listOfUrls){
                                   QString *path = new QString(url.toLocalFile());
                                   if(fDropHandlers[h].handler->dropData(fTarget,ev,kFileNameDragType,path)){
                                       dropAccepted = true;
                                       breakDropTypesLoop = true;
                                   }
                               }
                               if(breakDropTypesLoop){
                                   break;
                               }
                           }
                       }
                   }
               }
           }
       }

       if(dropAccepted) ev->acceptProposedAction();
       return dropAccepted;
}

void KxDragManager::handleLeave(QDragLeaveEvent *ev)
{
    fRubberBand->hide();
    //	Loop through all of the drop handlers and let them know that we
    //	won't be dropping data after all, just in case they were getting
    //	all excited about it
    //
    //	We don't have access to a drop type, so we can't narrow it down.
    //
    for (int h = 0; h < fDropHandlers.count(); ++h) {
        fDropHandlers[h].handler->dropCancelled();
    }
}

bool KxDragManager::checkDropTypes(QDragMoveEvent *ev)
{
    const KxDragData *dragData = NULL;
    bool match = false;

    fPositionMatters = false;
    KxDragItemDataType dragType = kUnknownSystemType;
    if(ev->mimeData()->hasFormat(KxDragData::kKxDataMineType)){
        dragData = qobject_cast<const KxDragData*>(ev->mimeData());
    }else { 
        //	There's no custom Kx data so this must be a drag from a
        //	user-defined control or another application. The only external
        //	data types we currently recognize are files and text, so if this
        //	drag doesn't have one of those, then we don't care about it.
        dragType = getDragType(ev);
        if(dragType == kUnknownSystemType)
            return false;
    }
   QRect dropRect;
   foreach(HandlerEntry<KxDropHandler> entry, fDropHandlers){
       //	If the position of the drop point affects the types of
       //	data this handler offers then make a note of that since we
       //	may need to re-check the types every time the mouse moves.
       if (entry.handler->positionMatters()) fPositionMatters = true;
       //	Get the list of drop types which are acceptable to this
       //	handler.
        KxDragItemDataTypeArray dropTypes;
        entry.handler->getDropTypes(fTarget,ev,dropTypes);
        // are any of these types in the dragged data 
        for(int i=0; i < dropTypes.length(); ++i){
            if((dragData && dragData->hasType(dropTypes[i]))|| ((dragData == NULL) && (dropTypes[i] == dragType))){
                match = true;
                // It's expensive to get the drag items just to draw the rubber
                // band. Most of the time, dataMatters is false, so we don't
                // need the information to get the drop rectangle for
                // highlighting purposes. We'll only grab it if we truly need
                // it (e.g., for TclipEditorPort).
                KxDragItemList *items = entry.handler->dataMatters()? dragData->getDragItems():NULL;
                int numItems = (items == NULL)?1: items->size();
                // the data pointer is valid if we don't care, but otherwise 
                // we will need to find a valid data pointer 
                void *data = NULL;
                bool dataIsValid = true;
                for(int j=0; j < numItems; ++j){
                    if(items){
                        KxDragItem *item = items->at(j);
                        dataIsValid = item->canProvideType(dropTypes[i]);
                        if(dataIsValid){
                            data = item->getDragData(dropTypes[i]);
                        }
                    }
                    if(dataIsValid){
                        if(dropRect.isNull()){
                            dropRect = entry.handler->getDropRect(fTarget,ev,dropTypes[i],data);
                        }else{
                            QRect newDropRect = entry.handler->getDropRect(fTarget,ev,dropTypes[i],data);
                            // A null dropRect would wipe out the previous
                            // drag region, so check first
                            if(!newDropRect.isNull()){
                                dropRect = dropRect.intersected(newDropRect);
                                if(dropRect.isNull()){
                                    // Looks like we have two disjoint, non-null
                                    // regions. Pick the lesser (i.e, cheaper)
                                    // of two evils, and set dropRect to
                                    // newDropRect
                                    dropRect = newDropRect;
                                }
                            }
                        }

                    }
                }
                if(items == NULL){
                    // If the data doesn't matter, we don't need to check any
                    // other type, since we won't miss any extra information
                    //
                    break;
                }

            }
        }
   }
   if(dropRect.isNull()){
       fRubberBand->hide();
   }else{
#if defined (OSMac_X86_64_)
       // QT_TODO_MAC64
       //
       // On Mac 64, reset to a null rectangle before resizing, to get
       // around a QT bug.
       if(fRubberBand->size() != dropRect.size()){
           QRect nullRect;
           fRubberBand->resize(nullRect.size());
           fRubberBand->show();
       }
#endif
       fRubberBand->setGeometry(dropRect);
       fRubberBand->show();
   }
   return match;
}

bool KxDragManager::handleMousePress(QMouseEvent *ev)
{
    Qt::KeyboardModifiers mod;
    Qt::MouseButton button;
    KxQtHelper::getMouseEventQtButtonInfo(ev,button,mod);
#if defined(OSLinux_)
    // check for MMB paste on Linux before activating drag/drop handling
    //if (mod == Qt::NoModifier && button == Qt::MidButton) {
    //    KxMMBPaste* pasteTarget =
    //        dynamic_cast<KxMMBPaste*>(fTarget.data());
    //    if (pasteTarget && pasteTarget->canPaste(event)) {
    //        // get "selection" clipboard, only available on X11
    //        QClipboard* cb = QApplication::clipboard();
    //        QString text = cb->text(QClipboard::Selection);
    //        if (!text.isEmpty()) {
    //            pasteTarget->doPaste(event, text);
    //        }

    //        // this was a paste event, so set the flag to handle release and
    //        // return early
    //        fMousePressWasPaste = true;
    //        return true;
    //    }
    //}
#endif
    bool eventHandled = false;
    Qt::DropAction defaultAction = Qt::IgnoreAction;
    if((mod == Qt::NoModifier) || (mod == Qt::ControlModifier)){
        defaultAction = Qt::CopyAction;
    }else if(mod == Qt::ShiftModifier){
        defaultAction = Qt::MoveAction;
    }else if(mod == (Qt::ShiftModifier|Qt::ControlModifier)){
        defaultAction = Qt::LinkAction;
    }
    if((defaultAction != Qt::IgnoreAction) && ((button & fDragButtons) != 0)){
        // gather up the drag data from all of the handlers 
        KxDragData * dragData = KxDragData::create();
        QList<KxDragHandler*> handlersUsed;
        int   numDragItems = 0;
        foreach(HandlerEntry<KxDragHandler> entry, fDragHandlers){
            if((button & entry.handler->dragButtons()) != 0){
                entry.handler->getDragData(fTarget,ev,dragData);
                // keep track of the handlers which provide data
                if(dragData->getDragItems() && (dragData->getDragItems()->count() > numDragItems)){
                    handlersUsed.append(entry.handler);
                    numDragItems = dragData->getDragItems()->count();
                }
            }
        }
        if(!dragData->formats().isEmpty()){
            //	Qt will take ownership of 'dragData' but we will continue
                //	to own the custom Kiwi data within it. Get a pointer to
                //	that now so that we can delete it later.
            KxDragItemList *items = dragData->getDragItems();
            //	We have some drag data so start the drag.
            //
            //	QT_TODO	The old code used to open an undo chunk here
            //			and close after the drag ended. We need
            //			equivalent code to handle undoing a drag.
            QDrag *drag = new QDrag(fTarget);
            drag->setMimeData(dragData);
            Qt::DropAction dropAction;
            // On Mac64, the idle event used for deleting a shelf item in the
            // drag 'n drop to trash can operation was being executed before
            // the drag->exec was fully complete, and causing a crash (bug
            // #336981). Suspending all idle events prevents this type of
            // problem on Mac64, and, potentially, on other platforms in the
            // future.
           //	TeventHandler::suspendIdleEvents();
            dropAction = drag->exec(Qt::CopyAction | Qt::MoveAction | Qt::LinkAction,defaultAction);
            // TeventHandler::resumeIdleEvents();
            //	Let each of the handlers which provided data know how the
            //	drag turned out.
            foreach(KxDragHandler* handler, handlersUsed){
                handler->dragDone(fTarget,ev,dropAction);
            }
            // delete the data items 
            if(items){
                for(int i=0; i < items->size(); ++i){
                    if(items->at(i)->needsDeletion()){
                        delete items->at(i);
                    }
                }
                delete items;
            }
            ev->accept();
            eventHandled = true;
        }else{
            delete dragData;
        }
    }
    return eventHandled;
}





