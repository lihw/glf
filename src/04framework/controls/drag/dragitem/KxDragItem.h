#ifndef KxDragItem_H_
#define KxDragItem_H_
#include "../../base/HQtGui.h"
#include "../KxDragItemDataType.h"

enum KxDragOperationType {kDragMove, kDragCopy, kDragLink, kDragNone};

typedef QList<KxDragItemDataType> KxDragItemDataTypeArray;

class KX_DLLEXPORT KxDragItem 
{
public:
    KxDragItem(bool bDel = false);
    virtual ~KxDragItem();
    //	If it makes sense to be able to paste the item into other
    //	applications (e.g. a terminal window or a text editor) then
    //	override this method to provide a string representation of the
    //	item.
    virtual QString asText() {return QString();}
    virtual KxDragItemDataType dragItemType();
    virtual void getDragTypes(KxDragItemDataTypeArray &);
    virtual void* getDragData(KxDragItemDataType);
    virtual bool canProvideType(KxDragItemDataType );
    virtual bool needsDeletion();

public:
    bool fbDelete;
};

#endif