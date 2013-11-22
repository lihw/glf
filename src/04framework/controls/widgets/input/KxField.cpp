#include "KxField.h"
#include "../../utils/KxUtils.h"

KxField::KxField(QWidget *parent /* = 0 */)
:QLineEdit(parent)
, fAlwaysInvokeEnterCmdOnReturn(false)
, fDoingDrag(false)
, fDragAndDropInSameField(false)
, fReturnKeyPressed(false)
, fOldText("")
, fUsePreferredSize(false)
{
    // force RMB press event on context menu so that we can show the
    // context menu on press instead of on release
    setContextMenuPolicy(Qt::PreventContextMenu);
    
    connect(this,SIGNAL(returnPressed()),this, SLOT(handleReturnPressed()));
    connect(this,SIGNAL(editingFinished()),this,SLOT(handleEditingFinished()));
    connect(this,SIGNAL(selectionChanged()),this,SLOT(handleSelectionChanged()));

    //KxDragManager::addDragHandler(this,this);
}

void KxField::focusInEvent(QFocusEvent *ev)
{
    QLineEdit::focusInEvent(ev);
}

void KxField::focusOutEvent(QFocusEvent *ev)
{
    QLineEdit::focusOutEvent(ev);
    if(!hasAcceptableInput()){
        handleEditingFinished();
    }
}

void KxField::keyPressEvent(QKeyEvent *ev)
{
    fReturnKeyPressed = false;
    if(ev->key() == Qt::Key_Return){
        fReturnKeyPressed = true;
    }
    QLineEdit::keyPressEvent(ev);
    if(ev->key() == Qt::Key_Enter|| ev->key() == Qt::Key_Return){
        if(!hasAcceptableInput()){
            handleReturnPressed();
            handleEditingFinished();
        }
    }
    ev->accept();
}

void KxField::mousePressEvent(QMouseEvent *ev)
{
    if((ev != NULL) && (ev->button() == Qt::RightButton)){
        emit contextMenuRequested(this,ev);
    }
    QLineEdit::mousePressEvent(ev);
}


void KxField::handleReturnPressed()
{
    if(!fAlwaysInvokeEnterCmdOnReturn && fReturnKeyPressed){
        return;
    }  
    // you can execute the execute command here 
}

void KxField::handleEditingFinished()
{
    if(fOldText == text()){
        return;
    }

    makeValueInvalid();
    fOldText = text();
    updateConnections();

    emit changeCompleted();
}

void KxField::handleSelectionChanged()
{
    if(QApplication::clipboard()->supportsSelection()){
        QApplication::clipboard()->setText(selectedText(),QClipboard::Selection);
    }
}


void KxField::setReadOnly(bool readOnly)
{
    if(isReadOnly() != readOnly){
        QLineEdit::setReadOnly(readOnly);
        updateFieldColor();
    }
}


void KxField::changeEvent(QEvent *ev)
{
    QLineEdit::changeEvent(ev);
    if(QEvent::EnabledChange == ev->type()){
        updateFieldColor();
    }
}

void KxField::updateFieldColor()
{
    bool isDarkened = isReadOnly() || !isEnabled();
    QColor bg = gKxUtils->schemeQColor(isDarkened ? KxUtils::kReadOnlyBackground: KxUtils::kTextFieldBackground);
    QPalette pal = palette();
    pal.setColor(QPalette::Active,QPalette::Base,bg);
    pal.setColor(QPalette::Inactive,QPalette::Base,bg);
    pal.setColor(QPalette::Disabled,QPalette::Base,bg);
    setPalette(pal);
}


void KxField::setText(const QString &newText)
{
    fOldText = newText;
    QLineEdit::setText(newText);
}

void KxField::newValueFromConnection(const QVariant &value)
{
    if(value.canConvert<QString>()){
        QString newText = value.toString();
        if(newText != fOldText){
            setText(newText);
        }
    }
}


#if defined (OSLinux_)
bool KxField::canPaste(const QMouseEvent *event)const 
{
    bool result = false;
    if(!isReadOnly()){
        // if click is not on top of a selection then it is a paste 
        if(!hasSelectedText()){
            result = true;
        }else{
            KxField *nonConstThis = const_cast<KxField*>(this);
            int eventPos = nonConstThis->cursorPositionAt(event->pos());
            int selectStart = selectionStart();
            result = (eventPos < selectStart || eventPos >= (selectStart + selectedText().length()));
        }
        
    }
    return result;
}

void KxField::doPaste(const QMouseEvent *event, const QString &text)
{
    setCursorPosition(cusorPositionAt(event->pos()));
    insert(text);
}

#endif


void KxField::updateConnections(bool isInterim)
{
    QVariant value;
    getValueForConnections(value);
    emit newValueForConnections(value, isInterim);
}


void KxField::getDragData(QWidget *source, QMouseEvent *, KxDragData *dragData)
{
    if(hasSelectedText()){
        QString selection = selectedText();
        if(!selection.isEmpty()){
            fDragItem.setString(selection);
            dragData->addDragItem(&fDragItem);

            fDoingDrag = true;
        }
    }
}


void KxField::dragDone(QWidget*, QMouseEvent*, Qt::DropAction action)
{
    //	If a move was just completed, we need to delete the text which was
    //	moved, which will be the selected text.
    //
    //	However, if the drag and drop occurred completely within this
    //	control, then QLineEdit's drop code will have already handled the
    //	deletion, so we don't want to do it again.
    if ((action == Qt::MoveAction) && !fDragAndDropInSameField) {
        del();
    }

    fDoingDrag = false;
    fDragAndDropInSameField = false;
}


void KxField::dropEvent(QDropEvent* event)
{
    QLineEdit::dropEvent(event);

    if (fDoingDrag) {
        fDragAndDropInSameField = true;
    }
}


void KxField::getValueForConnections(QVariant& value) const
{
    value.setValue(text());
}


void KxField::setPreferredSize(int width, int height)
{
    fPreferredSize.setWidth(width);
    fPreferredSize.setHeight(height);
    fUsePreferredSize = true;
}


QSize KxField::sizeHint() const
{
    //	If we've been provided with an explicit preferred size, use it,
    //	otherwise use QLineEdit's sizeHint.
    QSize	hintSize = (fUsePreferredSize ? fPreferredSize : QLineEdit::sizeHint());

    //	Controls sometimes use an invalid sizeHint() to communicate state
    //	information to the caller, so we only do adjustments if the value
    //	is valid.
    if (hintSize.isValid()) {
        //	Make sure that the size lies within the min/max bounds.
        QSize	maxSize = maximumSize();
        QSize	minSize = minimumSize();

        if (hintSize.width() > maxSize.width())
            hintSize.setWidth(maxSize.width());
        else if (hintSize.width() < minSize.width())
            hintSize.setWidth(minSize.width());

        if (hintSize.height() > maxSize.height())
            hintSize.setHeight(maxSize.height());
        else if (hintSize.height() < minSize.height())
            hintSize.setHeight(minSize.height());
    }

    return hintSize;
}

