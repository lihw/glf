#include "KxListWidget.h"
#include "../../base/KxQtHelper.h"

KxListWidget::KxListWidget(QWidget *parent)
: QListWidget(parent) 
, fMousePressed(false)
{
}

KxListWidget::~KxListWidget()
{
}

void 
KxListWidget::keyPressEvent ( QKeyEvent * event )
{
	if ((NULL != event) && (event->key() == Qt::Key_Delete)) {
	
        //KxQtHelper::executeCommandScript("deleteCommand", this);
		event->accept();
		return;
	}  

	// In the following lines, we may be creating a new key event for QListWidget to process instead of
	// the original, so create another pointer for this new event so that we can safely delete it from memory
	// without touching the original event pointer (which may be handled by Qt later on).
	QKeyEvent *processEvent = NULL;

	// The left and right arrow keys are supposed to behave like the up and down arrow keys, respectively.
	// However, QListWidget doesn't interpret those key presses as such, so create a new event that is just
	// like the original but registers up/down keys rather than left/right and pass that to QListWidget's
	// keyPressEvent to process.
	if(event->key() == Qt::Key_Left) {
		processEvent = new QKeyEvent(event->type(), Qt::Key_Up, event->modifiers(),
								event->text(), event->isAutoRepeat(), event->count());
	} else if(event->key() == Qt::Key_Right) {
		processEvent = new QKeyEvent(event->type(), Qt::Key_Down, event->modifiers(),
								event->text(), event->isAutoRepeat(), event->count());
	}

	// Key presses with the control modifier are supposed to behave like normal key presses (i.e. as if the
	// control key was not pressed). However, QListWidget internally handles such key presses in a completely
	// different way, so create a new event that is just like the original but doesn't involve the control modifier
	// and pass that to QListWidget's keyPressEvent to process.
	bool controlPressed = (event->modifiers() & Qt::ControlModifier) && event->key() != Qt::Key_Control;
	if (controlPressed) {
		processEvent = new QKeyEvent(event->type(), event->key(), (event->modifiers() & ~Qt::ControlModifier),
								QString(QChar(event->key())), event->isAutoRepeat(), event->count());
	}

	// If we didn't have to replace the original key event, just make an exact copy of it for us to process.
	if(processEvent == NULL) {
		processEvent = new QKeyEvent(event->type(), event->key(), event->modifiers(),
								event->text(), event->isAutoRepeat(), event->count());
	}

	bool shiftPressed = (processEvent->modifiers() & Qt::ShiftModifier) && processEvent->key() != Qt::Key_Shift;
	QListWidgetItem * shiftSelectEndItem = currentItem();
	bool curItemWasSelected = shiftSelectEndItem?shiftSelectEndItem->isSelected():false;

	// Key presses with the shift modifier with a non-continguous multi-item selection are supposed to behave
	// the same way as if the selection only consisted of the continguous block that contains the current item.
	// However, QListWidget still includes the non-continguous blocks in the new selection. To accommodate,
	// we will first find and record the item in that one block that will be the other end of the new selection
	// (the first end, of course, being the new current item after the key press).
	if(shiftPressed && curItemWasSelected) {
		// If the current item is at the top of the block, search down for the other end. If it is at the bottom
		// of the block, search up. Otherwise, it is the only item in the block, and is thus the item that will be
		// the other end of the new selection.
		int r = row(currentItem());
		// If r==count()-1, we need to bail to the "else if" portion, else
		// we would dereference item(count()) and crash; see bug #335803.
		//
		if(r == 0 || ( (r != (count()-1)) && item(r + 1)->isSelected() && !item(r - 1)->isSelected())) {
			while(r < count() - 1) {
				QListWidgetItem * nextItemDown = item(r + 1);
				if(!nextItemDown->isSelected()) {
					break;
				} else {
					shiftSelectEndItem = nextItemDown;
					r++;
				}
			}
		} else if(r == count() - 1 || (item(r - 1)->isSelected() && !item(r + 1)->isSelected())) {
			while(r > 0) {
				QListWidgetItem * nextItemUp = item(r - 1);
				if(!nextItemUp->isSelected()) {
					break;
				} else {
					shiftSelectEndItem = nextItemUp;
					r--;
				}
			}
		}
	}

	QList<QListWidgetItem *> beforeSelected = selectedItems();
	QListWidget::keyPressEvent(processEvent);
	QList<QListWidgetItem *> afterSelected = selectedItems();

	if(shiftPressed) {
		// If the previous current item had not been selected before a key press with the Shift
		// modifier, all selected items on the list get deselected.
		if(!curItemWasSelected) {
			bool blocked = blockSignals(true);
			for(int i = 0; i < afterSelected.count(); i++) {
				QListWidgetItem *listItem = afterSelected[i];
				if(listItem) {
					listItem->setSelected(false);
				}
			}
			blockSignals(blocked);
			executeSelectCommand();
			event->accept();
			return;
		} else if(beforeSelected != afterSelected) {
			// Now, to emulate the expected behaviour, we select all the items between and 
			// including the new current item and the item we saved from before, and deselect
			// everything else.
			int shiftSelectFirstIndex = row(shiftSelectEndItem);
			int shiftSelectLastIndex = row(currentItem());

			if(shiftSelectFirstIndex > shiftSelectLastIndex) {
				int temp = shiftSelectFirstIndex;
				shiftSelectFirstIndex = shiftSelectLastIndex;
				shiftSelectLastIndex = temp;
			}

			bool blocked = blockSignals(true);
			for(int i = 0; i < count(); i++) {
				QListWidgetItem *listItem = item(i);
				if(listItem) {
					bool itemWithinSelection = (i >= shiftSelectFirstIndex && i <= shiftSelectLastIndex);
					if((itemWithinSelection && !listItem->isSelected()) || (!itemWithinSelection && listItem->isSelected())) {
						listItem->setSelected(!listItem->isSelected());
					}			
				}
			}
			blockSignals(blocked);
			afterSelected = selectedItems();
		}
	} else if((controlPressed && beforeSelected != afterSelected) || processEvent->key() == Qt::Key_Space) {
		// For whatever reason, Qt still picks up the control modifier and multi-selects on
		// alphanumeric key presses. To emulate the correct behaviour, we deselect every
		// selected item unless it is the current item. Space bar presses are supposed to do
		// the same thing (except with the Shift modifier, which should already be handled
		// above), so we include that case as well.
		bool blocked = blockSignals(true);

		QListWidgetItem *curItem = currentItem();
		for(int i = 0; i < afterSelected.count(); i++) {
			QListWidgetItem *listItem = afterSelected[i];
			if(listItem && listItem != curItem) {
				listItem->setSelected(false);
			}
		}

		// Select the current item if it wasn't already.
		if( curItem && !curItem->isSelected()) {
			curItem->setSelected(true);
		}

		blockSignals(blocked);
		afterSelected.clear();
		if( curItem ){
			afterSelected.append(curItem);
		}
	}

	// The keys listed below should always trigger a selection, even if the selection doesn't
	// actually change from before the key press.
	bool actionKeyPressed = processEvent->key() == Qt::Key_Up || processEvent->key() == Qt::Key_Down
								|| processEvent->key() == Qt::Key_Home || processEvent->key() == Qt::Key_End
								|| processEvent->key() == Qt::Key_PageUp || processEvent->key() == Qt::Key_PageDown
								|| processEvent->key() == Qt::Key_Enter || processEvent->key() == Qt::Key_Return
								|| processEvent->key() == Qt::Key_Space;

	// If the selection has changed, or one of the action keys listed above was pressed, or an
	// alphanumeric key press occurred that just happened to result in the same selection as
	// before, then execute the select command.
	if((beforeSelected != afterSelected) || actionKeyPressed
		|| ((beforeSelected.count() == 1) && (!processEvent->text().isEmpty())
		&& (beforeSelected[0]->text().startsWith(processEvent->text(), Qt::CaseInsensitive)))) {
		executeSelectCommand();
	}

	delete processEvent;
	event->accept();
}

void
KxListWidget::mousePressEvent(QMouseEvent* event)
{
	if( event->button() == Qt::MidButton ){
		// Let KxDragManager handle drag 'n drop.
		//
		event->ignore();
	} else if (!KxQtHelper::handlePopupMenu(this, event) && itemAt(event->pos()) != NULL) {
		/*
		Although the eventFilter in KxQtHelper does get installed on
		this widget, QListWidget eats the mouse click events and only a
		QContextMenuEvent is sent to the filter. So I'm overriding the
		press event here and calling the handlePopupMenu function directly.
		*/
	
		fMousePressed = true;

		// Mouse presses with the shift modifier with a non-continguous multi-item selection are supposed to behave
		// the same way as if the selection only consisted of the continguous block that contains the current item.
		// However, QListWidget still includes the non-continguous blocks in the new selection. To accommodate,
		// we will first find and record the item in that one block that will be the other end of the new selection
		// (the first end, of course, being the new current item after the mouse press).
		bool shiftPressed = (event->modifiers() & Qt::ShiftModifier);
		QListWidgetItem * shiftSelectEndItem = currentItem();
		bool curItemWasSelected = shiftSelectEndItem?shiftSelectEndItem->isSelected():false;
		if(shiftPressed && curItemWasSelected) {
			// If the current item is at the top of the block, search down for the other end. If it is at the bottom
			// of the block, search up. Otherwise, it is the only item in the block, and is thus the item that will be
			// the other end of the new selection.
			int r = row(currentItem());
			// If r==count()-1, we need to bail to the "else if" portion, else
			// we would dereference item(count()) and crash; see bug #335803.
			//
			if(r == 0 || ( (r != (count()-1)) && item(r + 1)->isSelected() && !item(r - 1)->isSelected())) {
				while(r < count() - 1) {
					QListWidgetItem * nextItemDown = item(r + 1);
					if(!nextItemDown->isSelected()) {
						break;
					} else {
						shiftSelectEndItem = nextItemDown;
						r++;
					}
				}
			} else if(r == count() - 1 || (item(r - 1)->isSelected() && !item(r + 1)->isSelected())) {
				while(r > 0) {
					QListWidgetItem * nextItemUp = item(r - 1);
					if(!nextItemUp->isSelected()) {
						break;
					} else {
						shiftSelectEndItem = nextItemUp;
						r--;
					}
				}
			}
		}

		QListWidget::mousePressEvent(event);

		if(shiftPressed && shiftSelectEndItem) {
			int shiftSelectFirstIndex = row(shiftSelectEndItem);
			int shiftSelectLastIndex = row(currentItem());

			// If the previous current item had not been selected before a mouse press with the
			// Shift modifier, the item on the other end of the new selection will actually be
			// either the one just before the previous current item (if the new current item is
			// above the previous) or just after (if the new current item is below the previous).
			if(shiftSelectFirstIndex > shiftSelectLastIndex) {
				if(!curItemWasSelected) {
					shiftSelectFirstIndex--;
				}

				int temp = shiftSelectFirstIndex;
				shiftSelectFirstIndex = shiftSelectLastIndex;
				shiftSelectLastIndex = temp;
			} else if(!curItemWasSelected) {
				shiftSelectFirstIndex++;
			}

			// Now, to emulate the expected behaviour, we deselect all the selected items that
			// aren't in the block between and including the new current item and the item we
			// saved from before (the items in the block itself should already be selected).
			bool blocked = blockSignals(true);
			QList<QListWidgetItem *> afterSelected = selectedItems();
			for(int i = 0; i < afterSelected.count(); i++) {
				QListWidgetItem *listItem = afterSelected[i];
				if(listItem) {
					int index = row(listItem);
					bool itemWithinSelection = (index >= shiftSelectFirstIndex && index <= shiftSelectLastIndex);
					if(!itemWithinSelection) {
						listItem->setSelected(false);
					}		
				}
			}
			blockSignals(blocked);
		}
	}
}

void
KxListWidget::mouseMoveEvent(QMouseEvent* event)
{
	// We only want items to get selected on a mouse drag if it started
	// from a valid mouse press event.
	//
	if(fMousePressed) {
		QListWidget::mouseMoveEvent(event);
	}
}

void
KxListWidget::mouseReleaseEvent(QMouseEvent* event)
{
    // If we are coming out of a mouse press, it's okay to execute the select
	// command now (since even a multi-selection will be done at this point).
	//
	if(fMousePressed) {
		fMousePressed = false;
		QListWidget::mouseReleaseEvent(event);
		executeSelectCommand();	
	}
}

void
KxListWidget::executeSelectCommand()
{
	// We want the select command to be executed on every item selection change
	// except during a multi-selection (there we only want the last item selected to
	// trigger the command).
	//
	/*if(!fMousePressed) {
		KxQtHelper::executeCommandScript("selectCommand", this);
	}*/
}
