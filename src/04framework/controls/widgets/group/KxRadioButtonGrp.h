#ifndef KxRadioButtonGrp_H_
#define KxRadioButtonGrp_H_
#include "../../base/HQtGui.h"
#include "KxWidgetGrp.h"

class KxRadioButton;
class KxButtonGroup;


class KX_DLLEXPORT KxRadioButtonGrp : public KxWidgetGrp
{
    Q_OBJECT

public:
    KxRadioButtonGrp(QWidget *parent=0, 
        bool hasLabel=false,
        int nRadio=1,
        KxButtonGroup *buttonGroup=NULL);

    void addField(KxRadioButton *);
    void getFields(KxRadioButton * (&X),
        KxRadioButton * (&Y),
        KxRadioButton * (&Z),
        KxRadioButton * (&W));
    virtual void makeVertical();

private:
    QList<KxRadioButton *> fFieldList;
};
#endif
