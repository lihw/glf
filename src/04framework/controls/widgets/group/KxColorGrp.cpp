
#include "KxColorGrp.h"

#include "../../layouts/KxRowLayout.h"
#include "../../layouts/KxColumnLayout.h"
#include "../display/KxLabel.h"
#include "../input/KxField.h"

KxColorGrp::KxColorGrp(QWidget* parent)
{
    fColor = QColor(255, 0, 0, 255);

    QWidget* colorLabel = new QWidget;
    addWidgetToGrp(colorLabel);

    QWidget* colorPicker = new QWidget;
    addWidgetToGrp(colorPicker);

    setColumnAlignment(0, Qt::AlignCenter | Qt::AlignVCenter);
    setColumnAlignment(1, Qt::AlignCenter | Qt::AlignVCenter);

    // Label
    KxColumnLayout* columnLayout = new KxColumnLayout(colorLabel);
    columnLayout->setSpacing(2);
    colorLabel->setLayout(columnLayout);
    columnLayout->setColumnAdjustable(true);
    
    fCanvas = new QLabel();
    fCanvas->setFixedSize(48, 48);
    columnLayout->addWidget(fCanvas);

    fColorField = new KxField();
    fColorField->setMaximumWidth(48);
    columnLayout->addWidget(fColorField);
    
    connect(fColorField, 
            SIGNAL(changeCompleted()), 
            this,
            SLOT(newColorFromColorField()));

    // color picker.
    QFormLayout* formLayout = new QFormLayout(colorPicker);
    formLayout->setContentsMargins(4, 0, 2, 0);
    formLayout->setSpacing(0);

    fRedSliderGrp = createColorSlider(fColor.red());
    fGreenSliderGrp = createColorSlider(fColor.green());
    fBlueSliderGrp = createColorSlider(fColor.blue());
    fAlphaSliderGrp = createColorSlider(fColor.alpha());

    formLayout->addRow(new KxLabel(QString("R:")), fRedSliderGrp);
    formLayout->addRow(new KxLabel(QString("G:")), fGreenSliderGrp);
    formLayout->addRow(new KxLabel(QString("B:")), fBlueSliderGrp);
    formLayout->addRow(new KxLabel(QString("A:")), fAlphaSliderGrp);

    connect(fRedSliderGrp, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(newColorFromSliderGrp(const QVariant&, bool)));
    connect(fGreenSliderGrp, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(newColorFromSliderGrp(const QVariant&, bool)));
    connect(fBlueSliderGrp, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(newColorFromSliderGrp(const QVariant&, bool)));
    connect(fAlphaSliderGrp, 
            SIGNAL(newValueForConnections(const QVariant&, bool)), 
            this,
            SLOT(newColorFromSliderGrp(const QVariant&, bool)));
    
    
    updateCanvas();
}
    
void KxColorGrp::newValueFromConnection(const QVariant& value)
{
}
    
void KxColorGrp::updateCanvas()
{
    QPalette canvasPalette(fColor);
    fCanvas->setPalette(canvasPalette);
    fCanvas->setAutoFillBackground(true);

    fColorField->setText(fColor.name());
}
        
void KxColorGrp::updatePicker()
{
    fRedSliderGrp->inputField()->setValue(fColor.red());
    fGreenSliderGrp->inputField()->setValue(fColor.green());
    fBlueSliderGrp->inputField()->setValue(fColor.blue());
    fAlphaSliderGrp->inputField()->setValue(fColor.alpha());

    fRedSliderGrp->slider()->setValueF(fColor.red());
    fGreenSliderGrp->slider()->setValueF(fColor.green());
    fBlueSliderGrp->slider()->setValueF(fColor.blue());
    fAlphaSliderGrp->slider()->setValueF(fColor.alpha());
}
    
KxIntSliderGrp* KxColorGrp::createColorSlider(int currentValue)
{
    KxIntSlider* intSlider;
    KxIntField* intField;

    intSlider = new KxIntSlider(Qt::Horizontal);
    intSlider->setSliderRangeF(0, 255);
    intSlider->setValueF(currentValue);
    
    intField = new KxIntField;
    intField->setMinValue(0);
    intField->setMaxValue(255);
    intField->setValueF(currentValue);

    KxIntSliderGrp* sliderGrp = new KxIntSliderGrp(NULL, false, intField, intSlider);
    sliderGrp->setColumnWidth(0, 40, false);
    sliderGrp->setColumnWidth(1, true);
    sliderGrp->slider()->setMinimumWidth(180);

    return sliderGrp;
}
    
void KxColorGrp::newColorFromSliderGrp(const QVariant& v, bool isInterim)
{
    QColor color(
            fRedSliderGrp->inputField()->value(),
            fGreenSliderGrp->inputField()->value(),
            fBlueSliderGrp->inputField()->value(),
            fAlphaSliderGrp->inputField()->value());

    if (color != fColor)
    {
        fColor = color;
        updateCanvas();

        emit newValueForConnections(QVariant(fColor.rgba()), false);
    }
}
    
void KxColorGrp::newColorFromColorField()
{
    QColor color(fColorField->text());
    if (color != fColor)
    {
        fColor = color;

        updatePicker();
        updateCanvas();

        emit newValueForConnections(QVariant(fColor.rgba()), false);
    }
}
    
const QColor KxColorGrp::color() const
{
    return fColor;
}

void KxColorGrp::setColor(const QColor& newColor)
{
    if (fColor != newColor)
    {
        fColor = newColor;
        updateCanvas();
        updatePicker();
    }
}
