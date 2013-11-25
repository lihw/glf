// -------------------------------------------------------------- 
// glf_output_window.h
// The output log window
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 

#ifndef GLF_OUTPUT_WINDOW_H
#define GLF_OUTPUT_WINDOW_H

#include <QDockWidget>

#include "controls/widgets/window/KxDockWidget.h"

class GLFOutputWindow : public KxDockWidget
{
    Q_OBJECT

public:
    GLFOutputWindow(QWidget* parent);

    void printMessage(const char* message);

protected slots:
    void onSave();
    void onClear();

private:
    QTextEdit* _pTextEdit;
    QPushButton* _pSaveButton;
    QPushButton* _pClearButton;

    static const int MAX_NUM_LINES = 16;
};

#endif 

