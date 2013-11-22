#ifndef QPrimitiveDragItem_H_
#define QPrimitiveDragItem_H_

#include "../../base/HQtGui.h"
#include "KxDragItem.h"

class KX_DLLEXPORT KxPrimitiveDragItem : public KxDragItem
{
public:
       KxPrimitiveDragItem(bool bDel = false);
       virtual ~KxPrimitiveDragItem();

       virtual QString asText() {return string();}
       const QString string()const;
       void setString(const QString &);
       
       virtual KxDragItemDataType dragItemType();
       virtual void getDragTypes(KxDragItemDataTypeArray &);
       virtual void * getDragData(KxDragItemDataType);
       virtual bool canProvideType(KxDragItemDataType );

private:
    QString fString;
};

#endif