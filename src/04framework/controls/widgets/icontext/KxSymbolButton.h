#ifndef KxSymbolButton_H_
#define KxSymbolButton_H_

#include "../../base/HQtGui.h"
#include "KxIconTextButton.h"
class KX_DLLEXPORT KxSymbolButton : public KxIconTextButton
{
    Q_OBJECT
public:
    KxSymbolButton(QWidget *parent=0);
};
#endif