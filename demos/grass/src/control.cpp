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
    
    Renderer* renderer = (Renderer*)glfGetRenderer();

    option->newValueFromUser(0);
    option->setCurrentIndex(renderer->m_renderingSetting.showGeometryOnly? 0 : 1);
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
    
    KxFloatSliderGrp* sliderGrp;

    KxCheckBox* showNormal = new KxCheckBox(QString("normal"));
    formLayout->addRow(new KxLabel(QString("Visible: ")), showNormal);

    connect(showNormal, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onShowNormalChanged(const QVariant&, bool)));
    
    Renderer* renderer = (Renderer*)glfGetRenderer();

    // The height of the blade
    sliderGrp = createSliderGroup(2.0f, 6.0f, renderer->m_geometrySetting.bladeHeight);
    formLayout->addRow(new KxLabel(QString("Blade height: ")), sliderGrp);
    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onBladeHeightChanged(const QVariant&, bool)));

    // The width of the blade
    sliderGrp = createSliderGroup(0.0005f, 0.5f, renderer->m_geometrySetting.bladeWidth);
    formLayout->addRow(new KxLabel(QString("Thickness: ")), sliderGrp);
    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onBladeWidthChanged(const QVariant&, bool)));
    
    // The thickness threshold of the blade
    sliderGrp = createSliderGroup(0.0f, 5.0f, renderer->m_geometrySetting.bladeThicknessThreshold);
    formLayout->addRow(new KxLabel(QString("Thickness thr: ")), sliderGrp);
    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onThicknessThresholdChanged(const QVariant&, bool)));
}

void Control::createShadingTabs(KxColumnLayout* mainLayout)
{
    KxFrameLayout* frameLayout; 
    QFormLayout* formLayout; 
    KxSliderGrp* sliderGrp;
    
    Renderer* renderer = (Renderer*)glfGetRenderer();

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
    
    menuItem = new KxMenuItem(QString("None"), NULL);
    menuItem->setValue(QVariant(0));
    option->menu()->addAction(menuItem);
    option->addItem(menuItem->text());

    menuItem = new KxMenuItem(QString("Phong"), NULL);
    menuItem->setValue(QVariant(Renderer::PHONG));
    option->menu()->addAction(menuItem);
    option->addItem(menuItem->text());
    
    menuItem = new KxMenuItem(QString("Kajiya"), NULL);
    menuItem->setValue(QVariant(Renderer::KAJIYA));
    option->menu()->addAction(menuItem);
    option->addItem(menuItem->text());

    option->newValueFromUser(0);
    option->setCurrentIndex(1); // Use phong lighting as default
    option->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    connect(option, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onLightingModelChanged(const QVariant&, bool)));

    // Lighting
    sliderGrp = createSliderGroup(0.0f, 2.0f, renderer->m_renderingSetting.light.ambient.x);
    formLayout->addRow(new KxLabel(QString("Ambient: ")), sliderGrp);
    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onAmbientChanged(const QVariant&, bool)));
    
    sliderGrp = createSliderGroup(0.0f, 2.0f, renderer->m_renderingSetting.light.diffuse.x);
    formLayout->addRow(new KxLabel(QString("Diffuse: ")), sliderGrp);
    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onDiffuseChanged(const QVariant&, bool)));
    
    sliderGrp = createSliderGroup(0.0f, 2.0f, renderer->m_renderingSetting.light.specular.x);
    formLayout->addRow(new KxLabel(QString("Specular: ")), sliderGrp);
    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onSpecularChanged(const QVariant&, bool)));
    
    sliderGrp = createSliderGroup(0.0f, 10.0f, 5.0f);
    formLayout->addRow(new KxLabel(QString("Shininess: ")), sliderGrp);
    connect(sliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onShininessChanged(const QVariant&, bool)));

    
    // -------------------------------------------------------------- 
    // Texture
    // -------------------------------------------------------------- 
    frameLayout = KxLayoutHelper::addFrameLayout(QString("Texture"), mainLayout);
    formLayout = KxLayoutHelper::addFormLayout(frameLayout);
    
    KxCheckBox* textureCheckBox = new KxCheckBox(QString("on"));
    formLayout->addRow(new KxLabel(QString("Texture: ")), textureCheckBox);
    textureCheckBox->setCheckState(renderer->m_renderingSetting.useTexture? Qt::Checked : Qt::Unchecked);

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
    
    KxCheckBox* checkBox;
    
    checkBox = new KxCheckBox(QString("on"));
    formLayout->addRow(new KxLabel(QString("Translucency: ")), checkBox);
    connect(checkBox, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onTranslucencyChanged(const QVariant&, bool)));
    
    checkBox->setCheckState(renderer->m_renderingSetting.translucency? Qt::Checked : Qt::Unchecked);
    
    checkBox = new KxCheckBox(QString("on"));
    formLayout->addRow(new KxLabel(QString("Shadowmap: ")), checkBox);
    connect(checkBox, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onShowShadowmapChanged(const QVariant&, bool)));
    
    
    // -------------------------------------------------------------- 
    // Antialiasing
    // -------------------------------------------------------------- 
    frameLayout = KxLayoutHelper::addFrameLayout(QString("Antialiasing"), mainLayout);
    formLayout = KxLayoutHelper::addFormLayout(frameLayout);
}

void Control::onDisplayChanged(const QVariant& index, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    if (index.toInt() == 0)
    {
        renderer->m_renderingSetting.showGeometryOnly = true;
    }
    else
    {
        renderer->m_renderingSetting.showGeometryOnly = false;
    }
}

void Control::onShowNormalChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_renderingSetting.showNormal = value.toBool();
}

void Control::onBladeHeightChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->setBladeHeight(value.toFloat());
}
    
void Control::onBladeWidthChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLfloat v = value.toFloat();
    renderer->m_geometrySetting.bladeWidth = v;
}

void Control::onThicknessThresholdChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLfloat v = value.toFloat();
    renderer->m_geometrySetting.bladeThicknessThreshold = v;
}
    
void Control::onLightingModelChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLuint index = value.toInt();
    renderer->m_renderingSetting.lightingMode = index;
}
    
void Control::onTextureChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    bool v = value.toBool();
    renderer->m_renderingSetting.useTexture = v;
}
    
KxFloatSliderGrp* Control::createSliderGroup(GLfloat minValue, GLfloat maxValue, GLfloat currentValue)
{
    KxFloatField* floatField = new KxFloatField;
    KxFloatSlider* floatSlider = new KxFloatSlider(Qt::Horizontal);

    KxFloatSliderGrp* sliderGrp = new KxFloatSliderGrp(NULL, false, floatField, floatSlider);
    sliderGrp->setColumnWidth(0, 60, false);
    sliderGrp->setColumnWidth(1, true);
    sliderGrp->slider()->setMinimumWidth(120);

    sliderGrp->slider()->setSliderRangeF(minValue, maxValue);
    sliderGrp->slider()->setValueF(currentValue);
    sliderGrp->inputField()->setMinValueF(minValue);
    sliderGrp->inputField()->setMaxValueF(maxValue);
    sliderGrp->inputField()->setValueF(currentValue);

    return sliderGrp;
}
    
void Control::onAmbientChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLfloat v = value.toFloat();
    renderer->m_renderingSetting.light.ambient = glm::vec4(v, v, v, 1.0f);
}

void Control::onDiffuseChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLfloat v = value.toFloat();
    renderer->m_renderingSetting.light.diffuse = glm::vec4(v, v, v, 1.0f);
}

void Control::onSpecularChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLfloat v = value.toFloat();
    renderer->m_renderingSetting.light.specular = glm::vec4(v, v, v, 1.0f);
}

void Control::onShininessChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GLfloat v = value.toFloat();
    renderer->m_renderingSetting.light.shininess = v;
}
    
void Control::onShowShadowmapChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    bool v = value.toBool();
    renderer->m_renderingSetting.smShow = v;
}

void Control::onTranslucencyChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    bool v = value.toBool();
    renderer->m_renderingSetting.translucency = v;
}
