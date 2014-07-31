// control.h
//
// Created at 2013/12/11
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved


#ifndef CONTROL_H
#define CONTROL_H

#include <glf/glf.h>

#include <QObject>

class KxColumnLayout;
class KxFloatSliderGrp;
class KxOptionMenu;
class KxCheckBox;

class Control : public GLFAbstractControl
{
    Q_OBJECT
      
public:
    Control(QWidget* parent);  
    virtual ~Control();

    void syncKnotPosition(const glm::vec3& p);

private:
    void createGeneralTab(KxColumnLayout* mainLayout);
    void createIOTab(KxColumnLayout* mainLayout);

    KxFloatSliderGrp* m_xSliderGrp;
    KxFloatSliderGrp* m_ySliderGrp;
    KxFloatSliderGrp* m_bladeWidthSliderGrp;

    KxOptionMenu* m_knotOption;
    KxCheckBox*   m_selfRotatingCheckBox;

protected slots:
    void onModeChanged(const QVariant& index, bool interim);
    void onTypeChanged(const QVariant& index, bool interim);
    void onShowNormalChanged(const QVariant& value, bool interim);
    void onShowGridChanged(const QVariant& value, bool interim);
    void onKnotChanged(const QVariant& value, bool interim);
    void onXChanged(const QVariant& value, bool interim);
    void onYChanged(const QVariant& value, bool interim);

    void onResetButtonClicked(bool checked);
    void onSaveButtonClicked(bool checked);
    void onLoadButtonClicked(bool checked);

    void onBladeWidthChanged(const QVariant& value, bool interim);
    void onSelfRotatingChanged(const QVariant& value, bool interim);
};

#endif // !CONTROL_H
