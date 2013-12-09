// control.h
//
// Created at 2013/12/9
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved


#ifndef CONTROL_H
#define CONTROL_H

#include <glf/glf.h>

#include <QObject>

class KxColumnLayout;

class Control : public GLFAbstractControl
{
    Q_OBJECT
      
public:
    Control(QWidget* parent);  
    virtual ~Control();

private:
    void createGeneralTab(KxColumnLayout* mainLayout);

protected slots:
    void onModelChanged(const QVariant& index, bool interim);
};

#endif // !CONTROL_H
