#include "KxTable.h"
#include "KxTableView.h"

KxTable::KxTable(QWidget *parent, int rows, int columns)
: QAbstractTableModel(),
fTableView(NULL),
fRows(rows),
fColumns(columns)
{
    setTableView(new KxTableView(parent));
}

KxTable::~KxTable()
{
    delete fTableView;
    fTableView = NULL;
}

// -----------------------------
// QAbstractTableModel interface
// -----------------------------

int KxTable::rowCount(const QModelIndex &parent) const
{
    return fRows;
}

int KxTable::columnCount(const QModelIndex &parent) const
{
    return fColumns;
}

QVariant KxTable::data(const QModelIndex &index, int role) const
{
    QVariant v;

    switch(role) {
        case Qt::DisplayRole:
        case Qt::EditRole:
        case Qt::ToolTipRole: 
            {
                QString value;
                getCell(index.row(), index.column(), value);
                v.setValue(value);
                break;
            }
        default:
            break;
    }

    return v;
}	

QVariant KxTable::headerData(int section, Qt::Orientation orientation, int role) const
{
    QVariant v;

    switch(role) {
        case Qt::DisplayRole: {
            QString value;
            getLabel(orientation, section, value);
            v.setValue(value);
            break;
                              }
        default:
            break;
    }

    return v;
}

Qt::ItemFlags KxTable::flags(const QModelIndex &index) const
{
    Qt::ItemFlags flags = Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
    return flags;
}

bool KxTable::setData(const QModelIndex &index, const QVariant &value, int role)
{
    bool wasChanged = false;
    if(role == Qt::EditRole) {
        QString newValue = value.toString();
        wasChanged = cellChanged(index.row(), index.column(), newValue);
        if(wasChanged) {
            // we are supposed to emit this signal if we override setData()
            emit dataChanged(index, index);
        }
    }
    return wasChanged;
}

// -----------------------------
// Maya interface
// -----------------------------
void KxTable::getLabel(Qt::Orientation inOrientation, int index, QString &outValue) const
//
// Description:
//		Call either getColumnLabel() or getRowLabel(), depending on
//		the orientation of the header. This avoids the confusion of
//		Horizontal == ColumnHeader..
//
{
    if(inOrientation == Qt::Horizontal) {
        getColumnLabel(index, outValue);
    } else {
        getRowLabel(index, outValue);
    }
}

bool KxTable::cellChanged(int, int, const QString& )
//
// This is called when the user changes the value of a cell.  By
// default, nothing happens.
//
{
    return true;
}

void KxTable::getColumnLabel(int number, QString &outValue) const
//
// Notes:
//		Base implementation is NULL
//
{
}

void KxTable::getRowLabel(int number, QString &outValue) const
//
// Notes:
//		Base implementation is NULL
//
{
}

void KxTable::setNumberOfRows(int count)
{
    if(count > fRows) {
        beginInsertRows(QModelIndex(), fRows, count-1);
        insertRows(fRows, count - fRows);
        fRows = count;
        endInsertRows();
    } else if(count < fRows) {
        beginRemoveRows(QModelIndex(), fRows - (fRows - count), fRows - 1);
        removeRows(fRows - count, fRows - count);
        fRows = count;
        endRemoveRows();
    }
}

int KxTable::numberOfRows() const
{
    return fRows;
}

void KxTable::setNumberOfColumns(int count)
{
    if(count > fColumns) {
        beginInsertColumns(QModelIndex(), fColumns, count-1);
        insertColumns(fColumns, count - fColumns);
        fColumns = count;
        endInsertColumns();
    } else if(count < fColumns) {
        beginRemoveColumns(QModelIndex(), fColumns - (fColumns - count), fColumns - 1);
        removeColumns(fColumns - count, fColumns - count);
        fColumns = count;
        endRemoveColumns();
    }
}

int KxTable::numberOfColumns() const
{
    return fColumns;
}

void KxTable::setColumnWidth(int firstCol, int lastCol, int columnWidth)
{
    if(tableViewIsNULL()) return;
    for(int index = firstCol; index <= lastCol; index++) {
        fTableView->setColumnWidth(index, columnWidth);
    }
}

void KxTable::hideRowLabels()
{
    if(tableViewIsNULL()) return;
    fTableView->verticalHeader()->hide();
}

void KxTable::showRowLabels()
{
    if(tableViewIsNULL()) return;
    fTableView->verticalHeader()->show();
}

void KxTable::hideColumnLabels()
{
    if(tableViewIsNULL()) return;
    fTableView->horizontalHeader()->hide();
}

void KxTable::showColumnLabels()
{
    if(tableViewIsNULL()) return;
    fTableView->horizontalHeader()->show();
}

bool KxTable::isSelected(int row, int col)
{
    if(tableViewIsNULL()) return false;
    QModelIndex idx = index(row, col);
    return fTableView->selectionModel()->isSelected(idx);
}

void KxTable::setSpan(int row, int column, int rowSpan, int columnSpan)
{
    if(tableViewIsNULL()) return;
    fTableView->setSpan(row, column, rowSpan, columnSpan);
}

void KxTable::redrawCells()
{
    // Indicate that all data is dirty
    redrawCells(0, rowCount()-1, 0, columnCount()-1);
}

void KxTable::redrawCells(int firstRow, int lastRow, int firstColumn, int lastColumn)
{
    // Indicate that specified data is dirty
    emit dataChanged(
        KxTable::index(firstRow,firstColumn),
        KxTable::index(lastRow,lastColumn));
}

bool KxTable::suspendUpdates(bool shouldSuspend)
//
// If shouldSuspend is true, this method will stop the table
// from updating itself.  To resume updating, this method
// should be called with shouldSuspend set to false.
//
// To simplify the use of this call in possibly nested
// situations, this method returns the previous setting
// when it is called.  To use it safely, do the following:
//
//     bool temp = table->suspendUpdates(true);
//     .... do stuff that changes the table
//     if(!temp) table->suspendUpdates(false);
//
{
    if (tableView()) {
        bool isSuspended = !tableView()->updatesEnabled();
        if(shouldSuspend && isSuspended || !shouldSuspend && !isSuspended) {
            // value has not changed
            return shouldSuspend;
        } else {
            // setUpdatesEnabled has the opposite semantics
            tableView()->setUpdatesEnabled(!shouldSuspend);
            return !shouldSuspend;
        }
    } else {
        // if table view does not exist then updates to it are by
        // definition suspended
        return true;
    }
}

KxTableView* KxTable::tableView() const
{
    return fTableView;
}

void KxTable::setTableView(KxTableView *inView)
//
// Description:
//		Set the KxTableView, connecting up its selection changes
//		to call selectionChanged().
//
//		This is used by classes such as KxChannelBoxDelegate, whose
//		createEditor() method creates editors on the fly and needs to hook
//		them up properly.
//
{
    if(inView == fTableView) {
        // there's nothing to do, we're already
        // using inView
        return;
    }

    // get rid of the old view
    delete fTableView;

    // assign the new view
    fTableView = inView;

    if(fTableView != NULL) {
        // hook up to the new view
        fTableView->setModel(this);
        QItemSelectionModel *selectionModel = fTableView->selectionModel();

//#ifndef NDEBUG
//        // had a problem with putting the following line inside #ifndef and following
//        // 3 lines outside, so we have separate #ifndef/#else versions of connecting
//        // since we want to verify 'connected' in the debug build.
//        //
//        bool connected =
//            connect(selectionModel,
//            SIGNAL(selectionChanged(QItemSelection, QItemSelection)), 
//            SLOT(selectionChanged()));
//        DB_ASSERT(connected);
//        // debugging connections
//        connected = connect(selectionModel,
//            SIGNAL(destroyed()),
//            SLOT(selectionModelDestroyed()));
//        DB_ASSERT(connected);
//
//        connected = connect(fTableView,
//            SIGNAL(destroyed()),
//            SLOT(tableViewDestroyed()));
//        DB_ASSERT(connected);
//#else
        connect(selectionModel,
            SIGNAL(selectionChanged(QItemSelection, QItemSelection)), 
            SLOT(selectionChanged()));
//#endif /* !NDEBUG */

        connect(fTableView,
            SIGNAL(pressed(QModelIndex)),
            SLOT(cellEntered(QModelIndex)));
    }
}

QString &
KxTable::generateNewCellValue (
                                  const QString &currentValue,
                                  const QString &newValue
                                  )
                                  //
                                  //	Description:
                                  //		This is a helper method which will return a QString which
                                  //	is intended to be passed as a parameter for a .mel command.
                                  //	newValue is examined for relative change strings ('+=', '-=',
                                  //	'*=', "/=") and the appropriate string is constructed
                                  //
{
    static QString result;
    result.clear();

    // There are two kinds of change:  absolute and relative.  The user
    // can type just a number, and this will do an absolute change.  For
    // a relative change, the first two non-whitespace characters in the string
    // must be one of the following combinations:
    //
    //		+=   to do an addition
    //		-=   to do a subtraction
    //		*=	 to do a multiplication
    //		/=   to do a division

    enum {
        kAbsolute,
        kAddition,
        kSubtraction,
        kMultiplication,
        kDivision
    } opKey = kAbsolute;

    // First, check for any of the relative-change characters.

    QString localValue( newValue );
    //localValue.removeSpaces();

    //if ( localValue.length() > 2 && '=' == localValue[1] ){
    //    // an equals in the 2nd position means this may be a
    //    // relative operation
    //    QChar value = localValue.at(0);
    //    switch( value ){
    //    case '+':
    //        opKey = kAddition;
    //        break;
    //    case '-':
    //        opKey = kSubtraction;
    //        break;
    //    case '*':
    //        opKey = kMultiplication;
    //        break;
    //    case '/':
    //        opKey = kDivision;
    //        break;
    //    default:
    //        //anything else is an error.
    //        return (result);
    //    }
    //}

    if (opKey == kAbsolute) {
        result = localValue;
    }
    else {
        result = "(";
        result += currentValue + " ";
       
        /*switch (opKey) {
        case kAddition:
            result+= "+";
            break;
        case kSubtraction:
            result+= "-";
            break;
        case kMultiplication:
            result += "*";
            break;
        case kDivision:
            result += "/";
            break;
        default:
        }*/
            
        result += " ";
        result += localValue.mid( 2, localValue.length() - 2);
        result += ")";
    }
    return result;
}

void KxTable::selectionChanged()
//
// Description:
//		Qt uses a different model than Maya for selection change notification. Rather than
//		having a single, atomic notification of all the changes, Qt sends out a 'removed from
//		selection' message, followed by a separate 'added to selection' message.
//
//		KxTableView has special code in it to capture the 'removed from selection' and
//		coalesce it with the 'added to selection', so that this method is only called once.
//
//		See KxTableView and specifically KxTableView::mouseReleaseEvent(), which
//		emits the 'selectionChanged()' signal.
//
{
    // Do nothing by default
}

void KxTable::cellEntered(const QModelIndex& index)
//
//	Description:
//		Triggered on pressed signal from view
//
{
    // Do nothing by default
}

//void KxTable::getSelectedColumns( boolArray& selected )
//{
//    selected.createAndSet( false, fColumns );
//
//    // for each column
//    // for each row in the column
//    // if it is selected
//    // set the boolean in the array
//    for( int column = 0; column < fColumns; column++) {
//        selected[ column ] = false;
//        for( int row = 0; row <= fRows; row++) {
//            if( isSelected( row, column ) ) {
//                selected[ column ] = true;
//                break;
//            }
//        }
//    }
//}


bool KxTable::selection( int& firstRow, int& lastRow,
                               int& firstCol, int& lastCol, int rangeIndex)
                               // if there are selected cells, this method returns true and sets
                               // the parameter values.  If no cells are selected, this method returns
                               // false.  Note that the XRT structure does not mandate that
                               // the start be less than the end -- it depends which direction the
                               // user selected.
{
  /*  const int rowAdjust = (fFlags.testBit(kShowColumnLabels) ? 1 : 0);
    const int colAdjust = (fFlags.testBit(kShowRowLabels) ? 1 : 0);*/
    const int rowAdjust = 0;
    const int colAdjust = 0;
    bool hasSelection = false;

    if (rangeIndex == 0) {
        fSelectionBlocks.clear ();
        for (int row = 0; row < fRows; row++) {
            for (int col = 0; col < fColumns;) {
                if (!isSelected(row + rowAdjust, col + colAdjust)){
                    col++;
                    continue;
                }

                // Find an existing block that we can fit into
                //
                bool foundExistingBlock = false;
                if (row != 0) {
                    for (int currentBlock = 0; (currentBlock < (int)fSelectionBlocks.count()) && !foundExistingBlock; currentBlock++) {
                        QRect &block = fSelectionBlocks[currentBlock];
                        if ((block.bottom() == (int)(row - 1)) && (block.left() == (int)col)) {
                            // Check to see if we can extend this block down
                            //
                            foundExistingBlock = true;
                            int test;
                            for (test = col + 1; (test < fColumns) && ((int)test <= block.right()) && foundExistingBlock; test++) {
                                if (!isSelected(row + rowAdjust, test + colAdjust)){
                                    foundExistingBlock = false;
                                }
                            }
                            if (foundExistingBlock) {
                                // Check to make sure the cell to the right of the block is not selected
                                test = block.right() + 1;
                                if (test < fColumns) {
                                    if (isSelected(row + rowAdjust, test + colAdjust)){
                                        foundExistingBlock = false;
                                    }
                                }
                                if (foundExistingBlock) {
                                    // Extend the block down
                                    //
                                    block.setBottom(row);
                                    col = block.right() + 1;
                                }
                            }
                        }
                    }
                }
                if (!foundExistingBlock) {
                    // We need to create a new block

                    int extent = col;
                    for (int test = col + 1; test < fColumns; test++) {

                        if (isSelected(row + rowAdjust, test + colAdjust)){
                            extent++;
                        }
                        else {
                            break;
                        }
                    }
                    fSelectionBlocks.append(QRect(col, row, extent-col+1, 1));
                    col = extent + 1;
                }
            }
        }
    }
    if (rangeIndex < (int)fSelectionBlocks.count()) {
        const QRect &block = fSelectionBlocks[rangeIndex];
        firstRow = block.top();
        lastRow = block.bottom();
        firstCol = block.left();
        lastCol = block.right();
        hasSelection = true;
    }

    return (hasSelection);
}

void KxTable::setSelection(const QItemSelection& selection, QItemSelectionModel::SelectionFlags command, bool notify)
{
    if(tableViewIsNULL()) return;
    QItemSelectionModel* selectionModel = fTableView->selectionModel();
    const QItemSelection oldSelection = selectionModel->selection();

    selectionModel->select(selection, command);

    if (notify && oldSelection != selectionModel->selection()) {
        selectionChanged();
    }
}

void KxTable::clearSelection( bool notify )
{
    setSelection(QItemSelection(), QItemSelectionModel::Clear, notify);
}

void KxTable::setSelection (int row, int col, bool notify)
{
    QModelIndex idx = index(row, col);
    if (idx.isValid()) {
        setSelection(QItemSelection(idx, idx), QItemSelectionModel::ClearAndSelect, notify);
    }
}

void KxTable::setSelection (int firstRow, int lastRow, int firstCol, int lastCol, bool notify)
{
    QModelIndex startIdx = index(firstRow, firstCol);
    QModelIndex endIdx = index(lastRow, lastCol);
    if (startIdx.isValid() && endIdx.isValid()) {
        setSelection(QItemSelection(startIdx, endIdx), QItemSelectionModel::ClearAndSelect, notify);
    }
}

void KxTable::addToSelection (int row, int col, bool notify)
{
    QModelIndex idx = index(row, col);
    if (idx.isValid()) {
        setSelection(QItemSelection(idx, idx), QItemSelectionModel::Select, notify);
    }
}

void KxTable::removeFromSelection (int row, int col, bool notify)
{
    QModelIndex idx = index(row, col);
    if (idx.isValid()) {
        setSelection(QItemSelection(idx, idx), QItemSelectionModel::Deselect, notify);
    }
}

void KxTable::hideColumns(int firstCol, int lastCol )
{
    if(tableViewIsNULL()) return;
    for (int i = firstCol; i <= lastCol; i++)
    {
        fTableView->setColumnHidden(i, true);
    }
}


void KxTable::showColumns(int firstCol, int lastCol )
{
    if(tableViewIsNULL()) return;
    for (int i = firstCol; i <= lastCol; i++)
    {
        fTableView->setColumnHidden(i, false);
    }
}


void KxTable::showAllColumns()
{
    if(fColumns > 0) {
        showColumns(0, fColumns - 1);
    }
}


//void KxTable::showColumns( boolArray& visible)
//{
//    if(tableViewIsNULL()) return;
//    int count = fColumns+1;
//    if (visible.length() < count) 
//    {
//        count = visible.length();
//    }
//
//    for (int i = 0; i < count; ++i) 
//    {
//        fTableView->setColumnHidden(i, !visible[i]);
//    }
//}



void KxTable::redrawLabels(Qt::Orientation orientation)
{
    int count = 0;
    if (Qt::Horizontal == orientation)
    {
        count = numberOfColumns();
    }
    else
    {
        count = numberOfRows();
    }

    if (0 == count)
    {
        return;
    }

    emit headerDataChanged(orientation, 0, count - 1);
}

//#ifndef NDEBUG
//void KxTable::selectionModelDestroyed()
////
//// Description:
////		Debugging method, allowing a breakpoint to be set
////		for when Qt is about to delete the selection model
////
//{
//    static int destructionCount = 0;
//    destructionCount++;
//}
//
//void KxTable::tableViewDestroyed()
////
//// Description:
////		Debugging method, allowing a breakpoint to be set
////		for when Qt is about to delete the table view, which
////		seems to happen unexpectedly.
////
//{
//    static int destructionCount = 0;
//    destructionCount++;
//}
//#endif /* !NDEBUG */

bool KxTable::tableViewIsNULL() const
//
// Description:
//		Debugging method. Allows catching when the table view
//		becomes unexpectedly NULL.
//
{
    bool isNull = true;
    if(fTableView != NULL) {
        isNull = false;
    }
//#ifndef NDEBUG
//    else {
//        static int nullTableCount = 0;
//        nullTableCount++;
//    }
//#endif
    return isNull;
}