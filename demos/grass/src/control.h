// control.h
//
// Created at 2013/12/4
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
    void createGeometryTab(KxColumnLayout* mainLayout);
    void createShadingTabs(KxColumnLayout* mainLayout);

protected slots:
    void onDisplayChanged(const QVariant& index, bool interim);

    // Geometry
    void onShowNormalChanged(const QVariant& value, bool interim);
    void onBladeWidthChanged(const QVariant& value, bool interim);
    void onThicknessThresholdChanged(const QVariant& value, bool interim);

    // Shading
    void onLightingModelChanged(const QVariant& value, bool interim);
    void onTextureChanged(const QVariant& value, bool interim);
};

#endif // !CONTROL_H
