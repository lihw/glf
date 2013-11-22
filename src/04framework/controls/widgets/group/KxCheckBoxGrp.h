#ifndef KxCheckBoxGrp_H_
#define KxCheckBoxGrp_H_

#include "../../base/HQtGui.h"
#include "KxWidgetGrp.h"

class KxCheckBox;

class KX_DLLEXPORT KxCheckBoxGrp : public KxWidgetGrp
{
    Q_OBJECT

public:
    KxCheckBoxGrp(QWidget *parent=0, 
        bool hasLabel=false, 
        int nChecks=1);

    void addField(KxCheckBox *);
    void getFields(KxCheckBox * (&X),
        KxCheckBox * (&Y),
        KxCheckBox * (&Z),
        KxCheckBox * (&W));
    virtual void makeVertical();

private:
    QList<KxCheckBox *> fFieldList;
};
#endif