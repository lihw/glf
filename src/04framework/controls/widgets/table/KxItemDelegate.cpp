#include "KxItemDelegate.h"

KxItemDelegate::KxItemDelegate(QObject* parent)
: ParentClass(parent)
{
}

KxItemDelegate::~KxItemDelegate()
{
}

void KxItemDelegate::setEditorData(
                                      QWidget *				editor,
                                      const QModelIndex &		index) const
{
    ParentClass::setEditorData(editor, index);

    // for text editors, the event that triggers setting of the editor
    // data also clears the selection, so we restore it here (always want
    // text selected after data is set)
    doTextSelection(editor);
}

bool KxItemDelegate::eventFilter(QObject* object, QEvent* event)
{
    // differentiate between "return" and "enter" when user is editing
    QWidget* editor = qobject_cast<QWidget*>(object);
    if (editor) {
        if (event->type() == QEvent::KeyPress) {
            switch(static_cast<QKeyEvent *>(event)->key()) {
                case Qt::Key_Enter:
                    fLastEditor = editor;
                    emit commitData(editor);
                    return true;
                case Qt::Key_Return:
                    emit commitData(editor);
                    emit closeEditor(editor, QAbstractItemDelegate::SubmitModelCache);
                    // note: fall through to next block
                case Qt::Key_Escape:
                    // Finished editing and closed editor - return focus to panel that last had it.
                    /*Tpanel *panel = gThePanelManager->panelWithFocus();
                    if(NULL != panel) {
                        panel->setFocus();
                    }*/
                    return true;
            }
        }
    }

    return ParentClass::eventFilter(object, event);
}

void KxItemDelegate::selectCurrent()
{
    doTextSelection(fLastEditor);
}

void KxItemDelegate::doTextSelection(QObject* editor) const
{
    // trim and select all text
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
    if (lineEdit) {
        QString text = lineEdit->text();
        text = text.trimmed();
        lineEdit->setText(text);
        lineEdit->selectAll();
        if (QApplication::clipboard()->supportsSelection()) {
            // Clipboard is automatically updated for mouse selection
            // But we also want programatic selection to work
            QApplication::clipboard()->setText(lineEdit->selectedText(), QClipboard::Selection);
        }
    }
}

//
// Copyright 2009 Autodesk, Inc.  All rights reserved.
//
// This computer source code and related instructions and comments are
// the unpublished confidential and proprietary information of Autodesk,
// Inc. and are protected under applicable copyright and trade secret
// law.  They may not be disclosed to, copied or used by any third party
// without the prior written consent of Autodesk, Inc.
//
