#include "KxOptionMenuGrp.h"

#include "../../menus/KxOptionMenu.h"
#include "../display/KxLabel.h"


#define kLabelName 				"Label"
#define kOptionMenuName 		"OptionMenu"
#define kExtraLabelName 		"ExtraLabel"


// Description:	 Constructor
//
KxOptionMenuGrp::KxOptionMenuGrp(QWidget *parent, bool hasLabel, KxOptionMenu *optionMenu, bool hasExtraLabel)
: KxWidgetGrp(parent)
, fOptionMenu(optionMenu)
{
    // Add a label.
    if(hasLabel) {
        KxLabel *label = new KxLabel();
        label->setObjectName(kLabelName);
        addLabel(label);
    }

    // Add option menu
    fOptionMenu->setObjectName(kOptionMenuName);
    addWidgetToGrp(fOptionMenu);

    // Add extra label
    if(hasExtraLabel) {
        KxLabel *extraLabel = new KxLabel();
        extraLabel->setObjectName(kExtraLabelName);
        addExtraLabel(extraLabel);
    }
}
