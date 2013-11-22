#include "KxWidgetGrp.h"

#include "../display/KxLabel.h"
#include "../../layouts/KxRowColumnLayout.h"
#include "../../layouts/KxRowLayout.h"


#define kLabelName "label"
#define kExtraLabelName	 "extraLabel"

// Description:  Constructor
//
KxWidgetGrp::KxWidgetGrp(QWidget *parent)
: QWidget(parent)
, fLayout(NULL)
, fRowLayout(NULL)
, fRowColumnLayout(NULL)
, fLabel(NULL)
, fExtraLabel(NULL)
, fExtraLabelWidth(-1)
{
    // Make a horizontal layout for laying out the sub-widgets
    // of the group.
    //
    fRowLayout = new KxRowLayout(this);
    fLayout = fRowLayout;
    fIsVertical = false;

    // Put a small buffer at the end of the widget group so that the right-
    // most widget doesn't touch the edge of the group's parent layout.
    //
    fLayout->setContentsMargins(1,1,5,1);

    setLayout(fLayout);
}

// Description:  This procedure is called to add sub-widget to the
//               group.
//
void KxWidgetGrp::addWidgetToGrp(QWidget *newWidget)
{
    if (fLayout == NULL || newWidget == NULL)
    {
        return;
    }

    fLayout->addWidget(newWidget);
}

// Description:  This procedure is called to add a label to the group.
//
void KxWidgetGrp::addLabel()
{
    if (fLabel != NULL)
    {
        // We already have a label.
        //
        return;
    }
    fLabel = new KxLabel(this);
    fLabel->setObjectName(kLabelName);
    addWidgetToGrp(fLabel);
}

// Description:  This procedure is called to add a caller-supplied label to
//               the group.
//
void KxWidgetGrp::addLabel(KxLabel* label)
{
    if (fLabel != NULL)
    {
        // We already have a label.
        //
        return;
    }
    fLabel = label;
    addWidgetToGrp(fLabel);
}

// Description:  This procedure is called to add a extra label 
//               to the group.
//
void KxWidgetGrp::addExtraLabel()
{
    if (fExtraLabel != NULL)
    {
        // We already have an extra label.
        //
        return;
    }
    fExtraLabel = new KxLabel(this);
    fExtraLabel->setObjectName(kExtraLabelName);
    addWidgetToGrp(fExtraLabel);
}


// Description:  This procedure is called to add a caller-supplied extra label 
//               to the group.
//
void KxWidgetGrp::addExtraLabel(KxLabel* label)
{
    if (fExtraLabel != NULL)
    {
        // We already have an extra label.
        //
        return;
    }
    fExtraLabel = label;
    addWidgetToGrp(fExtraLabel);
}

// Description:  This procedure is called to return the text for
//               the label.
//
QString KxWidgetGrp::labelText() const
{
    QString text;
    if(fLabel != NULL)
        text = fLabel->text();
    return text;
}

// Description:  This procedure is called to set the text for
//               the label.
//
void KxWidgetGrp::setLabelText( const QString &newText)
{
    if (fLabel != NULL)
    {
        fLabel->setText(newText); 
    }
}

// Description:  This procedure is called to return the text for
//               the extra label.
//
QString KxWidgetGrp::extraLabelText() const
{
    QString text;
    if(fLabel != NULL)
        text = fLabel->text();
    return text;
}

// Description:  This procedure is called to set the text for
//               the extra label.
//
void KxWidgetGrp::setExtraLabelText( const QString &newText)
{
    if (fExtraLabel != NULL)
    {
        fExtraLabel->setText(newText);

        // Check whether or not the text to be set is an empty string. If it is, find
        // the column the extra label is in and set the width to 0, since we don't want
        // a blank label taking up space. Otherwise, set it to whatever width setting
        // may have been given to it before.
        // NOTE: Extra labels are currently only available in widget groups that
        // can only be horizontally laid out. As such, the following search for the 
        // extra label column is based on the assumption that we are working with 
        // a row layout, where the column indices would match the count. This will
        // have to be reworked if extra labels are later made available for vertical
        // layouts.
        if(!fIsVertical && fRowLayout != NULL) {
            int extraLabelColumn = -1;	   		
            for(int i = numColumns() - 1; i >= 0; i--) {
                KxLabel *extraLabel = qobject_cast<KxLabel *>(widget(i));
                if(extraLabel == fExtraLabel) { 
                    extraLabelColumn = i;
                    break;
                }
            }
            if(extraLabelColumn >= 0) {
                if(newText.isEmpty()) {
                    fRowLayout->setColumnWidth(extraLabelColumn, 0, false);
                } else if(fExtraLabelWidth >= 0) {
                    fRowLayout->setColumnWidth(extraLabelColumn, fExtraLabelWidth, false);
                } else {
                    fRowLayout->setColumnWidth(extraLabelColumn, fExtraLabel->sizeHint().width(), false);
                }
            }
        }
    }
}

// Description:  This method returns the 'index'th widget in the group
//               where the first widget has index 0. Non-widget items in
//               the group are ignored. If there is no 'index'th widget in
//               the group then NULL is returned.
//
QWidget* KxWidgetGrp::widget(unsigned int index)
{
    QWidget*  result = NULL;

    if (fLayout != NULL)
    {
        int  numItems = fLayout->count();

        if (index < (unsigned int)numItems)
        {
            unsigned int widgetIndex = 0;
            int          i;

            for (i = 0; i < numItems; ++i)
            {
                QLayoutItem* item = fLayout->itemAt(i);

                if (item && (result = item->widget()))
                {
                    if (widgetIndex == index) break;

                    ++widgetIndex;
                }
            }

            if (i >= numItems) result = NULL;
        }
    }

    return result;
}

// Description:  This procedure returns the number of rows in the group's
//               layout.
//
int KxWidgetGrp::numRows()
{
    Q_ASSERT((fRowColumnLayout != NULL && fRowLayout == NULL) || (fRowColumnLayout == NULL && fRowLayout != NULL));

    if(fIsVertical && fRowColumnLayout != NULL) {
        return fRowColumnLayout->numRows();
    } else if(fRowLayout != NULL) {
        return 1;
    } else {
        return -1;
    }
}

// Description:  This procedure returns the number of columns in the group's
//               layout.
//
int KxWidgetGrp::numColumns()
{
    Q_ASSERT((fRowColumnLayout != NULL && fRowLayout == NULL) || (fRowColumnLayout == NULL && fRowLayout != NULL));

    if(fIsVertical && fRowColumnLayout != NULL) {
        return fRowColumnLayout->numColumns();
    } else if(fRowLayout != NULL) {
        return fRowLayout->count();
    } else {
        return -1;
    }
}

// Description:  This procedure converts the layout of the group from a
//               horizontal row layout to a vertical row column layout (most of the
//               operations performed by the overrided version of this method
//               in the subclass).
//
void KxWidgetGrp::makeVertical()
{
    if(fRowColumnLayout != NULL) {
        fLayout = fRowColumnLayout;
        fIsVertical = true;

        setLayout(fLayout);
    }
}

// Description:  This procedure returns true if the group's layout is vertical,
//               false otherwise.
//
bool KxWidgetGrp::isVertical()
{
    return fIsVertical;
}

// Description:  This procedure sets the width of the specified column in the
//               group's layout, and whether or not the column is adjustable.
//
void KxWidgetGrp::setColumnWidth(int index, int width, bool adjustable)
{
    Q_ASSERT((fRowColumnLayout != NULL && fRowLayout == NULL) || (fRowColumnLayout == NULL && fRowLayout != NULL));

    if(fIsVertical && fRowColumnLayout != NULL) {
        fRowColumnLayout->setColumnWidth(index, width, adjustable);
    } else if(fRowLayout != NULL) {
        // Check if we are trying to set the column width for an extra label.
        // If we are, only apply the width if the label is non-empty. Otherwise,
        // set the width to 0, since we don't want a blank extra label to take up
        // any space.
        // NOTE: Extra labels are currently only available in widget groups that
        // can only be horizontally laid out. If extra labels are later made available
        // to widget groups that can be vertical, we will have to do this work in the
        // row column layout case above as well.
        if(fExtraLabel) {
            KxLabel *extraLabel = qobject_cast<KxLabel *>(fRowLayout->itemAt(index)->widget());
            if(extraLabel != fExtraLabel || !extraLabel->text().isEmpty()) {
                fRowLayout->setColumnWidth(index, width, adjustable);
            } else {
                fRowLayout->setColumnWidth(index, 0, adjustable);
            }
            // Save the width setting in case the label is set to a non-empty string later on
            fExtraLabelWidth = width;
        } else {
            fRowLayout->setColumnWidth(index, width, adjustable);
        }
    }
}

// Description:  This procedure sets the alignment of the specified column in the
//               group's layout
//
void KxWidgetGrp::setColumnAlignment(int index, Qt::Alignment alignment)
{
    Q_ASSERT((fRowColumnLayout != NULL && fRowLayout == NULL) || (fRowColumnLayout == NULL && fRowLayout != NULL));

    if(fIsVertical && fRowColumnLayout != NULL) {
        fRowColumnLayout->setColumnAlignment(index, alignment);
    } else if(fRowLayout != NULL) {
        fRowLayout->setColumnAlignment(index, alignment);
    }
}

// Description:  This procedure sets the horizontal attachments of the specified
//               column in the group's layout
//
void KxWidgetGrp::setColumnAttachment(int index, KxWidgetGrp::OffsetSide side, int offset)
{
    Q_ASSERT((fRowColumnLayout != NULL && fRowLayout == NULL) || (fRowColumnLayout == NULL && fRowLayout != NULL));

    if(fIsVertical && fRowColumnLayout != NULL) {
        if(side == kLeftSide) {
            fRowColumnLayout->setColumnAttachment(index, KxRowColumnLayout::kLeftSide, offset);
        } else if(side == kRightSide) {
            fRowColumnLayout->setColumnAttachment(index, KxRowColumnLayout::kRightSide, offset);
        } else if(side == kBothHSides) {
            fRowColumnLayout->setColumnAttachment(index, KxRowColumnLayout::kBothHSides, offset);
        }
    } else if(fRowLayout != NULL) {
        if(side == kLeftSide) {
            fRowLayout->setColumnAttachment(index, KxRowLayout::kLeftSide, offset);
        } else if(side == kRightSide) {
            fRowLayout->setColumnAttachment(index, KxRowLayout::kRightSide, offset);
        } else if(side == kBothHSides) {
            fRowLayout->setColumnAttachment(index, KxRowLayout::kBothHSides, offset);
        }
    }
}

// Description:  This procedure sets the vertical attachments of the specified column 
//               in the group's layout
//
void KxWidgetGrp::setRowAttachment(int index, KxWidgetGrp::OffsetSide side, int offset)
{
    Q_ASSERT((fRowColumnLayout != NULL && fRowLayout == NULL) || (fRowColumnLayout == NULL && fRowLayout != NULL));

    if(fIsVertical && fRowColumnLayout != NULL) {
        if(side == kTopSide) {
            fRowColumnLayout->setRowAttachment(index, KxRowColumnLayout::kTopSide, offset);
        } else if(side == kBottomSide) {
            fRowColumnLayout->setRowAttachment(index, KxRowColumnLayout::kBottomSide, offset);
        } else if(side == kBothVSides) {
            fRowColumnLayout->setRowAttachment(index, KxRowColumnLayout::kBothVSides, offset);
        }
    } else if(fRowLayout != NULL) {
        if(side == kTopSide) {
            fRowLayout->setRowAttachment(index, KxRowLayout::kTopSide, offset);
        } else if(side == kBottomSide) {
            fRowLayout->setRowAttachment(index, KxRowLayout::kBottomSide, offset);
        } else if(side == kBothVSides) {
            fRowLayout->setRowAttachment(index, KxRowLayout::kBothVSides, offset);
        }
    }
}

// Description:  This procedure set the specified column in the group's
//               layout to have an adjustable width (only for non-vertical layouts)
//
void KxWidgetGrp::setColumnAdjustable(int index)
{
    Q_ASSERT((fRowColumnLayout != NULL && fRowLayout == NULL) || (fRowColumnLayout == NULL && fRowLayout != NULL));

    if(!fIsVertical && fRowLayout != NULL) {
        fRowLayout->setColumnAdjustable(index);
    }
}
