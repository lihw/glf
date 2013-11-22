#include "KxRowColumnLayout.h"
#include "../base/KxQtHelper.h"

using namespace std;

//---------------------------------------------------
// Class: KxRowColumnLayout
// Description:  This class implements the Kx rowColumnLayout.
//---------------------------------------------------

// Description:  Constructor.
//
KxRowColumnLayout::KxRowColumnLayout( 
    QWidget *parent,
    ConstraintMode mode,
    int numberOfRowsOrColumns)
    : QLayout( parent )
    , fAllColumnSpacing ( 0 )
    , fAllRowSpacing ( 0 )
    , fConstraintMode ( mode )
    , fFixedNumberOfRowsOrColumns ( numberOfRowsOrColumns ) 
{
    setContentsMargins(1,1,1,1);
    setProperty("hasMarginsSet", qVariantFromValue(true));

    if(fConstraintMode == kFixNumberOfColumns) {
        for(int i = 0; i < fFixedNumberOfRowsOrColumns; i++) {
            fColumnSpacing[i] = 0;
        }
    } else {
        for(int i = 0; i < fFixedNumberOfRowsOrColumns; i++) {
            fRowSpacing[i] = 0;
        }
    }
}

// Description:  This procedure is called when we try to add
//               an item to the layout.
//
void KxRowColumnLayout::addItem(QLayoutItem *item)
{
    if (item == NULL)
    {
        return;
    }

    LayoutItemProperties properties;

    // Calculate the new item's position in the major and minor 
    // cooridnate.
    //
    int minorDimensionIndex = count()/ fFixedNumberOfRowsOrColumns;
    int majorDimensionIndex = count() % fFixedNumberOfRowsOrColumns; 

    properties.item = item;

    if (fConstraintMode == kFixNumberOfColumns)
    {		
        properties.row = minorDimensionIndex;
        properties.column = majorDimensionIndex;
    }
    else
    {
        properties.row = majorDimensionIndex;
        properties.column = minorDimensionIndex;
    }

    fLayoutItems.append(properties);

    applyColumnAlignment(properties.column);
    applyRowAlignment(properties.row);
}

// Description:  This procedure returns the layout item located at the specified
//               index. If there is no item, NULL is returned.
//
QLayoutItem * KxRowColumnLayout::itemAt(int index) const
{
    if(index >= 0 && index < fLayoutItems.count()) {
        return fLayoutItems[index].item;
    } else {
        return NULL;
    }
}

// Description:  This procedure removes the layout item at the specified index
//               from the layout and returns it. If there is no item, NULL is returned.
//
QLayoutItem * KxRowColumnLayout::takeAt(int index)
{
    if(index >= 0 && index < fLayoutItems.count()) {
        QLayoutItem * deletedItem = fLayoutItems.takeAt(index).item;	

        // Take out all items after the deleted one so that we can reset their positions in the layout
        QList<QLayoutItem *> shiftItems;
        while(itemAt(index) != NULL) {
            shiftItems.append(fLayoutItems.takeAt(index).item);
        }

        // Put the items back into the layout
        while(!shiftItems.empty()) {
            addItem(shiftItems.takeFirst());
        }

        invalidate();

        return deletedItem;
    } else {
        return NULL;
    }
}

// Description:  This procedure returns the number of items in the layout.
//
int KxRowColumnLayout::count() const
{
    return fLayoutItems.count();
}

// Description:  This procedure returns the minimum size of the layout,
//               which we want to be the size hint.
//
QSize KxRowColumnLayout::minimumSize() const
{
    return sizeHint();
}

// Description:  This procedure sets the minimum width of the specified column,
//               as well as whether or not the column can be resized. If the column is
//	             not adjustable, the specified width is the fixed width.
//
void KxRowColumnLayout::setColumnWidth(int column, int width, bool adjustable)
{
    if(width < 0) {
        width = 0;
    }

    ColumnWidthProperties properties;
    if(fColumnWidths.contains(column)) {
        properties = fColumnWidths[column];
        properties.width = width;
    } else {
        properties.width = width;
        properties.isAdjustable = adjustable;
    }

    fColumnWidths[column] = properties;
    if(column < numColumns()) {
        invalidate();
    }
}

// Description:  This procedure sets the minimum height of the specified
//               row, as well as whether or not the row can be resized.
//
void KxRowColumnLayout::setRowHeight(int row, int height)
{
    fSetRowHeights[row] = height;
    if(row < numRows()) {
        invalidate();
    }
}

// Description:  This procedure saves the given alignment for the specified column 
//               and applies it if column is non-empty.
//
void KxRowColumnLayout::setColumnAlignment(int column, Qt::Alignment alignment)
{
    fColumnAlignments[column] = alignment;
    if(column < numColumns()) {
        applyColumnAlignment(column);
    }
}

// Description:  This procedure saves the given alignment for the specified row 
//               and applies it if row is non-empty.
//
void KxRowColumnLayout::setRowAlignment(int row, Qt::Alignment alignment)
{
    fRowAlignments[row] = alignment;
    if(row < numRows()) {
        applyRowAlignment(row);
    }
}

// Description:  This procedure sets the horizontal offsets of the specified 
//               column (left, right, or both).
// 
void KxRowColumnLayout::setColumnAttachment(int column, KxRowColumnLayout::OffsetSide side, int offset)
{
    ColumnAttachProperties properties;

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
            break;
    }

    fColumnOffsets[column] = properties;
    if(column < numColumns()) {
        invalidate();
    }
}

// Description:  This procedure sets the offsets of the specified row (top, bottom, or both).
// 
void KxRowColumnLayout::setRowAttachment(int row, KxRowColumnLayout::OffsetSide side, int offset)
{
    RowAttachProperties properties;

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
            properties.topAttach = false;
            properties.topOffset = 0;
            properties.bottomAttach = false;
            properties.bottomOffset = 0;
            break;
    }

    fRowOffsets[row] = properties;
    if(row < numRows()) {
        invalidate();
    }
}

// Description:  This procedure sets the layout's geometry according to the
//               specified rectangle.
//
void KxRowColumnLayout::setGeometry(const QRect &rect)
{
    QLayout::setGeometry(rect);

    applyWidthsAndHeights();
}

// Description:  This procedure calculates and returns the preferred size of
//               the layout, which should be based on the minimum widths and heights
//               that were set.
//
QSize KxRowColumnLayout::sizeHint() const
{
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;

    getContentsMargins(&left, &top, &right, &bottom);

    int totalColumnSpacing = 0;
    int totalRowSpacing = 0;

    if(fConstraintMode == kFixNumberOfColumns) {
        for(int i = 0; i < fFixedNumberOfRowsOrColumns; i++) {
            totalColumnSpacing += fColumnSpacing[i];
        }

        int rows = numRows();
        if(fAllRowSpacing > 0 && rows > 1) {
            totalRowSpacing += fAllRowSpacing * (rows - 1);
        }
    } else {
        for(int i = 0; i < fFixedNumberOfRowsOrColumns; i++) {
            totalRowSpacing += fRowSpacing[i];
        }

        int columns = numColumns();
        if(fAllColumnSpacing > 0 && columns > 1) {
            totalColumnSpacing += fAllColumnSpacing * (columns - 1);
        }
    }

    int totalWidth = left + totalColumnSpacing + right;
    int totalHeight = top + totalRowSpacing + bottom;

    for(int i = 0 ; i < numColumns(); i++) {
        // Add the column's width to the total width
        if(fColumnWidths.contains(i) && fColumnWidths[i].width >= 0) {			
            totalWidth += fColumnWidths[i].width;
        } else {
            // Column has no set minimum width, so use find biggest widget's size hint to get appropriate width
            int maxWidth = 0;

            for(int j = 0; j < count(); j++) {
                LayoutItemProperties properties = fLayoutItems[j];
                if(properties.column == i) {
                    QLayoutItem *item = properties.item;
                    QWidget *w = NULL;

                    if(item && (w = item->widget())) {
                        // We want to give the widget at least its minimum width
                        int widgetWidth = KxQtHelper::getBaseControlWidth(w);
                        if(widgetWidth > maxWidth) {
                            maxWidth = widgetWidth;
                        }
                    }
                }
            }

            totalWidth += maxWidth;

            // Add the column's offsets to the total width
            if(fColumnOffsets.contains(i)) {			
                totalWidth += (fColumnOffsets[i].leftOffset + fColumnOffsets[i].rightOffset);
            }
        }
    }

    for(int i = 0 ; i < numRows(); i++) {
        // Add the row's height to the total height
        if(fSetRowHeights.contains(i) && fSetRowHeights[i] >= 0) {			
            totalHeight += fSetRowHeights[i];
        } else {
            // Row has no set minimum height, so use biggest widget's size hint to get appropriate height
            int maxHeight = 0;

            for(int j = 0; j < count(); j++) {
                LayoutItemProperties properties = fLayoutItems[j];
                if(properties.row == i) {
                    QLayoutItem *item = properties.item;
                    QWidget *w = NULL;

                    if(item && (w = item->widget())) {
                        // We want to give the widget at least its minimum height
                        int widgetHeight = KxQtHelper::getBaseControlHeight(w);
                        if(widgetHeight > maxHeight) {
                            maxHeight = widgetHeight;
                        }
                    }
                }
            }

            totalHeight += maxHeight;

            // Add the row's offsets to the total height
            if(fRowOffsets.contains(i)) {			
                totalHeight += (fRowOffsets[i].topOffset + fRowOffsets[i].bottomOffset);
            }
        }
    }

    return QSize(totalWidth, totalHeight);
}

// Description:  This procedure sets the spacing between rows.
//
void KxRowColumnLayout::setColumnSpacing(int column, int spacing)
{
    if(fConstraintMode == kFixNumberOfColumns) {
        fColumnSpacing[column] = spacing;
        if(column < numColumns()) {
            invalidate();
        }
    } else {
        fAllColumnSpacing = spacing;
        invalidate();
    }
}

// Description:  This procedure sets the spacing between rows.
//
void KxRowColumnLayout::setRowSpacing(int row, int spacing)
{
    if(fConstraintMode == kFixNumberOfRows) {
        fRowSpacing[row] = spacing;
        if(row < numRows()) {
            invalidate();
        }
    } else {
        fAllRowSpacing = spacing;
        invalidate();
    }
}

// Description:  This procedure returns true if the layout is in fixed
//               row format, false otherwise.
//
bool KxRowColumnLayout::isHorizontal()
{
    if (fConstraintMode == kFixNumberOfRows) {
        return true;
    } else {
        return false;
    }
}

// Description:  This procedure returns true if the layout is in fixed
//               column format, false otherwise.
//
bool KxRowColumnLayout::isVertical()
{
    if (fConstraintMode == kFixNumberOfColumns) {
        return true;
    } else {
        return false;
    }
}

// Description:  This procedure returns the number of columns in
//               the layout.
//
int KxRowColumnLayout::numRows() const
{
    if (fConstraintMode == kFixNumberOfColumns) {
        return (int)ceil((double)count()/(double)fFixedNumberOfRowsOrColumns);		
    } else {
        return fFixedNumberOfRowsOrColumns;
    }
}

// Description:  This procedure returns the number of columns in
//               the layout.
//
int KxRowColumnLayout::numColumns() const
{
    if (fConstraintMode == kFixNumberOfRows) {
        return (int)ceil((double)count()/(double)fFixedNumberOfRowsOrColumns);		
    } else {
        return fFixedNumberOfRowsOrColumns;
    }
}

// Description:  This procedure returns the number of rows specified 
//               when the layout is created.
// 
int KxRowColumnLayout::numberOfRowsSpecifiedByUser()
{
    if (fConstraintMode == kFixNumberOfRows)
    {
        return fFixedNumberOfRowsOrColumns;
    }

    // Otherwise, return 0.  The user did not specify the number
    // of rows.
    //
    return 0;
}

// Description:  This procedure returns the number of columns specified
//               when the layout is created.
//
int KxRowColumnLayout::numberOfColumnsSpecifiedByUser()
{
    if (fConstraintMode == kFixNumberOfColumns)
    {
        return fFixedNumberOfRowsOrColumns;
    }

    // Otherwise, return 0.  The user did not specify the number
    // of columns.
    //
    return 0;
}

// Description:  This procedure calculates the default widths of every column
//               that does not already have a user-set width (this is based
//               on the largest widget width in that column, since we want to
//               fit at least the widest widget).
//
void KxRowColumnLayout::calculateDefaultWidths()
{
    // Clear the previously saved defaults
    fDefaultColumnWidths.clear();

    for(int i = 0 ; i < numColumns(); i++) {
        if(!fColumnWidths.contains(i)) {			
            // Column has no set minimum width, so use find biggest widget's size hint to get appropriate width
            int maxWidth = 0;

            for(int j = 0; j < count(); j++) {
                LayoutItemProperties properties = fLayoutItems[j];
                if(properties.column == i) {
                    QLayoutItem *item = properties.item;
                    QWidget *w = NULL;

                    if(item && (w = item->widget())) {
                        // We want to give the widget at least its minimum width
                        int widgetWidth = KxQtHelper::getBaseControlWidth(w);
                        if(widgetWidth > maxWidth) {
                            maxWidth = widgetWidth;
                        }
                    }
                }
            }

            fDefaultColumnWidths.insert(i, maxWidth);
        }
    }
}

// Description:  This procedure calculates the default heights of every row
//               that does not already have a user-set height (this is based
//               on the largest widget height in that row, since we want to
//               fit at least the tallest widget).
//
void KxRowColumnLayout::calculateDefaultHeights()
{
    // Clear the previously saved defaults
    fDefaultRowHeights.clear();

    for(int i = 0 ; i < numRows(); i++) {
        if(!fSetRowHeights.contains(i)) {			
            // Row has no set minimum height, so use biggest widget's size hint to get appropriate height
            int maxHeight = 0;

            for(int j = 0; j < count(); j++) {
                LayoutItemProperties properties = fLayoutItems[j];
                if(properties.row == i) {
                    QLayoutItem *item = properties.item;
                    QWidget *w = NULL;

                    if(item && (w = item->widget())) {
                        // We want to give the widget at least its minimum height
                        int widgetHeight = KxQtHelper::getBaseControlHeight(w);
                        if(widgetHeight > maxHeight) {
                            maxHeight = widgetHeight;
                        }
                    }
                }
            }

            fDefaultRowHeights.insert(i, maxHeight);
        }
    }
}

// Description:  This procedure calculates and sets the geometry of each
//               item in the layout, based on its set position and minimum widths
//               and heights.
//
void KxRowColumnLayout::applyWidthsAndHeights()
{
    int left = 0;
    int right = 0;
    int top = 0;
    int bottom = 0;

    getContentsMargins(&left, &top, &right, &bottom);

    QRect rect = geometry();

    // Calculate the total width available for the widgets
    int totalColumnSpacing = 0;
    if (fConstraintMode == kFixNumberOfColumns) {
        for(int i = 0; i < fFixedNumberOfRowsOrColumns; i++) {
            totalColumnSpacing += fColumnSpacing[i];
        }
    } else {
        int columns = numColumns();
        if(fAllColumnSpacing > 0 && columns > 1) {
            totalColumnSpacing += fAllColumnSpacing * (columns - 1);
        }
    }
    int itemsWidth = rect.width() - left - totalColumnSpacing - right;

    int nonAdjColsWidth = 0;
    int numAdjCols = 0;

    int totalColumnOffsets = 0;

    // Recalculate the default column widths, in case the width of a child changed
    calculateDefaultWidths();

    // Calculate the total width of non-adjustable columns and the number of adjustable columns,  as well as the total column offset space needed
    for(int i = 0; i < numColumns(); i++) {
        int itemOffset = 0;
        if(fColumnOffsets.contains(i)) {			
            // Get the column's horizontal offsets
            itemOffset = (fColumnOffsets[i].leftOffset + fColumnOffsets[i].rightOffset);
        }

        if(fColumnWidths.contains(i)) {
            if(!fColumnWidths[i].isAdjustable) {
                // Use any set column width if the column is non-adjustable
                nonAdjColsWidth += fColumnWidths[i].width;
            } else {
                numAdjCols++;

                // The adjustable column width should not include the horizontal offsets, so take them into account
                totalColumnOffsets += itemOffset;
            }
        } else if(fDefaultColumnWidths.contains(i)) {
            // Using the default column width if none is set, so column is non-adjustable
            nonAdjColsWidth += fDefaultColumnWidths[i];

            // The default column width should not include the horizontal offsets, so take them into account
            totalColumnOffsets += itemOffset;
        } else {
            // Shouldn't get here, but just in case, set the column to be adjustable
            numAdjCols++;

            // The adjustable column width should not include the horizontal offsets, so take them into account
            totalColumnOffsets += itemOffset;
        }
    }

    // The adjustable columns equally share the rest of the width not taken by the non-adjustable columns or the offsets
    int adjColWidth = 0;

    if(numAdjCols > 0) {
        adjColWidth = (itemsWidth - nonAdjColsWidth - totalColumnOffsets)/numAdjCols;
    }

    // Recalculate the default row heights, in case the height of a child changed
    calculateDefaultHeights();

    // Coordinates of the first layout item
    int rowCount = 0;
    int columnCount = 0;

    bool leftAttach = false;
    bool rightAttach = false;
    int leftOffset = 0;
    int rightOffset = 0;

    bool topAttach = false;
    bool bottomAttach = false;
    int topOffset = 0;
    int bottomOffset = 0;

    int currX = rect.x() + left;
    int currY = rect.y() + top;

    if (fConstraintMode == kFixNumberOfColumns) {
        currX += fColumnSpacing[0];
    } else {
        currY += fRowSpacing[0];
    }

    // Include the left offset in the current X-coordinate calculation and the top offset in the current Y-coordinate calculation, if they exist
    if(fColumnOffsets.contains(columnCount)) {
        // Grab the left and right attachments while we're here
        leftAttach = fColumnOffsets[columnCount].leftAttach;
        rightAttach = fColumnOffsets[columnCount].rightAttach;

        leftOffset = fColumnOffsets[columnCount].leftOffset;
        rightOffset = fColumnOffsets[columnCount].rightOffset;
        currX += leftOffset;
    }

    if(fRowOffsets.contains(rowCount)) {
        // Grab the top and bottom attachments while we're here
        topAttach = fRowOffsets[rowCount].topAttach;
        bottomAttach = fRowOffsets[rowCount].bottomAttach;

        topOffset = fRowOffsets[rowCount].topOffset;
        bottomOffset = fRowOffsets[rowCount].bottomOffset;
        currY += topOffset;
    }

    for(int i = 0; i < count(); i++) {
        QLayoutItem *item = itemAt(i);
        QWidget *w = NULL;
        if(item) {
            w = item->widget();
        }

        int columnWidth = 0;
        int itemWidth = 0;
        int rowHeight = 0;
        int itemHeight = 0;
        int realY = currY;

        if(w != NULL) {
            itemWidth = KxQtHelper::getBaseControlWidth(w);
            itemHeight = KxQtHelper::getBaseControlHeight(w);
        }

        if(fColumnWidths.contains(columnCount)) {
            // Use the set column width unless the column is adjustable
            if(!fColumnWidths[columnCount].isAdjustable) {
                // Subtract the offsets to figure out the actual available space
                columnWidth = fColumnWidths[columnCount].width - leftOffset - rightOffset;
            } else {
                columnWidth = adjColWidth;
                itemWidth = columnWidth;
            }
        } else if(fDefaultColumnWidths.contains(columnCount)) {
            // Use the default column width
            columnWidth = fDefaultColumnWidths[columnCount];
        } else {
            // Shouldn't get here, but just in case, use the adjustable column width
            columnWidth = adjColWidth;
            itemWidth = columnWidth;
        }

        // Make sure the width we're going to give the widget
        // isn't bigger than the column space we have for it.
        if(itemWidth >= columnWidth) {
            itemWidth = columnWidth;
        } else {
            // If the widget width is smaller than the column space
            // then we need to determine through the attachments
            // where within the space the widget should be placed 
            // and how much of the space it will actually take.
            if(leftAttach && rightAttach) {
                itemWidth = columnWidth;
            } else if(rightAttach) {
                currX += (columnWidth - itemWidth);

                // Since we increased currX, the next X-coordinate is actually
                // itemWidth away (plus right offset and spacing), rather than 
                // columnWidth. Change the value of columnWidth to itemWidth
                // so that the calculation of the next X-coordinate further down in
                // this method will be correct.
                columnWidth = itemWidth;
            } else if(!leftAttach) {
                // Default attachment is both
                itemWidth = columnWidth;
            }
        }

        if(fSetRowHeights.contains(rowCount)) {
            // Use any set row height (minus offsets to figure out the actual available space)
            rowHeight = fSetRowHeights[rowCount] - topOffset - bottomOffset;
        } else if(fDefaultRowHeights.contains(rowCount)) {
            // Set the default row height
            rowHeight = fDefaultRowHeights[rowCount];
        } else {
            // Shouldn't get here, but just in case, use the widget's height as the row height
            rowHeight = itemHeight;
        }

        // Make sure the height we're going to give the widget
        // isn't bigger than the row space we have for it.
        if(itemHeight >= rowHeight) {
            itemHeight = rowHeight;
        } else {
            // If the widget height is smaller than the row space
            // then we need to determine through the attachments
            // where within the space the widget should be placed 
            // and how much of the space it will actually take.
            if(topAttach && bottomAttach) {
                itemHeight = rowHeight;
            } else if(bottomAttach) {
                // Calculate the appropriate vertical starting point for the widget so that it is bottom-attached
                realY += (rowHeight - itemHeight);
            } else if(!topAttach) {
                // Default attachment is both
                itemHeight = rowHeight;
            }
        }

        if(item && w) {				
            QRect itemRect(currX, realY, itemWidth, itemHeight); 
            w->setGeometry(itemRect);
        }

        // Obtain the coordinates for the next layout item
        if (fConstraintMode == kFixNumberOfRows) {
            // Check if we are on the last row - must move to first row, next column
            if(rowCount == fFixedNumberOfRowsOrColumns - 1) {
                // Save the last column's right offset so we can include it in the current X-coordinate calculation
                int prevRightOffset = rightOffset;

                rowCount = 0;
                columnCount++;

                leftAttach = false;
                rightAttach = false;

                leftOffset = 0;
                rightOffset = 0;

                topAttach = false;
                bottomAttach = false;

                topOffset = 0;
                bottomOffset = 0;

                // Get the attachments and offsets for the next column
                if(fColumnOffsets.contains(columnCount)) {
                    leftAttach = fColumnOffsets[columnCount].leftAttach;
                    rightAttach = fColumnOffsets[columnCount].rightAttach;

                    leftOffset = fColumnOffsets[columnCount].leftOffset;
                    rightOffset = fColumnOffsets[columnCount].rightOffset;
                }

                // Get the attachments and offsets for the first row
                if(fRowOffsets.contains(rowCount)) {
                    topAttach = fRowOffsets[rowCount].topAttach;
                    bottomAttach = fRowOffsets[rowCount].bottomAttach;

                    topOffset = fRowOffsets[rowCount].topOffset;
                    bottomOffset = fRowOffsets[rowCount].bottomOffset;
                }

                currX += (columnWidth + prevRightOffset + fAllColumnSpacing + leftOffset);
                currY = rect.y() + top + topOffset + fRowSpacing[rowCount];
            } else {
                // Save the last row's bottom offset so we can include it in the current Y-coordinate calculation
                int prevBottomOffset = bottomOffset;

                rowCount++;

                topAttach = false;
                bottomAttach = false;

                topOffset = 0;
                bottomOffset = 0;

                // Get the attachments and offsets for the next row
                if(fRowOffsets.contains(rowCount)) {
                    topAttach = fRowOffsets[rowCount].topAttach;
                    bottomAttach = fRowOffsets[rowCount].bottomAttach;

                    topOffset = fRowOffsets[rowCount].topOffset;
                    bottomOffset = fRowOffsets[rowCount].bottomOffset;
                }

                currY += (rowHeight + prevBottomOffset + topOffset + fRowSpacing[rowCount]);
            }
        } else {
            // Check if we are on the last column - must move to first column, next row
            if(columnCount == fFixedNumberOfRowsOrColumns - 1) {
                // Save the last row's bottom offset so we can include it in the current Y-coordinate calculation
                int prevBottomOffset = bottomOffset;

                columnCount = 0;
                rowCount++;

                leftAttach = false;
                rightAttach = false;

                leftOffset = 0;
                rightOffset = 0;

                topAttach = false;
                bottomAttach = false;

                topOffset = 0;
                bottomOffset = 0;

                // Get the attachments and offsets for the first column
                if(fColumnOffsets.contains(columnCount)) {
                    leftAttach = fColumnOffsets[columnCount].leftAttach;
                    rightAttach = fColumnOffsets[columnCount].rightAttach;

                    leftOffset = fColumnOffsets[columnCount].leftOffset;
                    rightOffset = fColumnOffsets[columnCount].rightOffset;
                }

                // Get the attachments and offsets for the next row
                if(fRowOffsets.contains(rowCount)) {
                    topAttach = fRowOffsets[rowCount].topAttach;
                    bottomAttach = fRowOffsets[rowCount].bottomAttach;

                    topOffset = fRowOffsets[rowCount].topOffset;
                    bottomOffset = fRowOffsets[rowCount].bottomOffset;
                }

                currX = rect.x() + left + leftOffset + fColumnSpacing[columnCount];
                currY += (rowHeight + prevBottomOffset + fAllRowSpacing + topOffset);
            } else {
                // Save the last column's right offset so we can include it in the current X-coordinate calculation
                int prevRightOffset = rightOffset;

                columnCount++;

                leftAttach = false;
                rightAttach = false;

                leftOffset = 0;
                rightOffset = 0;

                // Get the attachments and offsets for the next column
                if(fColumnOffsets.contains(columnCount)) {
                    leftAttach = fColumnOffsets[columnCount].leftAttach;
                    rightAttach = fColumnOffsets[columnCount].rightAttach;

                    leftOffset = fColumnOffsets[columnCount].leftOffset;
                    rightOffset = fColumnOffsets[columnCount].rightOffset;
                }

                currX += (columnWidth + prevRightOffset + leftOffset + fColumnSpacing[columnCount]);
            }
        }
    }
}

// Description:  This procedure sets the alignment for the specified column if there 
//               was an alignment declared.
//
void KxRowColumnLayout::applyColumnAlignment(int column)
{
    if(fColumnAlignments.contains(column)) {
        for(int i = 0; i < count(); i++) {
            LayoutItemProperties properties = fLayoutItems[i];
            if(properties.column == column) {
                QLayoutItem* item = properties.item;
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
    }
}

// Description:  This procedure sets the alignment for the specified row if there 
//               was an alignment declared.
//
void KxRowColumnLayout::applyRowAlignment(int row)
{
    if(fRowAlignments.contains(row)) {
        for(int i = 0; i < count(); i++) {
            LayoutItemProperties properties = fLayoutItems[i];
            if(properties.row == row) {
                QLayoutItem* item = properties.item;
                QWidget *widget = NULL;
                if (item && (widget = item->widget())) {
                    QLabel *label = dynamic_cast<QLabel *>(widget);
                    if(label != NULL) {
                        label->setAlignment(fRowAlignments[row]);
                    } else {
                        setAlignment(widget, fRowAlignments[row]);
                    }
                }
            }
        }
    }
}
