#ifndef KxDropHandler_H_
#define KxDropHandler_H_
#include "../base/HQtGui.h"
#include "dragitem/KxDragItem.h"

class KX_DLLEXPORT KxDropHandler 
{
public:
    KxDropHandler() {}
    virtual ~ KxDropHandler(){}
    //	The handler must reimplement this method to handle dropped data. If
    //	the handler accepts the data it must return true, otherwise it must
    //	return false.
    //
    //	If the drag contains multiple items of data this method will be
    //	called once for each item.
    virtual bool dropData(QWidget *target, const QDropEvent *ev, int dataType, void *data) = 0;


    //	The handler must reimplement this method to return an array of the
    //	types of dropped data which it is willing to accept. If the
    //	acceptable types depend upon where in the control the drop occurs
    //	the method should only return those types which are valid for the
    //	position returned by event->pos().
    virtual void	getDropTypes(
        const QWidget*			target,
        const QDropEvent*		event,
        KxDragItemDataTypeArray & dropTypes
        ) const = 0;

    //	If the data types acceptable to the handler depend upon where in
    //	the target control the drop occurs, the handler must reimplement
    //	this method to return true.
    virtual bool	positionMatters() const;

    // If the getDropRect method needs access to the dataType and the data, the
    // target control must reimplement this method to return true, so that the
    // drag manager can pass in the data. If dataMatters() returns false, the
    // drag manager will pass a null pointer to getDropRect. This default
    // behaviour is necessary to avoid the overhead of querying the drag items
    // when the data is not actually needed.
    //
    virtual bool	dataMatters() const;
    virtual QRect	getDropRect(QWidget* target,
        QDropEvent* event,
        int dataType,
        void* data );

    // Called if a drag leaves the control without performing a drop. This
    // allows the handler to clean up any preparations it may have made in
    // getDropTypes() or getDropRect().
    virtual void	dropCancelled(){};

};
inline bool KxDropHandler::positionMatters() const
{ return false; }

inline bool KxDropHandler::dataMatters() const
{ return false; }

inline QRect KxDropHandler::getDropRect(	QWidget* target,
                                           QDropEvent*,
                                           int,
                                           void* )
{ return target->rect();}


#endif