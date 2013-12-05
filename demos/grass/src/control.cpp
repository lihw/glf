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
    createGeometryExpansionTab(mainLayout);
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

void Control::createGeometryExpansionTab(KxColumnLayout* mainLayout)
{
    // Show the normal
    KxFrameLayout* frameLayout = KxLayoutHelper::addFrameLayout(QString("Geometry Expansion"), mainLayout);
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
