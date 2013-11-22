#ifndef KxTextFieldGrp_H_
#define KxTextFieldGrp_H_

#include "../../base/HQtGui.h"
#include "KxWidgetGrp.h"

class KX_DLLEXPORT KxTextFieldGrp : public KxWidgetGrp
{
    Q_OBJECT

public:
    KxTextFieldGrp(QWidget *parent=0, bool hasLabel = false);

    KxField * inputField();
    KxField * kxField();

private:
    KxField * fField;    
};

inline KxField * KxTextFieldGrp::inputField()
{
    return fField;
}

inline KxField * KxTextFieldGrp::kxField()
{
    return fField;
}

#endif