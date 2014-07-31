// control.cpp
//
// Created at 2013/12/4
//
// Hongwei Li hongwei.li@amd.com
// All rights reserved

#include "control.h"

#include "renderer.h"
#include "grassblade.h"

#include <glf/glf_controls.h>

Control::Control(QWidget* parent)
    : GLFAbstractControl(parent)
{
    KxColumnLayout* mainLayout = new KxColumnLayout(parent);
    parent->setLayout(mainLayout);
    mainLayout->setColumnAdjustable(true);
    
    createGeneralTab(mainLayout);
    createIOTab(mainLayout);
}

Control::~Control()
{
}

void Control::createGeneralTab(KxColumnLayout* mainLayout)
{
    KxFrameLayout* frameLayout = KxLayoutHelper::addFrameLayout(QString("General"), mainLayout);
    QFormLayout* formLayout = KxLayoutHelper::addFormLayout(frameLayout);

    // Mode
    QWidget *radiobuttonWidget = new QWidget;
    KxRowLayout *radiobuttonLayout = new KxRowLayout(radiobuttonWidget);
    radiobuttonLayout->setNumColumns(2);

    KxRadioButton *radiobutton1 = new KxRadioButton("edit");
    KxRadioButton *radiobutton2 = new KxRadioButton("view");
    KxButtonGroup *buttonGroup = new KxButtonGroup;
    buttonGroup->addButton(radiobutton1, 0);
    buttonGroup->addButton(radiobutton2, 1);

    radiobutton1->toggle();
        
    radiobuttonLayout->addWidget(radiobutton1);
    radiobuttonLayout->addWidget(radiobutton2);
    formLayout->addRow(new KxLabel(QString("Mode: ")), radiobuttonWidget);
    
    connect(buttonGroup, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onModeChanged(const QVariant&, bool)));
    

    // Type
    KxOptionMenu* option = new KxOptionMenu;
    formLayout->addRow(new KxLabel(QString("Type: ")), option);
    
    option->menu()->clear();
    option->clear();
    
    KxMenuItem* menuItem;

    for (int i = 0; i < Renderer::NUM_BLADES; ++i)
    {
        menuItem = new KxMenuItem(QString("%1").arg(i), NULL);
        menuItem->setValue(QVariant(i));
        option->menu()->addAction(menuItem);
        option->addItem(menuItem->text());
    }
    
    option->newValueFromUser(0);
    option->setCurrentIndex(0);
    option->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    connect(option, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onTypeChanged(const QVariant&, bool)));
    
    // Normal
    KxCheckBox* showNormal = new KxCheckBox(QString("normal"));
    formLayout->addRow(new KxLabel(QString("Visible: ")), showNormal);

    connect(showNormal, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onShowNormalChanged(const QVariant&, bool)));
    
    // Grid
    KxCheckBox* showGrid = new KxCheckBox(QString("grid"));
    formLayout->addRow(new KxLabel(QString("")), showGrid);

    connect(showGrid, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onShowGridChanged(const QVariant&, bool)));
    
    // Knot Changed
    m_knotOption = new KxOptionMenu;
    formLayout->addRow(new KxLabel(QString("Knot: ")), m_knotOption);
    
    m_knotOption->menu()->clear();
    m_knotOption->clear();
    
    for (int i = 1; i < BLADE_NUM_KNOTS; ++i)
    {
        menuItem = new KxMenuItem(QString("%1").arg(i), NULL);
        menuItem->setValue(QVariant(i));
        m_knotOption->menu()->addAction(menuItem);
        m_knotOption->addItem(menuItem->text());
    }
    
    m_knotOption->newValueFromUser(0);
    m_knotOption->setCurrentIndex(0);
    m_knotOption->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    
    connect(m_knotOption, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onKnotChanged(const QVariant&, bool)));

    // X and Y
    KxFloatField* floatField = new KxFloatField;
    KxFloatSlider* floatSlider = new KxFloatSlider(Qt::Horizontal);
    m_xSliderGrp = new KxFloatSliderGrp(NULL, false, floatField, floatSlider);
    m_xSliderGrp->setColumnWidth(0, 60, false);
    m_xSliderGrp->setColumnWidth(1, true);
    m_xSliderGrp->slider()->setMinimumWidth(240);

    m_xSliderGrp->slider()->setSliderRangeF(-5.0f, 5.0f);
    m_xSliderGrp->slider()->setValueF(0.0f);
    m_xSliderGrp->inputField()->setMinValueF(-5.0f);
    m_xSliderGrp->inputField()->setMaxValueF(5.0f);
    m_xSliderGrp->inputField()->setValueF(0.f);
    
    formLayout->addRow(new KxLabel(QString("X: ")), m_xSliderGrp);

    connect(m_xSliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onXChanged(const QVariant&, bool)));
    
    floatField = new KxFloatField;
    floatSlider = new KxFloatSlider(Qt::Horizontal);
    m_ySliderGrp = new KxFloatSliderGrp(NULL, false, floatField, floatSlider);
    m_ySliderGrp->setColumnWidth(0, 60, false);
    m_ySliderGrp->setColumnWidth(1, true);
    m_ySliderGrp->slider()->setMinimumWidth(240);

    m_ySliderGrp->slider()->setSliderRangeF(-5.0f, 5.0f);
    m_ySliderGrp->slider()->setValueF(0.0f);
    m_ySliderGrp->inputField()->setMinValueF(-5.0f);
    m_ySliderGrp->inputField()->setMaxValueF(5.0f);
    m_ySliderGrp->inputField()->setValueF(0.f);
    
    formLayout->addRow(new KxLabel(QString("Y: ")), m_ySliderGrp);

    connect(m_ySliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onYChanged(const QVariant&, bool)));

    // Expansion width
    floatField = new KxFloatField;
    floatSlider = new KxFloatSlider(Qt::Horizontal);
    m_bladeWidthSliderGrp = new KxFloatSliderGrp(NULL, false, floatField, floatSlider);
    m_bladeWidthSliderGrp->setColumnWidth(0, 60, false);
    m_bladeWidthSliderGrp->setColumnWidth(1, true);
    m_bladeWidthSliderGrp->slider()->setMinimumWidth(240);

    m_bladeWidthSliderGrp->slider()->setSliderRangeF(0, 1.0f);
    m_bladeWidthSliderGrp->slider()->setValueF(0.08f);
    m_bladeWidthSliderGrp->inputField()->setMinValueF(0.0f);
    m_bladeWidthSliderGrp->inputField()->setMaxValueF(1.0f);
    m_bladeWidthSliderGrp->inputField()->setValueF(0.08f);
    
    formLayout->addRow(new KxLabel(QString("Blade width: ")), m_bladeWidthSliderGrp);

    connect(m_bladeWidthSliderGrp, 
        SIGNAL(newValueForConnections(const QVariant&, bool)), 
        this,
        SLOT(onBladeWidthChanged(const QVariant&, bool)));
    
    m_selfRotatingCheckBox = new KxCheckBox(QString("On"));
    formLayout->addRow(new KxLabel(QString("Self rotating: ")), m_selfRotatingCheckBox);

    connect(m_selfRotatingCheckBox, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(onSelfRotatingChanged(const QVariant&, bool)));
}
    
void Control::createIOTab(KxColumnLayout* mainLayout)
{
    QWidget* widget = new QWidget;
    mainLayout->addWidget(widget);

    QHBoxLayout* layout = new QHBoxLayout(widget);
    
    QPushButton* button;

    button = new QPushButton("Reset");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(onResetButtonClicked(bool)));
    layout->addWidget(button);
    button = new QPushButton("Save");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(onSaveButtonClicked(bool)));
    layout->addWidget(button);
    button = new QPushButton("Load");
    connect(button, SIGNAL(clicked(bool)), this, SLOT(onLoadButtonClicked(bool)));
    layout->addWidget(button);

    //layout->setMarginWidth(5);
    //layout->setMarginHeight(2);
}

void Control::onModeChanged(const QVariant& index, bool interim)
{
    if (index.toInt() == 0) // edit
    {
        m_xSliderGrp->setEnabled(true);
        m_ySliderGrp->setEnabled(true);
        m_knotOption->setEnabled(true);
        m_bladeWidthSliderGrp->setEnabled(false);
        m_selfRotatingCheckBox->setEnabled(false);

        Renderer* renderer = (Renderer*)glfGetRenderer();
        renderer->m_mode = Renderer::EDIT_MODE;
    }
    else
    {
        m_xSliderGrp->setEnabled(false);
        m_ySliderGrp->setEnabled(false);
        m_knotOption->setEnabled(false);

        m_bladeWidthSliderGrp->setEnabled(true);
        m_selfRotatingCheckBox->setEnabled(true);
        
        Renderer* renderer = (Renderer*)glfGetRenderer();
        renderer->m_mode = Renderer::VIEW_MODE;
    }
}

void Control::onTypeChanged(const QVariant& index, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_currentBladeIndex = index.toInt();
}

void Control::onShowNormalChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_showNormal = value.toBool();
}

void Control::onShowGridChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_showGrid = value.toBool();
}
    
void Control::onKnotChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_currentBladeKnotIndex = value.toInt();
    renderer->m_grassBladeKnotShader.getUniform("CurrentVertexId")->setValue(GLuint(value.toInt()));

    // Update the x and y position
    GrassBlade* blade = renderer->m_grassBlades[renderer->m_currentBladeIndex];
    glm::vec3 p = blade->getKnotPosition(renderer->m_currentBladeKnotIndex);

    m_xSliderGrp->slider()->setValueF(p.x);
    m_xSliderGrp->inputField()->setValueF(p.x);
    m_ySliderGrp->slider()->setValueF(p.y);
    m_ySliderGrp->inputField()->setValueF(p.y);
}

void Control::onXChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GrassBlade* blade = renderer->m_grassBlades[renderer->m_currentBladeIndex];
    glm::vec3 p = blade->getKnotPosition(renderer->m_currentBladeKnotIndex);
    p.x = value.toFloat();
    blade->setKnotPosition(renderer->m_currentBladeKnotIndex, p);
}

void Control::syncKnotPosition(const glm::vec3& p)
{
    m_xSliderGrp->slider()->setValueF(p.x);
    m_xSliderGrp->inputField()->setValueF(p.x);
    m_ySliderGrp->slider()->setValueF(p.y);
    m_ySliderGrp->inputField()->setValueF(p.y);
}

void Control::onYChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    GrassBlade* blade = renderer->m_grassBlades[renderer->m_currentBladeIndex];
    glm::vec3 p = blade->getKnotPosition(renderer->m_currentBladeKnotIndex);
    p.y = value.toFloat();
    blade->setKnotPosition(renderer->m_currentBladeKnotIndex, p);
}

void Control::onSaveButtonClicked(bool checked)
{
    FILE* fp = fopen("blade.txt", "wb");
    if (fp == NULL)
    {
        GLF_LOGWARNING("Failed to open blade.txt for outputing blade data");
        return ;
    }

    Renderer* renderer = (Renderer*)glfGetRenderer();
    for (GLuint i = 0; i < Renderer::NUM_BLADES; ++i)
    {
        std::string text;
        GrassBlade* blade = renderer->m_grassBlades[i];
        blade->serialize(text);
        
        fprintf(fp, "%s\n", text.c_str());
    }

    fclose(fp);
}

void Control::onLoadButtonClicked(bool checked)
{
    FILE* fp = fopen("blade.txt", "rb");
    if (fp == NULL)
    {
        GLF_LOGWARNING("Failed to open blade.txt for reading blade data");
        return ;
    }

    GLuint lineno = 1;
    Renderer* renderer = (Renderer*)glfGetRenderer();
    for (GLuint i = 0; i < Renderer::NUM_BLADES; ++i)
    {
        char line[1024];
        if (fgets(line, 1024, fp) == NULL)
        {
            GLF_LOGWARNING("Error in reading line %d", lineno);
            break;
        }

        std::string text(line);
        GrassBlade* blade = renderer->m_grassBlades[i];
        blade->unserialize(text);

        lineno++;
    }

    fclose(fp);
}
    
void Control::onResetButtonClicked(bool checked)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();

    for (GLuint i = 0; i < Renderer::NUM_BLADES; ++i)
    {
        GrassBlade* blade = renderer->m_grassBlades[i];
        blade->resetShape();
        
        glm::vec3 pos = blade->getKnotPosition(renderer->m_currentBladeKnotIndex);
        m_xSliderGrp->slider()->setValueF(pos.x);
        m_xSliderGrp->inputField()->setValueF(pos.x);
        m_ySliderGrp->slider()->setValueF(pos.y);
        m_ySliderGrp->inputField()->setValueF(pos.y);
    }
}
    
void Control::onBladeWidthChanged(const QVariant& value, bool interim)
{
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_grassBladeViewShader.getUniform("BladeWidth")->setValue(value.toFloat());
}
            
void Control::onSelfRotatingChanged(const QVariant& value, bool interim)
{
    bool v = value.toBool();
    Renderer* renderer = (Renderer*)glfGetRenderer();
    renderer->m_selfRotating = v;
}
