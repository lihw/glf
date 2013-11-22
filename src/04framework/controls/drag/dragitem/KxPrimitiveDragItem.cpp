#include "KxPrimitiveDragItem.h"


KxPrimitiveDragItem::KxPrimitiveDragItem(bool bDel /* = false */)
:KxDragItem(bDel)
{

}


KxPrimitiveDragItem::~KxPrimitiveDragItem()
{

}

const QString KxPrimitiveDragItem::string()const
{
    return fString;
}

void KxPrimitiveDragItem::setString(const QString &src)
{
    fString = src;
}


KxDragItemDataType KxPrimitiveDragItem::dragItemType()
{
     return kPrimitiveDragType;
}

void KxPrimitiveDragItem::getDragTypes(KxDragItemDataTypeArray & typeArray)
{
    if(fString.length() > 0){
        typeArray.append(kTextDragType);
    }
}


void  *KxPrimitiveDragItem::getDragData(KxDragItemDataType dataType)
{
    void *dataPtr = NULL;
    if(kTextDragType == dataType){
        dataPtr = &fString;
    }else {
        dataPtr = &fString;
    }
    return dataPtr;

}


bool KxPrimitiveDragItem::canProvideType(KxDragItemDataType  theType)
{
    return (kTextDragType == theType || kPrimitiveDragType == theType) ? true: false; 
}