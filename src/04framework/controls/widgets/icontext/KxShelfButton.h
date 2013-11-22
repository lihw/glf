#ifndef KxShelfButton_H_
#define KxShelfButton_H_

#include "../../base/HQtGui.h"
#include "../../drag/KxDragHandler.h"
#include "../icontext/KxIconTextButton.h"


class KX_DLLEXPORT KxShelfButton : public KxIconTextButton,
    public KxDragHandler
{
    Q_OBJECT
public:
    KxShelfButton(QWidget *parent=0);
    /*void			setCommand( Taction* );*/
    virtual void	getDragData( QWidget*, QMouseEvent*, KxDragData* );

private:
   // TcommandDragItem	  	fDragItem;
};

#endif