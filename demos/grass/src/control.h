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
class KxFloatSliderGrp;

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

    KxFloatSliderGrp* createSliderGroup(GLfloat minValue, GLfloat maxValue, GLfloat currentValue);

protected slots:
    void onDisplayChanged(const QVariant& index, bool interim);

    // Geometry
    void onBladeHeightChanged(const QVariant& value, bool interim);
    void onShowNormalChanged(const QVariant& value, bool interim);
    void onBladeWidthChanged(const QVariant& value, bool interim);
    void onThicknessThresholdChanged(const QVariant& value, bool interim);

    // Shading
    void onLightingModelChanged(const QVariant& value, bool interim);
    void onTextureChanged(const QVariant& value, bool interim);

    void onAmbientChanged(const QVariant& value, bool interim);
    void onDiffuseChanged(const QVariant& value, bool interim);
    void onSpecularChanged(const QVariant& value, bool interim);
    void onShininessChanged(const QVariant& value, bool interim);

    // Translucent
    void onShowShadowmapChanged(const QVariant& value, bool interim);
    void onTranslucencyChanged(const QVariant& value, bool interim);

    // Ambient occlusion
    void onAmbientOcclusionChanged(const QVariant& value, bool interim);
};

#endif // !CONTROL_H
