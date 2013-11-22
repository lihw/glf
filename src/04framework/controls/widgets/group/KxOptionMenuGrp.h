#ifndef  KxOptionMenuGrp_H_
#define  KxOptionMenuGrp_H_
#include "../../base/HQtGui.h"
#include "KxWidgetGrp.h"
class KxOptionMenu;

class KX_DLLEXPORT KxOptionMenuGrp : public KxWidgetGrp
{
    Q_OBJECT

public:
    KxOptionMenuGrp(QWidget *parent = NULL, bool hasLabel = false, KxOptionMenu *optionMenu = NULL, bool hasExtraLabel = false);

    KxOptionMenu *	optionMenu() const;

private:
    KxOptionMenu *	fOptionMenu;
};

inline KxOptionMenu * KxOptionMenuGrp::optionMenu() const
{
    return fOptionMenu;
}
#endif