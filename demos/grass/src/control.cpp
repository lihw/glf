// control.cpp
//
// Created at 2013/12/4
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "control.h"

#include "renderer.h"

#include <glf/glf_controls.h>

Control::Control(QWidget* parent)
    : GLFAbstractControl(parent)
{
    KxColumnLayout* mainLayout = new KxColumnLayout(parent);
    parent->setLayout(mainLayout);
    mainLayout->setColumnAdjustable(true);
    
    createGeneralTab(mainLayout);
    createGeometryTab(mainLayout);
    createShadingTabs(mainLayout);
}

Control::~Control()
{
}

void Control::createGeneralTab(KxColumnLayout* mainLayout)
{
    KxFrameLayout* frameLayout = KxLayoutHelper::addFrameLayout(QString("General"), mainLayout);
    QFormLayout* formLayout = KxLayoutHelper::addFormLayout(frameLayout);

    KxOptionMenu* option = new KxOptionMenu;
    formLayout->addRow(new KxLabel(QString("Display: ")), option);
    
    option->menu()->clear();
    option->clear();
    
    KxMenuItem* menuItem;

    menuItem = new KxMenuItem(QString("Stem"), NULL);
    menuItem->setValue(QVariant(0));
    option->menu()->addAction(menuItem);
    option->addItem(menuItem->text());
    
    menuItem = new KxMenuItem(QString("Blade"), NULL);
    menuItem->setValue(QVariant(1));
    option->menu()->addAction(menuItem);
    option->addItem(menuItem->text());

    option->newValueFromUser(0);
    option->setCurrentIndex(0);
    option->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    connect(option, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onDisplayChanged(const QVariant&, bool)));
}

void Control::createGeometryTab(KxColumnLayout* mainLayout)
{
    // Show the normal
    KxFrameLayout* frameLayout = KxLayoutHelper::addFrameLayout(QString("Geometry"), mainLayout);
    QFormLayout* formLayout = KxLayoutHelper::addFormLayout(frameLayout);

    KxCheckBox* showNormal = new KxCheckBox(QString("normal"));
    formLayout->addRow(new KxLabel(QString("Visible: ")), showNormal);

    connect(showNormal, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onShowNormalChanged(const QVariant&, bool)));

    // The width of the blade
    KxFloatField* floatField = new KxFloatField;
    KxFloatSlider* floatSlider = new KxFloatSlider(Qt::Horizontal);
    KxFloatSliderGrp* sliderGrp = new KxFloatSliderGrp(NULL, false, floatField, floatSlider);
    sliderGrp->setColumnWidth(0, 60, false);
    sliderGrp->setColumnWidth(1, true);
    sliderGrp->slider()->setMinimumWidth(120);

    sliderGrp->slider()->setSliderRangeF(0.005f, 0.2f);
    sliderGrp->slider()->setValueF(0.01f);
    sliderGrp->inputField()->setMinValueF(0.005);
    sliderGrp->inputField()->setMaxValueF(0.5f);
    sliderGrp->inputField()->setValueF(0.01);
    
    formLayout->addRow(new KxLabel(QString("Blade Width: ")), sliderGrp);

    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onBladeWidthChanged(const QVariant&, bool)));

    
    // The threshold of thickness
    floatField = new KxFloatField;
    floatSlider = new KxFloatSlider(Qt::Horizontal);
    sliderGrp = new KxFloatSliderGrp(NULL, false, floatField, floatSlider);
    sliderGrp->setColumnWidth(0, 60, false);
    sliderGrp->setColumnWidth(1, true);
    sliderGrp->slider()->setMinimumWidth(120);

    sliderGrp->slider()->setSliderRangeF(1.0f, 10.0f);
    sliderGrp->slider()->setValueF(5.00f);
    sliderGrp->inputField()->setMinValueF(1.0f);
    sliderGrp->inputField()->setMaxValueF(10.0f);
    sliderGrp->inputField()->setValueF(5.00);
    
    formLayout->addRow(new KxLabel(QString("Thickness: ")), sliderGrp);

    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onThicknessThresholdChanged(const QVariant&, bool)));
}

void Control::createShadingTabs(KxColumnLayout* mainLayout)
{
    KxFrameLayout* frameLayout; 
    QFormLayout* formLayout; 

    // -------------------------------------------------------------- 
    // Lighting
    // -------------------------------------------------------------- 
    frameLayout = KxLayoutHelper::addFrameLayout(QString("Lighting"), mainLayout);
    formLayout = KxLayoutHelper::addFormLayout(frameLayout);
    
    KxOptionMenu* option = new KxOptionMenu;
    formLayout->addRow(new KxLabel(QString("Lighting: ")), option);
    
    option->menu()->clear();
    option->clear();
    
    KxMenuItem* menuItem;

    menuItem = new KxMenuItem(QString("Phong"), NULL);
    menuItem->setValue(QVariant(0));
    option->menu()->addAction(menuItem);
    option->addItem(menuItem->text());
    
    menuItem = new KxMenuItem(QString("Kajiya"), NULL);
    menuItem->setValue(QVariant(1));
    option->menu()->addAction(menuItem);
    option->addItem(menuItem->text());

    option->newValueFromUser(0);
    option->setCurrentIndex(0);
    option->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    connect(option, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onLightingModelChanged(const QVariant&, bool)));
    
    // -------------------------------------------------------------- 
    // Texture
    // -------------------------------------------------------------- 
    frameLayout = KxLayoutHelper::addFrameLayout(QString("Texture"), mainLayout);
    formLayout = KxLayoutHelper::addFormLayout(frameLayout);
    
    KxCheckBox* textureCheckBox = new KxCheckBox(QString("on"));
    formLayout->addRow(new KxLabel(QString("Texture: ")), textureCheckBox);

    connect(textureCheckBox, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onTextureChanged(const QVariant&, bool)));
    
    // -------------------------------------------------------------- 
    // Shadow
    // -------------------------------------------------------------- 
    frameLayout = KxLayoutHelper::addFrameLayout(QString("Shadow"), mainLayout);
    formLayout = KxLayoutHelper::addFormLayout(frameLayout);
    
    // -------------------------------------------------------------- 
    // Translucency
    // -------------------------------------------------------------- 
    frameLayout = KxLayoutHelper::addFrameLayout(QString("Translucency"), mainLayout);
    formLayout = KxLayoutHelper::addFormLayout(frameLayout);
    
    // -------------------------------------------------------------- 
    // Antialiasing
    // -------------------------------------------------------------- 
    frameLayout = KxLayoutHelper::addFrameLayout(QString("Antialiasing"), mainLayout);
    formLayout = KxLayoutHelper::addFormLayout(frameLayout);
}

void Control::onDisplayChanged(const QVariant& index, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_currentGrassShader = &renderer->m_grassShaders[index.toInt()];
}

void Control::onShowNormalChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_grassShowNormal = value.toBool();
}
    
void Control::onBladeWidthChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLfloat bladeWidth = value.toFloat();
    renderer->m_grassShaders[Renderer::COLOR].getUniform("BladeWidth")->setValue(bladeWidth);
}

void Control::onThicknessThresholdChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLfloat v = value.toFloat();
    renderer->m_grassShaders[Renderer::COLOR].getUniform("ThicknessThreshold")->setValue(v);
}
    
void Control::onLightingModelChanged(const QVariant& value, bool interim)
{
}
    
void Control::onTextureChanged(const QVariant& value, bool interim)
{
}
