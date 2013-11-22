#include "KxCheckBoxGrp.h"

#include "../button/KxCheckBox.h"
#include "../../layouts/KxRowLayout.h"
#include "../../layouts/KxRowColumnLayout.h"

#define kCheckBoxBaseName "check"

// Description:  Constructor.
//
KxCheckBoxGrp::KxCheckBoxGrp(
                                   QWidget *parent, bool hasLabel, int nChecks)
                                   : KxWidgetGrp(parent)
{
    if (hasLabel)
    {
        // Add a label.
        //
        addLabel();
    }

    // Add the check boxes.
    //
    for (int i = 0; i < nChecks; i++)
    {
        QString text;
        KxCheckBox * checkBox = new KxCheckBox(text);
        if (checkBox != NULL)
        {
            QString name;
            QTextStream(&name) << kCheckBoxBaseName << i+1;
            checkBox->setObjectName(name);
            addField(checkBox);
        }  
    }
}

// Description:  This procedure add a field to the group.
//
void KxCheckBoxGrp::addField(KxCheckBox *newField)
{
    // Put the widget in the layout.
    //
    addWidgetToGrp(newField);

    // Keep a pointer to it on the list.
    //
    fFieldList.append(newField);
}

// Description:  This procedure returns the check box field in the
//               group.
//
void KxCheckBoxGrp::getFields( KxCheckBox *(&X),
                                 KxCheckBox *(&Y),
                                 KxCheckBox *(&Z),
                                 KxCheckBox *(&W))
{
    X = NULL; Y = NULL; Z = NULL; W = NULL;
    int numOfFields = fFieldList.count();
    if (numOfFields >= 1) X = fFieldList[0];
    if (numOfFields >= 2) Y = fFieldList[1];
    if (numOfFields >= 3) Z = fFieldList[2];
    if (numOfFields >= 4) W = fFieldList[3];
}

void KxCheckBoxGrp::makeVertical()
{
    if(fRowLayout != NULL) {
        QList<QLayoutItem *> childList;
        while(fRowLayout->count() > 0) {
            childList.append(fRowLayout->takeAt(0));
        }

        delete fRowLayout;
        fRowLayout = NULL;

        KxRowColumnLayout::ConstraintMode mode = KxRowColumnLayout::kFixNumberOfColumns;
        if (fLabel != NULL) {
            // Need two columns for the label and the checkboxes
            fRowColumnLayout = new KxRowColumnLayout(this, mode, 2);

            // The label is always the first child, so add it in
            fRowColumnLayout->addItem(childList.takeFirst());
        } else {
            // Need just one column for the checkboxes
            fRowColumnLayout = new KxRowColumnLayout(this, mode, 1);
        }

        // Add in the checkboxes
        while(!childList.isEmpty()) {
            fRowColumnLayout->addItem(childList.takeFirst());

            // Add a spacer item for buffer space under the label if there is one (don't need to do this if this is the last checkbox) 
            if(fLabel != NULL && !childList.isEmpty()) {
                QSpacerItem *indent = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
                fRowColumnLayout->addItem(indent);
            }
        }

        KxWidgetGrp::makeVertical();
    }
}
