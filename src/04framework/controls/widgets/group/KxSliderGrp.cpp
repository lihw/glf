#include "KxSliderGrp.h"

#include "../display/KxLabel.h"
#include "../../layouts/KxRowColumnLayout.h"
#include "../../base/KxQtHelper.h"
#include "../input/KxNumericField.h"
#include "../slider/KxSlider.h"
#include "../../layouts/KxRowLayout.h"




#define kLabelName "label"

// Description:  This constructor allows the user the specify
//               that we want the group to have a label.  We constructor the
//               label here.
//
KxSliderGrp::KxSliderGrp(
                               QWidget *parent, bool hasLabel, KxNumericField * newField, 
                               KxSlider *newSlider, bool hasExtraLabel)
                               : KxConnectableWidgetGrp(parent)
{
    KxLabel * newLabel = NULL;
    if (hasLabel)
    {
        newLabel = new KxLabel(this);
        newLabel->setObjectName(kLabelName);
    }

    init(newLabel, newField, newSlider, hasExtraLabel);
}

// Description:  This constructor allows the user to pass in the
//               label they constructed.
//               For example, the user might want the label to be
//               of type KxExprLabel which is a subclass of
//               KxLabel.
//
KxSliderGrp::KxSliderGrp(
                               QWidget *parent, KxLabel * newLabel, KxNumericField * newField, 
                               KxSlider *newSlider, bool hasExtraLabel)
                               : KxConnectableWidgetGrp(parent)
{
    init(newLabel, newField, newSlider, hasExtraLabel);
}

// Description:  This procedure initialize the group and add the
//               sub-widgets into the group.
//
void KxSliderGrp::init(KxLabel * newLabel, KxNumericField * newField,
                          KxSlider *newSlider, bool hasExtraLabel)
{
    if (newLabel != NULL)
    {
        // Add the label.
        //
        fLabel = newLabel;
        addWidgetToGrp(newLabel); 
    }

    // Put in an integer field, if fField is not NULL.
    //
    addWidgetToGrp(newField);

    // Put in an integer slider.
    //
    if(newSlider != NULL) {
        addWidgetToGrp(newSlider);
    } else {
        QSpacerItem *space = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        fLayout->addItem(space);
    }

    if (hasExtraLabel)
    {
        // Put in an extra label.
        //
        addExtraLabel();
    }

    makeConnections(newField, newSlider);
}


void KxSliderGrp::setValue(double newValue)
{
    setFieldValue(newValue);
    setSliderValue(newValue);
}


void KxSliderGrp::insertSlider(KxSlider* newSlider)
{
    if (newSlider)
    {
        if(!isVertical()) {
            if(fRowLayout != NULL) {
                // Find the spacer item currently replacing the slider and remove it from the layout
                QSpacerItem *space = NULL;
                int	index = 0;

                while(index < fLayout->count()) {
                    QLayoutItem *item = fLayout->itemAt(index);
                    if(item != NULL) {
                        space = dynamic_cast<QSpacerItem *>(item);
                        if(space != NULL) {				
                            break;
                        }
                    }

                    index++;
                }

                fLayout->removeItem(space);

                // Insert the slider in the index where the spacer item was
                fRowLayout->insertWidget(index, newSlider);
            }
        } else {
            if(fRowColumnLayout != NULL) {
                if(fLabel != NULL) {
                    // If there is a label, put a spacer item below it so that the slider will be under the field
                    QSpacerItem *indent = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
                    fRowColumnLayout->addItem(indent);
                }

                // Put in the slider
                fRowColumnLayout->addWidget(newSlider);
            }
        }

        makeConnections(numericField(), newSlider);
    }
}

void KxSliderGrp::insertSliderSpace(int index)
{
    if(fRowLayout != NULL) {
        QSpacerItem *space = new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Minimum);
        fRowLayout->insertItem(index, space);
    }
}

void KxSliderGrp::removeSlider(KxSlider* slider)
{
    if(slider) {
        int index = fLayout->indexOf(slider);

        //	Simply deleting the slider will take care of removing it from the
        //	group, the layout, breaking its connections, and so forth.
        delete slider;

        if(!isVertical()) {
            insertSliderSpace(index);
        } else {
            if(fLabel != NULL) {
                // Find the spacer item that was under the label and remove it from the layout
                QSpacerItem *space = NULL;
                int	index = 0;

                while(index < fLayout->count()) {
                    QLayoutItem *item = fLayout->itemAt(index);
                    if(item != NULL) {
                        space = dynamic_cast<QSpacerItem *>(item);
                        if(space != NULL) {				
                            break;
                        }
                    }

                    index++;
                }

                fLayout->removeItem(space);
            }
        }
    }
}

// Description:  Connect slots and signals to keep the slider and the field
//               in synch.
//
void KxSliderGrp::makeConnections(
                                     KxNumericField* field, KxSlider* slider
                                     )
{
    if (field && slider)
    {
        // When the slider's value changes, we want to change the
        // field's value.
        //
        // There may be change commands attached to the slider's
        // 'sliderChangeCompleted' signal and we want to be sure that the
        // field has been updated before those commands run. To that end,
        // we use the 'newValueForConnections' signal for this because it
        // will be emitted before 'sliderChangeCompleted'.
        //
        connect(
            slider,
            SIGNAL(newValueForConnections(const QVariant&, bool)),
            this,
            SLOT(useSliderValueToSetFieldValue())
            );

        // When the user editted the field, we want to update the 
        // slider.
        // The user can be directly editing the field by 
        //   . either typing in a number
        //   . or use the invisio-slider. 
        //
        //
        // There may be change commands attached to the field's
        // 'changeCompleted' signal and we want to be sure that the
        // slider has been updated before those commands run. To that end,
        // we use the 'newValueForConnections' signal for this because it
        // will be emitted before 'changeCompleted'.
        //
        connect(
            field,
            SIGNAL(newValueForConnections(const QVariant&, bool)),
            this,
            SLOT(useFieldValueToSetSliderValue())
            );
    }
}


// Description:  This procedure is called after the slider is moved.
//               We want to use the slider's value to set the 
//               field's value.
//
void KxSliderGrp::useSliderValueToSetFieldValue()
{
    if (mayaSlider() == NULL || numericField() == NULL)
    {
        return;
    }

    double newValue = mayaSlider()->sliderValueF();
    if (numericField()->valueF() != newValue) 
    {
        setFieldValue(newValue);
    }
}

// Description:  This procedure is called after the user editted
//               the field.  We want to use the field value to 
//               set the slider value.
//
void KxSliderGrp::useFieldValueToSetSliderValue()
{
    KxSlider * theSlider = mayaSlider();
    KxNumericField * theField = numericField();

    if (theSlider ==NULL || theField ==NULL)
    {
        return;
    }

    double fieldValue = theField->valueF();

    if (theSlider->sliderValueF() != fieldValue)
    {
        setSliderValue(fieldValue);
    }
}


void KxSliderGrp::setFieldValue(double newValue)
{
    KxNumericField * theField = numericField();

    if (theField) {
        //  Clip the value to the field's range.
        double fieldMin = theField->minValueF();
        double fieldMax = theField->maxValueF();

        if (newValue < fieldMin) {
            newValue = fieldMin;
        } else if (newValue > fieldMax) {
            newValue = fieldMax;
        }

        theField->newValueFromConnection(newValue);
    }
}


void KxSliderGrp::setSliderValue(double newValue)
{
    KxSlider * theSlider = mayaSlider();

    if (theSlider) {
        // If the new value lies outside the slider's current range, but
        // within that of the field (assuming that we have a field) then
        // expand the range of the slider to include the new value.
        //
        // If there's no field available then the value will be clipped to
        // the slider's current range.
        //
        double sliderMin = theSlider->sliderMinimumF();
        double sliderMax = theSlider->sliderMaximumF();
        KxNumericField * theField = numericField();

        if ( newValue < sliderMin ){

            if (theField) {
                // First, get the field min to use as an absolute "minimal
                // min"
                //
                double fieldMin = theField->minValueF();

                // If our new value is less than 0, the new min will be
                // twice that value, bounded by the fieldMin
                //
                if ( newValue < 0 ){
                    sliderMin = 2 * newValue;
                } else {
                    // The new value is greater than 0.  Increase the range
                    // downwards by a distance of 1/2 the current range.
                    //
                    sliderMin -= (sliderMax-sliderMin)/2;
                }
                if ( sliderMin < fieldMin ){
                    sliderMin = fieldMin;
                }

                theSlider->setSliderRangeF(sliderMin,sliderMax);
            } else {
                newValue = sliderMin;
            }
        } else if ( newValue > sliderMax ){

            if (theField) {
                // First, get the field max to use as an absolute "maximal
                // max"
                //
                double fieldMax = theField->maxValueF();

                // If our new value is greater than 0, the new max will be
                // twice that value, bounded by the fieldMax
                //
                if ( newValue > 0 ){
                    sliderMax = 2 * newValue;
                } else {
                    // The new value is less than 0.  Increase the range
                    // upwards by a distance of 1/2 the current range.
                    //
                    sliderMax += (sliderMax-sliderMin)/2;
                }
                if ( sliderMax > fieldMax ){
                    sliderMax = fieldMax;
                }

                theSlider->setSliderRangeF(sliderMin,sliderMax);
            } else {
                newValue = sliderMax;
            }
        }

        theSlider->newValueFromConnection(newValue);
    }
}
