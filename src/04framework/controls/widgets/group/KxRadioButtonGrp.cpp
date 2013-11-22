#include "KxRadioButtonGrp.h"

#include "../button/KxRadioButton.h"
#include "../button/KxButtonGroup.h"
#include "../../layouts/KxRowLayout.h"
#include "../../layouts/KxRowColumnLayout.h"


#define kRadioButtonBaseName "radio"

// Description:  Constructor.
//
KxRadioButtonGrp::KxRadioButtonGrp(
    QWidget *parent, bool hasLabel, int nRadio, KxButtonGroup *buttonGroup)
    : KxWidgetGrp(parent)
{
    if (hasLabel)
    {
        // Add a label.
        //
        addLabel();
    }

    // Add the radio buttons.
    //
    for (int i = 0; i < nRadio; i++)
    {
        QString text;
        KxRadioButton * radioButton = new KxRadioButton(text);
        if (radioButton != NULL)
        {
            QString name;
            QTextStream(&name) << kRadioButtonBaseName << i+1;
            radioButton->setObjectName(name);
            addField(radioButton);
            if (buttonGroup != NULL)
            {
                buttonGroup->addButton(radioButton);
            }
        }
    } 
}

// Description:  This procedure add a field to the group
//
void KxRadioButtonGrp::addField(KxRadioButton *newField)
{
    // Put the widget in the layout.
    //
    addWidgetToGrp(newField);

    // Keep a pointer to it on the list.
    //
    fFieldList.append(newField);
}

// Description:  This procedure returns the radio button 
//               in the group.
//
void KxRadioButtonGrp::getFields( KxRadioButton * (&X),
                                    KxRadioButton * (&Y),
                                    KxRadioButton * (&Z),
                                    KxRadioButton * (&W))
{
    X = NULL; Y = NULL; Z = NULL; W = NULL;
    int numOfFields = fFieldList.count();
    if (numOfFields >= 1) X = fFieldList[0];
    if (numOfFields >= 2) Y = fFieldList[1];
    if (numOfFields >= 3) Z = fFieldList[2];
    if (numOfFields >= 4) W = fFieldList[3];
}

void KxRadioButtonGrp::makeVertical()
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
            // Need two columns for the label and the radio buttons
            fRowColumnLayout = new KxRowColumnLayout(this, mode, 2);

            // The label is always the first child, so add it in
            fRowColumnLayout->addItem(childList.takeFirst());
        } else {
            // Need just one column for the radio buttons
            fRowColumnLayout = new KxRowColumnLayout(this, mode, 1);
        }

        // Add in the radio buttons
        while(!childList.isEmpty()) {
            fRowColumnLayout->addItem(childList.takeFirst());

            // Add a spacer item for buffer space under the label if there is one (don't need to do this if this is the last radio button) 
            if(fLabel != NULL && !childList.isEmpty()) {
                QSpacerItem *indent = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
                fRowColumnLayout->addItem(indent);
            }
        }

        KxWidgetGrp::makeVertical();
    }
}
