#include "KxGridLayout.h"
#include <math.h>	// For ceil()

#define kDefaultNumColumns 3
#define kDefaultNumRows 1

#define kDefaultCellWidth 32
#define kDefaultCellHeight 32

//---------------------------------------------------
// Class: KxGridLayout
// Description:  This class implements the Kx gridLayout using a flow layout implementation as a
//                        basis (see KxFlowLayout and QflowLayout in KxShelfLayout for examples).
//---------------------------------------------------

// Description:  Constructor.
//
KxGridLayout::KxGridLayout(QWidget *parent, bool columnsResize, bool allowEmptyCells, bool autoGrow)
: QLayout(parent)
, fColumnsResizable(columnsResize)
, fAllowEmptyCells(allowEmptyCells)
, fAutoGrow(autoGrow)
, fNumberOfColumns(kDefaultNumColumns)
, fNumberOfRows(kDefaultNumRows)
, fCellWidth(kDefaultCellWidth)
, fCellHeight(kDefaultCellHeight)
, fNumPositions(0)
{
}

// Description:  Constructor.
//
KxGridLayout::KxGridLayout(bool columnsResize, bool allowEmptyCells, bool autoGrow)
: QLayout()
, fColumnsResizable(columnsResize)
, fAllowEmptyCells(allowEmptyCells)
, fAutoGrow(autoGrow)
, fNumberOfColumns(kDefaultNumColumns)
, fNumberOfRows(kDefaultNumRows)
, fCellWidth(kDefaultCellWidth)
, fCellHeight(kDefaultCellHeight)
, fNumPositions(0)
{
}

// Description:  Destructor.
//
KxGridLayout::~KxGridLayout()
{
    QLayoutItem *item;
    while ((item = takeAt(0)))
        delete item;
}

// Description:  Add a new item into the layout.  Place it after
//		 the last child. 
//
void KxGridLayout::addItem(QLayoutItem *item)
{
	fItemList.append(item);

	if(fAllowEmptyCells && fNumPositions > 0) {
		// Search for the first empty space in the layout and put the item
		// in that spot.
		bool foundEmptySpace = false;
		for(int i = 0; i < fNumPositions; i++) {
			if(!fPositionToItem.contains(i)) {
				fPositionToItem[i] = item;
				fItemToPosition[item] = i;
				foundEmptySpace = true;
				break;
			}
		}

		// If no empty space is found, put the item in the last position.
		if(!foundEmptySpace) {
			fPositionToItem[fNumPositions] = item;
			fItemToPosition[item] = fNumPositions;
			fNumPositions++;
		}
	} else {
		// If we don't allow empty cells or we're inserting the first item,
		// the item will be put into the last position.
		fPositionToItem[fNumPositions] = item;
		fItemToPosition[item] = fNumPositions;
		fNumPositions++;
	}
	
	// If autoGrow is set to true, then the max number of rows
	// allowed will be the greatest of the user specified number
	// and the number of rows that the layout's items take up.
	if(fAutoGrow) {
		int newItemsRow = (int)ceil((double)(fItemToPosition[item] + 1)/ (double)fNumberOfColumns);
		if(fNumberOfRows < newItemsRow) {
			fNumberOfRows = newItemsRow;
		}
	}
}

// Description:  Returns the number of items in the layout.
//
int KxGridLayout::count() const
{
    return fItemList.size();
}

// Description:  Returns the layout item at the specified index.
//
QLayoutItem *KxGridLayout::itemAt(int index) const
{
    return fItemList.value(index);
}

// Description:  Removes the layout item at index from the layout, and returns it.
//
QLayoutItem *KxGridLayout::takeAt(int index)
{
	if (index >= 0 && index < fItemList.size()) {
		QLayoutItem * deletedItem = fItemList.takeAt(index);
		int position = -1;

		if(fItemToPosition.contains(deletedItem)) {
			position = fItemToPosition[deletedItem];
			fPositionToItem.remove(position);
			fItemToPosition.remove(deletedItem);
		}

		if(!fAllowEmptyCells) {
			// Take out all items after the deleted one so that we can reset their positions in the layout
			QList<QLayoutItem *> shiftItems;
			for(int i = position + 1; i < fNumPositions; i++) {
				if(fPositionToItem.contains(i)) {
					QLayoutItem *item = fPositionToItem.take(i);
					shiftItems.append(item);
					fItemToPosition.remove(item);
				}
			}

			// Put the items back into the layout and save their new positions
			fNumPositions = position;
			while(!shiftItems.empty()) {
				QLayoutItem *item = shiftItems.takeFirst();
				fPositionToItem[fNumPositions] = item;
				fItemToPosition[item] = fNumPositions;
				fNumPositions++;
			}

			invalidate();
		} else if(position == fNumPositions - 1) {
			// If we are deleting the last item in the layout, check if there are any empty
			// spaces before it so we can get rid of those too, since we don't want trailing
			// empty spaces to be counted in the min. size/size hint calculation.
			while(fNumPositions > 0 && !fPositionToItem.contains(fNumPositions - 1)) {
				fNumPositions--;
			}
		}

		return deletedItem;
	} else {
		return NULL;
	}
}

// Description:  Returns the direction in which the layout wants to grow.
//
Qt::Orientations KxGridLayout::expandingDirections() const
{
	return Qt::Horizontal;
}

// Description:  Returns true if the layout's preferred height depends on its width.
//
bool KxGridLayout::hasHeightForWidth() const
{
	if(fAutoGrow) {
		return true;
	} else {
		return false;
	}
}

// Description:  Calculates the preferred height for the layout, given the specified width.
//
int KxGridLayout::heightForWidth(int width) const
{
	QRect rect(0, 0, width, 0);
	int x = rect.x();
    int y = rect.y();
	int currentRow = 1;
	int currentColumn = 0;

	for(int i = 0; i < fNumPositions; i++) {	
		currentColumn++;
		int nextX = x + fCellWidth;

		// If columnsResizable is set to true, go to the next row if we can't fit the control within the parent's width.
		// If columnsResizable is set to false, go to the next row if we have reached the specified max number of columns.
		if((fColumnsResizable && nextX > rect.right() + 1) || (!fColumnsResizable && currentColumn > fNumberOfColumns)) {
			x = rect.x();
			y = y + fCellHeight;
			nextX = x + fCellWidth;
			currentRow++;
			currentColumn = 1;
		}

		x = nextX;
	}

	return y + fCellHeight - rect.y();
}

// Description:  Sets the layout's geometry according to the specified rectangle.
//
void KxGridLayout::setGeometry(const QRect &rect)
{
	QLayout::setGeometry(rect);
	doLayout(rect);
}

// Description:  Returns the size hint of the layout, which, depending on the
//                layout settings, is either based on the set number of columns/rows
//                or the current width of the layout.
//
QSize KxGridLayout::sizeHint() const
{
    int width = 0;
	int height = 0;
	
	if(fColumnsResizable) {
		width = parentWidget()->width();
	} else {
		width = fNumberOfColumns * fCellWidth;
	}

	if(fAutoGrow) {
		height = heightForWidth(width);
	} else {
		height = fNumberOfRows * fCellHeight;
	}

	return QSize(width, height);
}

// Description:  Returns the minimum size of the layout, which is based on 
//                the set number of columns and the number of rows that results
//                from that many columns.
//
QSize KxGridLayout::minimumSize() const
{
    int width = fNumberOfColumns * fCellWidth;
	int height = 0;

	if(fAutoGrow) {
		int numRows = (int)ceil((double)fNumPositions / (double)fNumberOfColumns);
		height = numRows * fCellHeight;
	} else {
		height = fNumberOfRows * fCellHeight;
	}

	return QSize(width, height);
}

// Description:  Set the general user suggested cell width.
//
void KxGridLayout::setCellWidth(int width)
{
	fCellWidth = width;
	invalidate();
}

// Description:  Returns the general user suggested cell width.
//
int KxGridLayout::cellWidth()
{
	return fCellWidth;
}

// Description:  Set the general user suggested cell height.
//
void KxGridLayout::setCellHeight(int height)
{
	fCellHeight = height;
	invalidate();
}

// Description:  Returns the general user suggested cell height.
//
int KxGridLayout::cellHeight()
{
	return fCellHeight;
}

// Description:  Set the number of rows.
//
void KxGridLayout::setNumberOfRows(int rows)
{
	fNumberOfRows = rows;
	invalidate();
}

// Description:  Returns the number of rows in the layout (may
//                be different than what the user set if autoGrow
//                is set to true).
//
int KxGridLayout::rowCount()
{
	if(fAutoGrow) {
		QRect rect = geometry();
		int height = heightForWidth(rect.width());
		return (height / fCellHeight);
	} else {
		return fNumberOfRows;
	}
}

// Description:  Set the number of columns.
//
void KxGridLayout::setNumberOfColumns(int columns)
{
	fNumberOfColumns = columns;
	invalidate();
}

// Description:  Returns the number of columns in the layout (may
//                be different than what the user set if columnsResizable
//                is set to true).
//
int KxGridLayout::columnCount()
{
	if(fColumnsResizable) {
		return (parentWidget()->width() / fCellWidth);
	} else {
		return fNumberOfColumns;
	}
}

// Description:  Returns true if the number of columns in the layout
//                is set to adjust to the width of the layout.
//
bool KxGridLayout::columnsResizable() const
{
	return fColumnsResizable;
}

// Description:  Returns true if controls can be freely positioned in the 
//                layout (potentially leaving empty spaces when deleting or
//                moving them).
//
bool KxGridLayout::allowEmptyCells() const
{
	return fAllowEmptyCells;
}

// Description:  Returns true if the size of the layout is set to grow as 
//                children are added (potentially changing the set number of
//                rows).
//
bool KxGridLayout::autoGrow() const
{
	return fAutoGrow;
}

// Description:  This procedure is used to list the content of 
//            cells in the grid layout in the order that they appear
//            in the grid.
//
//            For example, the grid layout with contents
//
//                 --- ---- --- --- 
//                  c |  a |   | f
//                 --- ---- --- --- 
//                    |  k | y |  
//                 --- ---- --- --- 
//
//            will give back a list
//                  c, a, NULL, f, NULL, k, y
// 
void KxGridLayout::cellContentInGridOrder(QList<QLayoutItem *> &gridOrderItemList)
{
	// Clear the list.
    gridOrderItemList.clear();

	for(int i = 0; i < fNumPositions; i++) {
		if(fPositionToItem.contains(i)) {
			gridOrderItemList.append(fPositionToItem[i]);
		} else {
			// Put in a NULL pointer for empty spaces in the layout.
			gridOrderItemList.append(NULL);
		}
	}
}

// Description:  Moves a control from its old cell to a new cell in the layout
//                (if the new cell is non-empty, the two controls switch positions).
//
void KxGridLayout::reposition(QObject *object, int newCellPosition)
{
    // If we don't allow empty cells, we can't move a control to a position
	// above the last position, since this would leave an empty space.
	if(!fAllowEmptyCells && newCellPosition >= fNumPositions) {
		return;
	}
	
	if (object == NULL || !object->isWidgetType()) {
        return;
    }

    // QGridLayout stores item in the order they
    // are added, not the in the order of cell position.
    // We want to find the index number QGridLayout
    // stored this item.
    QWidget * widgetToBeMoved = (QWidget *)object;
    if (widgetToBeMoved == NULL) {
        return;
    }

    // Find the user specified item.
	int oldIndex = indexOf(widgetToBeMoved);
    if (oldIndex < 0) {
        // Did not find the widget in any cell of the layout.
        return;
    }

    QLayoutItem *item = itemAt(oldIndex);

    // Calculate the item's old cell position.
    int oldCellPosition = -1;
	
	if(fItemToPosition.contains(item)) {
		oldCellPosition = fItemToPosition[item];
	}

    if (oldCellPosition == newCellPosition) {
        // The item is already in the correct cell.  We are done.
        return;
    } else {
		// We are going to move the item, so remove its old position info.
		fPositionToItem.remove(oldCellPosition);
		fItemToPosition.remove(item);
	}

    // Check if there is already an item in the new cell position.
	QLayoutItem *item2 = NULL;
	if(fPositionToItem.contains(newCellPosition)) {
		item2 = fPositionToItem[newCellPosition];

		// We are going to move the item, so remove its old position info.
		fPositionToItem.remove(newCellPosition);
		fItemToPosition.remove(item2);

		// Move the item to the first item's old position.
		fPositionToItem[oldCellPosition] = item2;
		fItemToPosition[item2] = oldCellPosition;
	}

	// Move the item to the new cell position.
	fPositionToItem[newCellPosition] = item;
	fItemToPosition[item] = newCellPosition;

	// If we put the item above the last position, the number of
	// positions has increased.
	if(newCellPosition >= fNumPositions) {
		fNumPositions = newCellPosition + 1;
	}

    invalidate();
}

// Description:  Calculates and sets the geometry of each item in the layout,
//               based on its set position and the layout's settings and/or width.
//
void KxGridLayout::doLayout(const QRect &rect)
{
    int x = rect.x();
    int y = rect.y();
	int currentRow = 1;
	int currentColumn = 0;

	for(int i = 0; i < fNumPositions; i++) {	
		currentColumn++;
		int nextX = x + fCellWidth;

		// If columnsResizable is set to true, go to the next row if we can't fit the control within the layout's width.
		// If columnsResizable is set to false, go to the next row if we have reached the specified max number of columns.
		if((fColumnsResizable && nextX > rect.right() + 1) || (!fColumnsResizable && currentColumn > fNumberOfColumns)) {
			x = rect.x();
			y = y + fCellHeight;
			nextX = x + fCellWidth;
			currentRow++;
			currentColumn = 1;
		}

		if(fPositionToItem.contains(i)) {
			QLayoutItem *item = fPositionToItem[i];
			QWidget *w = NULL;
			if(item && (w = item->widget())) {
				// We must make sure the widget can actually fit within the allotted cell size.
				if(w->minimumWidth() > fCellWidth) {
					w->setMinimumWidth(fCellWidth);
				}
				if(w->minimumHeight() > fCellHeight) {
					w->setMinimumHeight(fCellHeight);
				}

				w->setGeometry(QRect(QPoint(x, y), QSize(fCellWidth, fCellHeight)));

				// Hide the controls that exceed the specified max number of rows
				// (we've already made sure the max number of columns won't be
				// exceeded where it matters).
				if(currentRow > fNumberOfRows) {
					w->setVisible(false);
				} else {
					w->setVisible(true);
				}
			}
		}

		x = nextX;
	}
}
