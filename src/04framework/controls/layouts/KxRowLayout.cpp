#include "KxRowLayout.h"

#include "../base/KxQtHelper.h"

using namespace  std;
// Class: KxRowLayout
// Description:  This class implements the Kx rowLayout.
//---------------------------------------------------

// Description:  Constructor.
//
KxRowLayout::KxRowLayout( QWidget * parent )
: QHBoxLayout(parent)
, fNumColumns(0)
, fAdjustableColumn(-1)
, fAutoDistributeWidths(false)
{
    setContentsMargins(1,1,1,1);
    setSpacing(2);
    setProperty("hasMarginsSet", qVariantFromValue(true));
}


// Description:  Set fAutoDistributeWidths boolean. If set to true, when the layout is resized,
//               the new widths of the widgets will be calculated using the set widths as proportions
//               of the new rectangle. Otherwise, will follow regular sizing method.
//
void KxRowLayout::setAutoDistributeWidths(bool autoDistribute)
{
    fAutoDistributeWidths = autoDistribute;
}

// Description:  Sets the column that will be resized when the layout is resized.
//
void KxRowLayout::setColumnAdjustable(int column)
{
    if(fAdjustableColumn != column) {
        fAdjustableColumn = column;
    }
}

// Description:  Sets the width and adjustability of the specified column.
//
void KxRowLayout::setColumnWidth(int column, int width, bool adjustable)
{
    ColumnWidthProperties properties;
    if(fColumnWidths.contains(column)) {
        properties = fColumnWidths[column];
        properties.width = width;
    } else {
        properties.width = width;
        properties.isAdjustable = adjustable;
    }

    fColumnWidths[column] = properties;
    if(column < count()) {
        invalidate();
    }
}

// Description:  Saves the given alignment for the specified column and applies it if column is non-empty.
//
void KxRowLayout::setColumnAlignment(int column, Qt::Alignment alignment)
{
    fColumnAlignments[column] = alignment;
    if(column < count()) {
        applyColumnAlignment(column);
    }
}

// Description:  Sets the attachment and offset for one side of the specified column.
//
void KxRowLayout::setColumnAttachment(int column, KxRowLayout::OffsetSide side, int offset)
{
    ColumnAttachProperties properties;
    if(fColumnOffsets.contains(column)) {
        properties = fColumnOffsets[column];

        switch(side) {
            case kLeftSide:
                properties.leftAttach = true;
                properties.leftOffset = offset;
                properties.rightAttach = false;
                properties.rightOffset = 0;
                break;

            case kRightSide:
                properties.rightAttach = true;
                properties.rightOffset = offset;
                properties.leftAttach = false;
                properties.leftOffset = 0;
                break;

            case kBothHSides:
                properties.leftAttach = true;
                properties.leftOffset = offset;
                properties.rightAttach = true;
                properties.rightOffset = offset;
                break;

            default:
                properties.leftAttach = false;
                properties.leftOffset = 0;
                properties.rightAttach = false;
                properties.rightOffset = 0;
                properties.topAttach = false;
                properties.topOffset = 0;
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;
        }
    } else {	
        switch(side) {
            case kLeftSide:
                properties.leftAttach = true;
                properties.leftOffset = offset;
                properties.rightAttach = false;
                properties.rightOffset = 0;
                properties.topAttach = false;
                properties.topOffset = 0;
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;

            case kRightSide:
                properties.rightAttach = true;
                properties.rightOffset = offset;
                properties.leftAttach = false;
                properties.leftOffset = 0;
                properties.topAttach = false;
                properties.topOffset = 0;
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;

            case kBothHSides:
                properties.leftAttach = true;
                properties.leftOffset = offset;
                properties.rightAttach = true;
                properties.rightOffset = offset;
                properties.topAttach = false;
                properties.topOffset = 0;
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;

            default:
                properties.leftAttach = false;
                properties.leftOffset = 0;
                properties.rightAttach = false;
                properties.rightOffset = 0;
                properties.topAttach = false;
                properties.topOffset = 0;
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;
        }
    }

    fColumnOffsets[column] = properties;
    if(column < count()) {
        invalidate();
    }
}

// Description:  Sets the attachment and offset for one side of the specified column.
//
void KxRowLayout::setRowAttachment(int column, KxRowLayout::OffsetSide side, int offset)
{
    ColumnAttachProperties properties;
    if(fColumnOffsets.contains(column)) {
        properties = fColumnOffsets[column];

        switch(side) {
            case kTopSide:
                properties.topAttach = true;
                properties.topOffset = offset;
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;

            case kBottomSide:
                properties.bottomAttach = true;
                properties.bottomOffset = offset;
                properties.topAttach = false;
                properties.topOffset = 0;
                break;

            case kBothVSides:
                properties.topAttach = true;
                properties.topOffset = offset;
                properties.bottomAttach = true;
                properties.bottomOffset = offset;
                break;

            default:
                properties.leftAttach = false;
                properties.leftOffset = 0;
                properties.rightAttach = false;
                properties.rightOffset = 0;
                properties.topAttach = false;
                properties.topOffset = 0;
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;
        }
    } else {	
        switch(side) {
            case kTopSide:
                properties.topAttach = true;
                properties.topOffset = offset;
                properties.leftAttach = false;
                properties.leftOffset = 0;
                properties.rightAttach = false;
                properties.rightOffset = 0;				
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;

            case kBottomSide:
                properties.bottomAttach = true;
                properties.bottomOffset = offset;
                properties.leftAttach = false;
                properties.leftOffset = 0;
                properties.rightAttach = false;
                properties.rightOffset = 0;
                properties.topAttach = false;
                properties.topOffset = 0;
                break;

            case kBothVSides:
                properties.topAttach = true;
                properties.topOffset = offset;
                properties.bottomAttach = true;
                properties.bottomOffset = offset;
                properties.leftAttach = false;
                properties.leftOffset = 0;
                properties.rightAttach = false;
                properties.rightOffset = 0;
                break;

            default:
                properties.leftAttach = false;
                properties.leftOffset = 0;
                properties.rightAttach = false;
                properties.rightOffset = 0;
                properties.topAttach = false;
                properties.topOffset = 0;
                properties.bottomAttach = false;
                properties.bottomOffset = 0;
                break;
        }
    }

    fColumnOffsets[column] = properties;
    if(column < count()) {
        invalidate();
    }
}

// Description:  Returns the minimum size of the layout, which we want to 
//               be the size hint.
//
QSize KxRowLayout::minimumSize() const
{
    return sizeHint();
}

// Description:  Sets the layout's geometry according to the specified rectangle.
//
void KxRowLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    applyColumnWidths();
}

// Description:  Calculates and returns the preferred size of the layout.
//
QSize KxRowLayout::sizeHint() const
{
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;

    getContentsMargins(&left, &top, &right, &bottom);

    int totalWidth = left + (count() - 1) * spacing() + right;
    int maxHeight = 0;
    for(int i = 0 ; i < count(); i++) {
        QLayoutItem *item = itemAt(i);
        QWidget *w = item->widget();

        if(w != NULL) {
            int itemHeight = KxQtHelper::getBaseControlHeight(w);
            int itemOffset = 0;

            // Look for the biggest widget height (including vertical offsets, if any)
            if(fColumnOffsets.contains(i)) {			
                // Get the column's horizontal offsets while we're here to use when calculating the width later
                itemOffset = (fColumnOffsets[i].leftOffset + fColumnOffsets[i].rightOffset);

                // Save the height of the widget plus the column's vertical offsets if it's the maximum height thus far
                if(maxHeight < (itemHeight + fColumnOffsets[i].topOffset + fColumnOffsets[i].bottomOffset)) {
                    maxHeight = itemHeight + fColumnOffsets[i].topOffset + fColumnOffsets[i].bottomOffset;
                }
            } else {
                // Save the height of the widget if it's the maximum height thus far
                if(maxHeight < itemHeight) {
                    maxHeight = itemHeight;
                }
            }

            int itemWidth = KxQtHelper::getBaseControlWidth(w) + itemOffset;

            if(fColumnWidths.contains(i)) {
                // We want to use any set width for the column as long as it fits the minimum size of the widget (including horizontal offsets, if any)
                if(fColumnWidths[i].width >= (w->minimumWidth() + itemOffset)) {
                    // If we are on the last column and the item is not right-attached, we take the itemWidth,
                    // since we don't need the extra space if the set column width is longer, and we don't want
                    // to risk truncation if it is shorter than the itemWidth, which should be the preferred
                    // width of the widget.
                    if(i != count() - 1 || (fColumnOffsets.contains(i) && fColumnOffsets[i].rightAttach)) {
                        itemWidth = fColumnWidths[i].width;
                    }
                } else {
                    // If the set column width can't fit the minimum size (plus offsets),
                    // set the item width to that minimum, since that's as low as we can go.
                    itemWidth = w->minimumWidth() + itemOffset;
                }
            }

            totalWidth += itemWidth;
        }
    }

    return QSize(totalWidth, top + maxHeight + bottom);
}

// Description:  Called when we try to add an item to the layout.
//
void KxRowLayout::addItem ( QLayoutItem * item )
{
    QHBoxLayout::addItem(item);
    applyColumnAlignment(count() - 1);
}

// Description:  Called when we try to add a widget to the layout.
//
void KxRowLayout::addWidget ( QWidget * w )
{
    QHBoxLayout::addWidget(w);
    applyColumnAlignment(count() - 1);
}

// Description:  Called when we try to insert an item in the layout
//               at the specified index.
//
void KxRowLayout::insertItem ( int index, QLayoutItem * item )
{
    QHBoxLayout::insertItem(index, item);
    applyColumnAlignment(index);
}

// Description:  Called when we try to insert a widget in the layout
//               at the specified index.
//
void KxRowLayout::insertWidget ( int index, QWidget * w )
{
    QHBoxLayout::insertWidget(index, w);
    applyColumnAlignment(index);
}

// Description:  Calculates and sets the geometry of each item in the layout,
//               based on its set position and minimum widths and heights.
//
void KxRowLayout::applyColumnWidths()
{
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;

    getContentsMargins(&left, &top, &right, &bottom);

    QRect rect = geometry();

    // Calculate the total width available for the widgets
    int itemsWidth = rect.width() - left - (count() - 1) * spacing() - right;

    if(!fAutoDistributeWidths) {
        QHash<int, int> actualColumnWidths;
        int nonAdjColsWidth = 0;
        int numAdjCols = 0;

        int totalOffsets = 0;

        // Calculate the total width of non-adjustable columns and the number of adjustable columns, as well as the total offset space needed
        for(int i = 0; i < count(); i++) {
            int itemOffset = 0;

            if(fColumnOffsets.contains(i)) {			
                // Get the column's horizontal offsets
                itemOffset = (fColumnOffsets[i].leftOffset + fColumnOffsets[i].rightOffset);
            }

            // Check whether or not this column is adjustable
            if(i == fAdjustableColumn || (fColumnWidths.contains(i) && fColumnWidths[i].isAdjustable)) {
                numAdjCols++;

                // The adjustable column width should not include the horizontal offsets, so take them into account
                totalOffsets += itemOffset;
            } else {
                QLayoutItem *item = itemAt(i);
                QWidget *w = NULL;
                int itemWidth = 0;
                if(item && (w = item->widget())) {
                    itemWidth = KxQtHelper::getBaseControlWidth(w) + itemOffset;

                    if(fColumnWidths.contains(i)) {
                        // We want to use any set width for the column as long as it fits the minimum size of the widget (including horizontal offsets, if any)
                        if(fColumnWidths[i].width >= (w->minimumWidth() + itemOffset)) {
                            // If we are on the last column and the item is not right-attached, we take the itemWidth,
                            // since we don't need the extra space if the set column width is longer, and we don't want
                            // to risk truncation if it is shorter than the itemWidth, which should be the preferred
                            // width of the widget.
                            if(i != count() - 1 || (fColumnOffsets.contains(i) && fColumnOffsets[i].rightAttach)) {
                                itemWidth = fColumnWidths[i].width;
                            }
                        } else {
                            // If the set column width can't fit the minimum size (plus offsets),
                            // set the item width to that minimum, since that's as low as we can go.
                            itemWidth = w->minimumWidth() + itemOffset;
                        }
                    }
                }

                // The horizontal offsets are included in itemWidth (they are part of the set column width, if used)
                nonAdjColsWidth += itemWidth;

                // Save the actual column space we have for the widget to use later
                actualColumnWidths[i] = itemWidth - itemOffset;
            }
        }

        // The adjustable columns equally share the rest of the width not taken by the non-adjustable columns or the offsets
        int adjColWidth = 0;

        if(numAdjCols > 0) {
            adjColWidth = (itemsWidth - nonAdjColsWidth - totalOffsets)/numAdjCols;
        }

        int currX = rect.x() + left;

        for(int i = 0; i < count(); i++) {
            QLayoutItem *item = itemAt(i);
            QWidget *w = NULL;
            if(item && (w = item->widget())) {
                int currY = rect.y() + top;
                int columnWidth = 0;
                int widgetWidth = KxQtHelper::getBaseControlWidth(w);

                // Include the left offset in the currrent X-coordinate calculation and
                // the top offset in the current Y-coordinate calculation, if they exist
                if(fColumnOffsets.contains(i)) {
                    currX += fColumnOffsets[i].leftOffset;
                    currY += fColumnOffsets[i].topOffset;
                }

                if(i == fAdjustableColumn || (fColumnWidths.contains(i) && fColumnWidths[i].isAdjustable)) {
                    columnWidth = adjColWidth;
                    widgetWidth = columnWidth;
                } else if(actualColumnWidths.contains(i)) {
                    columnWidth = actualColumnWidths[i];
                    // Make sure the width we're going to give the widget
                    // isn't bigger than the column space we have for it.
                    if(widgetWidth >= columnWidth) {
                        widgetWidth = columnWidth;
                    } else {
                        // If the widget width is smaller than the column space
                        // then we need to determine through the attachments
                        // where within the space the widget should be placed 
                        // and how much of the space it will actually take.
                        if(fColumnOffsets.contains(i)) {
                            if(fColumnOffsets[i].leftAttach && fColumnOffsets[i].rightAttach) {
                                widgetWidth = columnWidth;
                            } else if(fColumnOffsets[i].rightAttach) {
                                currX += (columnWidth - widgetWidth);

                                // Since we increased currX, the next X-coordinate is actually
                                // widgetWidth away (plus right offset and spacing), rather than 
                                // columnWidth. Change the value of columnWidth to widgetWidth
                                // so that the calculation of the next X-coordinate further down in
                                // this method will be correct.
                                columnWidth = widgetWidth;
                            }
                        }
                    }
                }					

                // We want to set the widget's height to at least its minimum height
                int widgetHeight = KxQtHelper::getBaseControlHeight(w);
                int availableHeight = rect.height() - top - bottom;
                bool hasRowAttachments = false;

                if(fColumnOffsets.contains(i)) {
                    availableHeight -= (fColumnOffsets[i].topOffset + fColumnOffsets[i].bottomOffset);
                    if(fColumnOffsets[i].topAttach && fColumnOffsets[i].bottomAttach) {
                        widgetHeight = availableHeight;
                        hasRowAttachments = true;
                    } else if(fColumnOffsets[i].bottomAttach) {
                        if(availableHeight > widgetHeight) {
                            currY += (availableHeight - widgetHeight);
                        }
                        hasRowAttachments = true;
                    } else if(fColumnOffsets[i].topAttach) {
                        // Don't have to change widgetHeight or currY in this case
                        // since they're already at the values we want.
                        hasRowAttachments = true;
                    }
                }

                // If there are no row attachments set, vertically center the widget in the available space.
                if(!hasRowAttachments) {
                    if(availableHeight > widgetHeight) {
                        // Calculate the appropriate vertical starting point so that the widget is vertically centered in the content rectangle
                        int extraSpace = (availableHeight - widgetHeight)/2;
                        currY += extraSpace;
                    }
                }

                QRect itemRect(currX, currY, widgetWidth, widgetHeight); 
                w->setGeometry(itemRect);

                // Include the right offset in the current X-coordinate calculation, if it exists
                if(fColumnOffsets.contains(i)) {
                    currX += (columnWidth + fColumnOffsets[i].rightOffset + spacing());
                } else {
                    currX += (columnWidth + spacing());
                }
            }
        }
    } else {
        //QT_TODO - Implement new functionality (was NOT in 2009) - Determine new widths by using the set widths as proportions of the new rectangle
    }
}

// Description:  Sets the alignment for the specified column if there was an alignment declared.
//
void KxRowLayout::applyColumnAlignment(int column)
{
    if(fColumnAlignments.contains(column)) {
        QLayoutItem* item = itemAt(column);
        QWidget *widget = NULL;
        if (item && (widget = item->widget())) {
            QLabel *label = dynamic_cast<QLabel *>(widget);
            if(label != NULL) {
                label->setAlignment(fColumnAlignments[column]);
            } else {
                setAlignment(widget, fColumnAlignments[column]);
            }
        }
    }
}

// Description:  Sets the maximum number of columns the layout can have.
//
void KxRowLayout::setNumColumns( int num )
{
    fNumColumns = num;
    setProperty("maxNumOfChildren", qVariantFromValue(fNumColumns));
}

// Description:  Returns the maximum number of columns the layout can have.
//
int KxRowLayout::numColumns()
{
    return fNumColumns;
}
