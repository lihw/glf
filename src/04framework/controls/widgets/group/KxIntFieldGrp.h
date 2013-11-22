#ifndef KxIntFieldGrp_H_
#define KxIntFieldGrp_H_

#include "../../base/HQtGui.h"
#include "KxWidgetGrp.h"

class KxIntField;


class KX_DLLEXPORT KxIntFieldGrp : public KxWidgetGrp
{
    Q_OBJECT

public:
    KxIntFieldGrp(QWidget *parent = 0);

    void    addField(KxIntField *);
    void    getFields(KxIntField * (&X),
        KxIntField * (&Y),
        KxIntField * (&Z),
        KxIntField * (&W));
    int  value(int index) const;
    void    setValue(int index, int newValue);

private:
    QList<KxIntField *> fFieldList;
};
#endif