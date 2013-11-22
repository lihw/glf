#ifndef KxSymbolCheckBox_H_
#define KxSymbolCheckBox_H_

#include "../../base/HQtGui.h"
#include "KxIconTextCheckBox.h"
class KX_DLLEXPORT KxSymbolCheckBox : public KxIconTextCheckBox
{
    Q_OBJECT
public:
    KxSymbolCheckBox(QWidget *parent=0);
};

#endif