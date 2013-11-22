#include "KxDragItem.h"

KxDragItem::KxDragItem(bool bDel /* = false */)
:fbDelete(bDel)
{

}


KxDragItem::~KxDragItem()
{

}

bool KxDragItem::needsDeletion()
{
    return fbDelete;
}

KxDragItemDataType KxDragItem::dragItemType()
{
    return 0;
}

bool KxDragItem::canProvideType(KxDragItemDataType )
{
    return false;
}


void KxDragItem::getDragTypes(KxDragItemDataTypeArray &)
{

}


void * KxDragItem::getDragData(KxDragItemDataType)
{
    return NULL;
}

