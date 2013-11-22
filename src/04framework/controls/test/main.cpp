// KxUITest.h
// The main entry of the control testing.
//
// Copyright 2012 by Bruce Wuu. All rights reserved.
//
// Bruce Wuu brucewuu@gmail.com

#include <controls/style/KxDarkStyle.h>

#include <controls/test/KxUITest.h>

int main(int argc, char *argv[])
{
    QApplication::setStyle(new KxDarkStyle);
    QApplication::setDesktopSettingsAware(false);
    QApplication a(argc, argv);

    KxUITest  w;
    w.show();
    return a.exec();
}
