#ifndef KxDragData_H_
#define KxDragData_H_
#include "../base/HQtGui.h"
#include "dragitem/KxDragItem.h"

typedef QList<KxDragItem*> KxDragItemList;

class KX_DLLEXPORT KxDragData : public QMimeData 
{
    Q_OBJECT
public:
    static KxDragData* create(){return new KxDragData();}
    void    addDragItem(KxDragItem *);
    void    appendText(const QString &textToAdd);
    void    clear();
    KxDragItemList * getDragItems()const;
    bool  hasType(KxDragItemDataType type)const;
    static const char * kKxDataMineType;

private:
    KxDragData(){}
};


#endif