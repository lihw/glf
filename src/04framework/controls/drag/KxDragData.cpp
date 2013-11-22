#include "KxDragData.h"

const char * KxDragData::kKxDataMineType = "application/x-Kx-data";


//	Append 'text' to the "text/plain" value.  If the object already
//	contains a "text/plain" value then 'text' will be appended to that with
//	an intervening newline.

void KxDragData::appendText(const QString &textToAdd)
{
    QString curText = text();
    if(curText.isEmpty()){
        setText(textToAdd);
    }else {
        curText += '\n';
        curText += textToAdd;
        setText(curText);
    }

}


void KxDragData::addDragItem(KxDragItem *newItem)
{
    KxDragItemList *items  = getDragItems();
    if(items == NULL){
        items = new KxDragItemList();
        QByteArray listPtr((char*)&items,sizeof(KxDragItemList*));
        setData(kKxDataMineType,listPtr);
    }
    items->append(newItem);
    const QString text = newItem->asText();
    if(!text.isEmpty()){
        appendText(text);
    }
}

void KxDragData::clear()
{
    KxDragItemList *items = getDragItems();
    if(items != NULL){
        for(int i=0; i < items->size(); ++i){
            if(items->at(i)->needsDeletion()){
                delete items->at(i);
            }
        }
        delete items;
        removeFormat(kKxDataMineType);
    }
}

KxDragItemList *KxDragData::getDragItems()const
{
    KxDragItemList * items  = NULL;
    QByteArray listPtr = data(kKxDataMineType);
    if(listPtr.size() >= sizeof(KxDragItemList*)){
        items = *(KxDragItemList**)listPtr.data();
    }
    return items;
}

bool KxDragData::hasType(KxDragItemDataType type)const
{
    KxDragItemList *items = getDragItems();
    if(items){
        for(int i=0;i < items->size(); ++i){
            if(items->at(i)->canProvideType(type)) return true;
        }
    }
    return false;
}

