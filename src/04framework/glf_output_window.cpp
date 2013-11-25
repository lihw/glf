// -------------------------------------------------------------- 
// glf_output_window.cpp
// The output log window
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#include "glf_output_window.h"

    
GLFOutputWindow::GLFOutputWindow(QWidget* parent)
    : KxDockWidget(tr("Output"), parent)
{
    setFloating(true);
    setFeatures(QDockWidget::DockWidgetMovable | QDockWidget:: DockWidgetFloatable);
    setAllowedAreas(Qt::NoDockWidgetArea);
    setMinimumSize(640, 120);

    QWidget* mainWidget = new QWidget(this);
    setWidget(mainWidget);

    _pTextEdit = new QTextEdit(this);
    _pTextEdit->setReadOnly(true);

    _pSaveButton = new QPushButton(tr("Save"), this);
    _pClearButton = new QPushButton(tr("Clear"), this);

    connect(_pSaveButton, SIGNAL(clicked()), this, SLOT(onSave()));
    connect(_pClearButton, SIGNAL(clicked()), this, SLOT(onClear()));
    
    QVBoxLayout* mainLayout = new QVBoxLayout(mainWidget);
    mainWidget->setLayout(mainLayout);

    mainLayout->addWidget(_pTextEdit);

    QHBoxLayout* hboxLayout = new QHBoxLayout();
    mainLayout->addLayout(hboxLayout);

    hboxLayout->addStretch();
    hboxLayout->addWidget(_pClearButton);
    hboxLayout->addWidget(_pSaveButton);
}

void GLFOutputWindow::printMessage(const char* message)
{
    static int index = 1;
    // Remove the first few lines when the buffer is overflow.
    int nlines = _pTextEdit->document()->lineCount();
    if (nlines > MAX_NUM_LINES)
    {
        int deleteNumLines = nlines / 2;
        for (int i = 0; i < deleteNumLines; ++i)
        {
            QTextCursor tc = _pTextEdit->textCursor();

            tc.movePosition(QTextCursor::Start);
            tc.select(QTextCursor::LineUnderCursor);
            tc.removeSelectedText();
        }

        QTextCursor tc = _pTextEdit->textCursor();
        tc.movePosition(QTextCursor::End);
        _pTextEdit->setTextCursor(tc);            
    }

    // Insert the new message
    char buffer[1024];
    sprintf(buffer, "%d: %s", index++, message);
    _pTextEdit->insertPlainText(QString(buffer));

    // Scroll to the bottom
    QScrollBar *sb = _pTextEdit->verticalScrollBar();
    sb->setValue(sb->maximum());
}

void GLFOutputWindow::onSave()
{
    QString saveFilename = QFileDialog::getSaveFileName(NULL, "Save the log to file", QString(), tr("Text files (*.txt)"));

    QFile file(saveFilename);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << _pTextEdit->document()->toPlainText();
}

void GLFOutputWindow::onClear()
{
    _pTextEdit->document()->clear();
        
    QTextCursor tc = _pTextEdit->textCursor();
    tc.movePosition(QTextCursor::End);
    _pTextEdit->setTextCursor(tc);            

    QScrollBar *sb = _pTextEdit->verticalScrollBar();
    sb->setValue(sb->maximum());
}

