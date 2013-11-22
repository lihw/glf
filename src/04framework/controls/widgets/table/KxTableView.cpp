#include "KxTableView.h"
#include "KxItemDelegate.h"


// TelfUtils.h includes X.h, which has KeyPress and KeyRelease defines
// that conflict with the QEvent::Type enum values of the same names.
// Undefine to prevent issues.
#if defined( OSLinux_ )

#ifdef KeyPress
#undef KeyPress
#endif

#ifdef KeyRelease
#undef KeyRelease
#endif

#endif

// 
// Class:   	KxTableView
//
// Author(s):	Reid Ellis
//
// Date:		Thu Apr 17 2008
//

KxTableView::KxTableView(QWidget *parent)
: QTableView(parent)
{
    // Make tables expand (QSizePolicy::Expanding) horizontally to fill
    // available space by default. QSizePolicy::Preferred is the default
    // horizontal and vertical size policy.
    QSizePolicy expandHorizontallyPolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
    setSizePolicy(expandHorizontallyPolicy);

    // set a custom delegate to handle the difference between
    // return and enter in the item editors
    setItemDelegate(new KxItemDelegate(parent));
}

KxTableView::~KxTableView()
{
//#ifndef NDEBUG
//    static int destructionCount = 0;
//    destructionCount++;
//#endif /* !NDEBUG */
}

void KxTableView::mousePressEvent(QMouseEvent *event)
//
// Description:
//		Save the selection before user interaction begins.
//
{
    preMousePressSelection = selectionModel()->selection();

    // allow further processing of the event (for context menus)
    event->setAccepted(false);

    if (event->button() == Qt::LeftButton || event->modifiers() == Qt::NoModifier) {
        QTableView::mousePressEvent(event);
    }
}

void KxTableView::mouseReleaseEvent(QMouseEvent *event)
//
// Description:
//		Initiates an edit on the last selected cell after mouse release
//
{
    QTableView::mouseReleaseEvent(event);
    // get the selection after the user has finished
    postMouseReleaseSelection = selectionModel()->selection();

    // get the last selected, non-hidden, editable QModelIndex in QItemSelectionRange
    for(int i = postMouseReleaseSelection.indexes().length()-1; i >= 0; --i) {
        QModelIndex ix = postMouseReleaseSelection.indexes().at(i);
        if (!isIndexHidden(ix) && ix.flags().testFlag(Qt::ItemIsEditable)) {
            edit(ix);
            break;
        }
    }
}

void KxTableView::expandColumns()
//
// Description:
//		Make all columns equally wide, maximizing the table horizontally.
//		This is called in a delayed way at construction time.
//
{
    // the model holds the number of columns
    int columnCount = model()->columnCount();
    int rowHeaderWidth = verticalHeader()->width();
    int vertScrollWidth = verticalScrollBar()->width();
    int availableWidth = contentsRect().width() - rowHeaderWidth - vertScrollWidth;

    if(availableWidth > 0) {
        // make all columns take up and equal amount of space
        for(int i=0; i<columnCount; i++) {
            setColumnWidth(i, availableWidth/columnCount);
        }
    }
}

void KxTableView::showAllColumns(bool show)
//
//	Description:
//		Convenience method to show/hide all columns
//
{
    KxTable* table = qobject_cast<KxTable*>(model());
    if (table) {
        int columnCount = table->numberOfColumns();
        for (int i=0; i<columnCount; i++) {
            setColumnHidden(i, !show);
        }
    }
}

void KxTableView::showAllRows(bool show)
//
//	Description:
//		Convenience method to show/hide all rows
//
{
    KxTable* table = qobject_cast<KxTable*>(model());
    if (table) {
        int rowCount = table->numberOfRows();
        for (int i=0; i<rowCount; i++) {
            setRowHidden(i, !show);
        }
    }
}
//
//#ifndef NDEBUG
//void KxTableView::forgetEditor(QTableView *table)
////
//// Description:
////		Debugging method to expose a protected signal.
////
//{
//    // call protected slot method
//    emit editorDestroyed(table);
//}
//#endif /* !NDEBUG */

void KxTableView::selectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
//
// Description:
//		Handle a change in the UI's selection state.
//
// Parameters:
//		selected - items added to the selection
//		deselected - items removed from the selection
//
{
    // if there is an active selection, we want the current index to be part
    // of it so that if the user starts editing the edit occurs as part of
    // the selection.
    QItemSelectionModel* selModel = selectionModel();
    if (selModel && !selModel->isSelected(currentIndex())) {
        if (!selModel->selection().isEmpty()) {
            selModel->setCurrentIndex(selModel->selection().first().topLeft(), QItemSelectionModel::NoUpdate);
        }
    }

    QTableView::selectionChanged(selected, deselected);
}

void KxTableView::dataChanged(const QModelIndex& topLeft, const QModelIndex& bottomRight)
//
// Description:
//		Handle dataChanged signal from model.
//		Update item delegate so that when a user presses "enter" when editing
//		(instead of "return"), the delegate will select the text and remain open
//		for further edits
//
// Parameters:
//		topLeft - the top left index of change
//		bottomRight - the bottom right index of change
//
{
    QTableView::dataChanged(topLeft, bottomRight);
    KxItemDelegate* mayaDelegate = qobject_cast<KxItemDelegate*>(itemDelegate());
    if (mayaDelegate) {
        mayaDelegate->selectCurrent();
    }
}

bool KxTableView::event(QEvent *inEvent)
//
// Description:
//		Look for QEvent::Polish events and emit a signal if
//		one appears.
//
{
    bool eventRecognized = QTableView::event(inEvent);
    if(inEvent->type() == QEvent::Polish) {
        emit polish(inEvent);
    }
    // We don't want key presses/releases to be interpreted as hotkeys, so
    // prevent further processing of such events. We need further processing
    // for other events, in case we're doing drag 'n drop, so the drag manager
    // can handle these events, if necessary.
    //
    if( ( inEvent->type() == QEvent::KeyPress) ||
        ( inEvent->type() == QEvent::KeyRelease) ){
            inEvent->setAccepted(eventRecognized);
    }

    return eventRecognized;
}

void KxTableView::resizeEvent(QResizeEvent *inEvent)
//
// Description:
//		After a table resizes, let other classes adjust things
//		by emitting another polish() signal.
{
    QTableView::resizeEvent(inEvent);
    emit polish(inEvent);
}

//+
