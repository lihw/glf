// KxUITest.h
// The main window of control test.
//
// Copyright 2012 by Bruce Wuu. All rights reserved.
//
// Bruce Wuu brucewuu@gmail.com


#ifndef KxUITest_H_
#define KxUITest_H_

class KxFormLayout;
class KxTabLayout;
class KxCheckBox;
class KxLabel;

#include <controls/base/HQtGui.h>

#include <user/kw_property_headers.h>

class KxUITest : public QWidget
{
    Q_OBJECT

public:
    KxUITest(QWidget* parent = 0);

    ~KxUITest();


public slots:
    void qtDisableAll();

    void qtEnableAll();

protected:
    virtual void timerEvent(QTimerEvent* event);

private:
    void showText(const QString &text);

    void testPropertyWidgets();

private:
    KxFormLayout * fFormLayout;
    KxTabLayout * fTabLayout;
    QWidget *fTabWidget;
    KxCheckBox * fDisableAllCheckBox;
    QWidget * fFirstTabWidget;

    KwPropertyBool _propertyBool;
    KwPropertyFloat _propertyFloat;
    KwPropertyInt _propertyInt;
    KwPropertyUint _propertyUint;
    KwPropertyString _propertyString;
    KwPropertyEnum _propertyEnum;

    KwPropertySRTMatrix _propertySRTMatrix;
    KwPropertyColor _propertyColor;
    //KwPropertyRTMatrix _propertyRTMatrix;

    KxLabel* _boolValueLabel;
    KxLabel* _intValueLabel;
    KxLabel* _uintValueLabel;
    KxLabel* _floatValueLabel;
    KxLabel* _stringValueLabel;
    KxLabel* _enumValueLabel;

    KxLabel* _srtmatrixValueLabel;
    KxLabel* _rtmatrixValueLabel;
    KxLabel* _colorValueLabel;

    QTimer* _timer;
};

#endif // KxUITest_H_
