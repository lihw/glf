#ifndef KxFloatFieldGrp_H_
#define KxFloatFieldGrp_H_
#include "../../base/HQtGui.h"
#include "KxWidgetGrp.h"
class KxFloatField;

class KX_DLLEXPORT KxFloatFieldGrp : public KxWidgetGrp
{
    Q_OBJECT

public:
    KxFloatFieldGrp(QWidget *parent = 0);

    void    addField(KxFloatField *);
    void    getFields(KxFloatField * (&X),
        KxFloatField * (&Y),
        KxFloatField * (&Z),
        KxFloatField * (&W));

private:
    QList<KxFloatField *> fFieldList;
};
#endif