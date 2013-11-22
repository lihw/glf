#include "KxScrollField.h"

#include "../../utils/KxUtils.h"
#include "../../drag/KxDragManager.h"
#include "../../base/KxQtHelper.h"

// Undef any X-header macros that are the same as Qt enums
#if defined(OSUnix_)
#undef FontChange
#undef KeyPress
#endif

// Description:  Constructor
//
KxScrollField::KxScrollField(QWidget *parent)
: Parent(parent)
, fDoingDrag(false)
, fDragAndDropInSameField(false)
, fSearchString("")
, fSearchOption(0)
, fIndentString("\t")
, fSpacesPerTab(4)
, fSearchWraps(false)
{
	// deliver mouse click events instead of using
	// Qt context menu architecture
	setContextMenuPolicy( Qt::PreventContextMenu );
    QScrollBar* bar = horizontalScrollBar();
    if (bar) bar->setContextMenuPolicy(Qt::PreventContextMenu);
    bar = verticalScrollBar();
    if (bar) bar->setContextMenuPolicy(Qt::PreventContextMenu);

	this->setFont(gKxUtils->schemeQFont (KxUtils::kFixedWidthFont));
    setSpacesPerTab(fSpacesPerTab);

	KxDragManager::addDragHandler(this, this);
}

// Description:  This procedure returns the number of 
//               cursor-addressable lines in the field
//
int KxScrollField::lineCount()
{
	if ( document() ) {
		return document()->blockCount();
	}
	return 0;
}

// Description:  This procedure handles the key press event.
//
void KxScrollField::keyPressEvent(QKeyEvent *event)
{
    // We don't want line seperators, send a return event instead
    if (event->matches(QKeySequence::InsertLineSeparator)) {
        QKeyEvent ev2 = QKeyEvent(QEvent::KeyPress, Qt::Key_Return, Qt::NoModifier);
        return keyPressEvent(&ev2);
    }
#if !defined(OSMac_)
    // Ctrl+Shift+Up/Down:
    // If at the end of the line, it selects all of the next line
    // Otherwise it selects to the end of the current line
    // This is a convenient shortcut for normal shift-end/home, shift-arrow selection
    // except on Mac, which never had this behaviour (Use Command+Shift+Left/Right instead)
    Qt::KeyboardModifiers mods = KxQtHelper::getKeyboardModifiers(event);
    if (mods.testFlag(Qt::ControlModifier) &&
        mods.testFlag(Qt::ShiftModifier)) {
        QTextCursor cur = textCursor();
        if (event->key() == Qt::Key_Up) {
            if (cur.atBlockStart()) {
                cur.movePosition(QTextCursor::PreviousBlock, QTextCursor::KeepAnchor);
            }                
            cur.movePosition(QTextCursor::StartOfBlock, QTextCursor::KeepAnchor);
            setTextCursor(cur);
            return;
        } else if (event->key() == Qt::Key_Down) {
            if (cur.atBlockEnd()) {
                cur.movePosition(QTextCursor::NextBlock, QTextCursor::KeepAnchor);
            }
            cur.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
            setTextCursor(cur);
            return;
        }
    }
#endif // OSMac_

    if (event->key() == Qt::Key_Enter) {
        // Call the enter command.
        //
       // KxQtHelper::executeCommandScript("enterCommand", this); 
    } else {
        // Call the key press command.
        //
       // KxQtHelper::executeCommandScript("keyPressCommand", this); 
    }
    Parent::keyPressEvent(event);

	// Scroll text fields should accept all key events.
	event->accept();
}

// Description:  This procedure handles the key release event.
//
void KxScrollField::keyReleaseEvent(QKeyEvent *event)
{
	Parent::keyReleaseEvent(event);

	// Scroll text fields should accept all key events.
	event->accept();
}

// Description:  This procedure handles the lose focus event.
//
void KxScrollField::focusOutEvent(QFocusEvent *event)
{
    // Call the changeCommand.
    //
    //KxQtHelper::executeCommandScript("changeCommand", this); 

    Parent::focusOutEvent(event);
}

// Description:  This procedure handles the receiving focus event.
//
void KxScrollField::focusInEvent(QFocusEvent *event)
{
    // Call the receiveFocusCommand.
    //
   // KxQtHelper::executeCommandScript("receiveFocusCommand", this); 

    Parent::focusInEvent(event);
}

// Description: This procedure handles the mouse press event
//
void KxScrollField::mousePressEvent(QMouseEvent *event)
{
	if( !KxQtHelper::handlePopupMenu(this, event) ) {
		Parent::mousePressEvent( event );
	}
	
}

/*!
  Change the editor font size by an increment
  \param[in] delta specified in pixels
 */
void KxScrollField::stepFontSize(int delta)
{
    QFont fnt = font();
    Q_ASSERT(fnt.pixelSize() >= 0);
    const int newSz = fnt.pixelSize() + delta;
    if (newSz > 0) {
        fnt.setPixelSize(newSz);
        setFont(fnt);
    }
}

/*!
  \brief Emit fontChanged signal for each FontChange event
 */
void KxScrollField::changeEvent(QEvent* event)
{
    Parent::changeEvent(event);
    if(event->type() == QEvent::FontChange) {
        emit fontChanged();
    }
}

/*!
 \brief handles the mouse wheel event
*/
void KxScrollField::wheelEvent(QWheelEvent *event)
{
    Qt::KeyboardModifiers mods = KxQtHelper::getKeyboardModifiers(event);
    if (mods.testFlag(Qt::ControlModifier)) {
        // Change the font size of the entire document
        int change = event->delta() > 0 ? 1 : -1;
        stepFontSize(change);
    } else {
        Parent::wheelEvent(event);
#if defined(OSMac_)
        //
        // See bug #350121
        // scroll field becomes corrupted when wheel scrolling on Mac
        //
        update();
#endif
    }
}

// Description:  This procedure sets the search option.
//
void KxScrollField::setSearchOption(
    QTextDocument::FindFlags option, bool state)
{
    if (state)
    {
        // Turn this option on.
        //
        fSearchOption |= option;
    }
    else
    {
        // Turn this option off.
        //
        fSearchOption &= ~option;
    }
}

bool KxScrollField::searchAndSelect()
//
// Description:
//  Perform one search and select op using search parameter state
//
{
	QTextCursor current = textCursor();
	QTextCursor findCursor = document()->find( searchString(), current, searchOption());
    bool backwards = searchOption() & QTextDocument::FindBackward;

    if (findCursor.isNull() && searchWraps())
    {
        // wrap the cursor and try again
        QTextCursor wrapped = current;
        wrapped.movePosition(backwards ? QTextCursor::End : QTextCursor::Start);
        findCursor = document()->find( searchString(), wrapped, searchOption());
        if (!findCursor.isNull() && (findCursor.position() == current.position())) {
            // just found the current match, which is not actually valid
            findCursor = QTextCursor();
        }
    }
    if (findCursor.isNull())
    {
        current.movePosition(backwards ? QTextCursor::Start : QTextCursor::End);
		setTextCursor(current);
        return false;
    }
    setTextCursor(findCursor);
	return true;
}


// Description:  This procedure indents the currently selected lines
//               or the current line if there is no selection.
//
void KxScrollField::indentSelection(int indentationDelta)
{
    // Figure out the beginning and end of the selection.
    //
    QTextCursor tmpTextCursor = textCursor();
    int startPos = tmpTextCursor.position();
    int endPos = tmpTextCursor.anchor();

    if (startPos > endPos)
    {
        // Swap them, so that startPos <= endPos.
        //
        startPos = tmpTextCursor.anchor();
        endPos = tmpTextCursor.position();
    }

    // Start from the line containing the position startPos.
    //
    tmpTextCursor.setPosition(startPos);
    tmpTextCursor.movePosition(QTextCursor::StartOfLine);

    tmpTextCursor.beginEditBlock();

    if (tmpTextCursor.position() <= endPos)
    {
        do
        {
            // For each line, indent it bY indentationDelta.
            // For example, 
            //  . inentationDelta = 1 means indent it to the right by one 
            //      more tab,
            //  . indentationDelta = -1 means indent it to the left by one
            //      more tab. 
            //
            if (indentationDelta >= 0) {
                for (int i=0; i < indentationDelta; ++i) {
                    // add an indent.  The end position must be shifted.
                    tmpTextCursor.insertText(fIndentString);
                    endPos += fIndentString.length();
                }
            } 
            else 
            {
                // we are deleting
                if (tmpTextCursor.atBlockEnd()) break;
                tmpTextCursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
                // move the cursor selection to include everything we want to delete
                // we will either delete tabs or spaces, 
                if (tmpTextCursor.selectedText().endsWith('\t')) {
                    // deleting tabs
                    for(int i=indentationDelta + 1; i < 0; ++i) {
                        if (tmpTextCursor.atBlockEnd()) break;
                        tmpTextCursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
                        if (!tmpTextCursor.selectedText().endsWith('\t')) {
                            tmpTextCursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor);
                            break;
                        }
                    }
                } else if (tmpTextCursor.selectedText().endsWith(' ')) {
                    // deleting spaces
                    for(int i=(indentationDelta*fSpacesPerTab) + 1; i < 0; ++i) {
                        if (tmpTextCursor.atBlockEnd()) break;
                        tmpTextCursor.movePosition(QTextCursor::NextCharacter,QTextCursor::KeepAnchor);
                        if (!tmpTextCursor.selectedText().endsWith(' ')) {
                            tmpTextCursor.movePosition(QTextCursor::PreviousCharacter,QTextCursor::KeepAnchor);
                            break;
                        }
                    }
                } else {
                    // no leading tabs or spaces
                    tmpTextCursor.clearSelection();
                }
                // remove the selected text.  The end position must be shifted.
                endPos -= tmpTextCursor.selectedText().length();
                tmpTextCursor.removeSelectedText();
            }

            // Move down by one line.
            //
            if (! tmpTextCursor.movePosition(QTextCursor::NextBlock) ) {
                break;
            }
        } while (tmpTextCursor.position() < endPos);
    }
    tmpTextCursor.endEditBlock();
}

// Description:  This procedure returns the line number 
//               for the line where the cursor is in the given
//               textCursor. 
//               The returned line number is 0-based.
//
/* static */
int KxScrollField::getLineNumber(QTextCursor textCursor)
{
	if ( !textCursor.isNull() ) {
		return textCursor.blockNumber();
	}
	return 0;
}

// Description:  This procedure selects the part of the text 
//               in the given textCursor from the given startPos
//               to the given endPos.
//
/* static */
void KxScrollField::select(
    QTextCursor &textCursor, int startPos, int endPos)
{
    if (startPos < 0) {
        startPos = 0;
    }
    if (endPos < 0) {
        endPos = 0;
    }

    if (startPos == endPos) {
        textCursor.setPosition(startPos);
    } else if (startPos > endPos) {
        select(textCursor, endPos, startPos);
    } else {
        textCursor.setPosition(startPos);
        textCursor.movePosition(QTextCursor::Right, 
                                QTextCursor::KeepAnchor, endPos - startPos + 1);
    }
}

//! Returns true if indentSelection uses tabs
bool KxScrollField::useTabs() const
{ 
    return fIndentString.at(0) == QChar('\t'); 
}

//! Toggle using tabs vs. using spaces for indentSelection
void KxScrollField::setUseTabs(bool useTabs)
{
    if (useTabs) {
        fIndentString = QChar('\t');
    } else {
        fIndentString.fill(' ', fSpacesPerTab);
    }
}
//! Set the number of spaces equivalent to one tab stop
void KxScrollField::setSpacesPerTab(int n)
{ 
    fSpacesPerTab = n;
    setTabStopWidth(fontMetrics().width(" ")*fSpacesPerTab);
    // update the indent string
    if (!useTabs()) {
        fIndentString.fill(' ', fSpacesPerTab);
    }
}


void KxScrollField::dragDone(QWidget*, QMouseEvent*, Qt::DropAction action)
{
	//	If a move was just completed, we need to delete the text which was
	//	moved, which will be the selected text.
	//
	//	However, if the drag and drop occurred completely within this
	//	control, then QTextEdit's drop code will have already handled the
	//	deletion, so we don't want to do it again.
	if ((action == Qt::MoveAction) && !fDragAndDropInSameField) {
		textCursor().removeSelectedText();
	}

	fDoingDrag = false;
	fDragAndDropInSameField = false;
}


void KxScrollField::getDragData(
	QWidget*, QMouseEvent*, KxDragData* dragData
)
{
	if (textCursor().hasSelection()) {
		QString	text = textCursor().selection().toPlainText();

		if (!text.isEmpty()) {
			//	Make the selected text available as a primitive
			//	drag item (i.e. a string).
			fDragItem.setString(text);
			dragData->addDragItem(&fDragItem);

			fDoingDrag = true;
		}
	}
}

void KxScrollField::dropEvent(QDropEvent* event)
{
	QTextEdit::dropEvent(event);

	if (fDoingDrag) {
		fDragAndDropInSameField = true;
	}
}

//#if defined(OSLinux_)
////! Return true if the mouse event could be a MMB paste
//bool KxScrollField::canPaste(const QMouseEvent* event) const
//{
//	bool result = false;
//	if (!isReadOnly()) {
//		// if click is not on top of a selection then it's a paste
//		QTextCursor cursor = textCursor();
//		if (!cursor.isNull()) {
//			if (!cursor.hasSelection()) {
//				result = true;
//			} else {
//				QTextCursor eventCursor = cursorForPosition(event->pos());
//				if (!eventCursor.isNull()) {
//					int eventPos = eventCursor.position();
//					result = (eventPos < cursor.selectionStart() ||
//								eventPos > cursor.selectionEnd());
//				}
//			}
//		}
//	}
//	return result;
//}
//
////! Paste the text into the field at the position of the event
//void KxScrollField::doPaste(const QMouseEvent* event, const QString& text)
//{
//	QTextCursor cursor = cursorForPosition(event->pos());
//	if (!cursor.isNull()) {
//		setTextCursor(cursor);
//		insertPlainText(text);
//	}
//}
//#endif
