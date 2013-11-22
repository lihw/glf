// -------------------------------------------------------------- 
// main.cpp
// The include header of glf
//
// An OpenGL framework.
//
// All rights reserved by AMD.
//
// Hongwei Li (hongwei.li@amd.com)
// -------------------------------------------------------------- 


#include "controls/style/KxDarkStyle.h"

#include "glf_main_window.h"
#include "glf_application.h"
#include "glf_config.h"

int main(int argc, char** argv)
{
    QApplication::setStyle(new KxDarkStyle);
    QApplication::setDesktopSettingsAware(false);

    GLFApplication app(argc, argv);
    GLFConfig config;
    GLFMainWindow w(config);
    app.setMainWindow(&w);

    w.show();

    return app.exec();
}
