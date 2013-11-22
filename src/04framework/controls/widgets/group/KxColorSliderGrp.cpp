
#include "KxColorSliderGrp.h"
#include "../../color/KxPortColor.h"
#include "../slider/KxSlider.h"
#include "../display/KxLabel.h"


#define kLabelName			"color"
#define kPortName			"port"
#define kSliderName			"slider"

KxColorSliderLabel::KxColorSliderLabel(QWidget * parent )
: QLabel(parent)
, fDoubleClicked(false)
, fDoubleClickTimer(this)
{
    connect(&fDoubleClickTimer, SIGNAL(timeout()), this, SLOT(singleClickResponder()));
}

KxColorSliderLabel::~KxColorSliderLabel()
{
}

void KxColorSliderLabel::mousePressEvent( QMouseEvent * )
{
    if (!fDoubleClickTimer.isActive())
    {
        fTime.restart();
    }
}

void KxColorSliderLabel::mouseReleaseEvent( QMouseEvent * )
{
    if (!fDoubleClickTimer.isActive())
    {
        int dblClickTime = QApplication::doubleClickInterval();
        int elapsed = fTime.elapsed();

        if (2*elapsed > dblClickTime)
        {
            singleClickResponder();
        }
        else
        {
            fDoubleClickTimer.start(2*elapsed);
        }
    }
}

void KxColorSliderLabel::mouseDoubleClickEvent( QMouseEvent * )
{
    fDoubleClickTimer.stop();
    fDoubleClicked = true;
    emit doubleClicked();
}

void KxColorSliderLabel::singleClickResponder()
{
    if (!fDoubleClicked)
    {
        emit singleClicked();
    }

    fDoubleClickTimer.stop();
    fDoubleClicked = false;
}

// Description:	 Constructor
//
KxColorSliderGrp::KxColorSliderGrp(QWidget *parent, bool hasLabel, KxColorSliderLabel * newCanvas, KxSlider *newSlider)
: KxConnectableWidgetGrp(parent)
, fReadOnly(false)
, fSlider(newSlider)
, fCanvas(newCanvas)
, fChangeCompleteRequired(false)
, fRValue(0.0)
, fGValue(0.0)
, fBValue(0.0)
{
    // Add a label.
    if(hasLabel) {
        KxLabel *label = new KxLabel();
        label->setObjectName(kLabelName);
        addLabel(label);
    }

    createRemainingControls(newCanvas, newSlider);
}

// Description:	 Constructor
//
KxColorSliderGrp::KxColorSliderGrp(QWidget *parent, KxLabel* newLabel, KxColorSliderLabel * newCanvas, KxSlider *newSlider)
: KxConnectableWidgetGrp(parent)
, fReadOnly(false)
, fSlider(newSlider)
, fCanvas(newCanvas)
, fChangeCompleteRequired(false)
, fRValue(0.0)
, fGValue(0.0)
, fBValue(0.0)
{
    // Add a label.
    if (newLabel != NULL) {
        addLabel(newLabel);
    }

    createRemainingControls(newCanvas, newSlider);
}

void KxColorSliderGrp::createRemainingControls(KxColorSliderLabel* canvas, KxSlider* slider)
{
    // Hook up the signal and slot connections.
    if (slider)
    {
        if (canvas)
        {
            if(canvas->objectName().isNull()) {
                canvas->setObjectName(kPortName);
            }
            // Put in a color canvas.
            addWidgetToGrp(canvas);

            // We have to post a color chooser when canvas button is clicked
            connect(fCanvas, SIGNAL(singleClicked()), this, SLOT(canvasSingleClicked()));
            connect(fCanvas, SIGNAL(doubleClicked()), this, SLOT(canvasDoubleClicked()));
        }

        if(slider->objectName().isNull()) {
            slider->setObjectName(kSliderName);
        }
        // Put in an integer slider.
        addWidgetToGrp(slider);

        // Handle value changes while the slider is moving.
        connect(
            slider,	SIGNAL(transientSliderChange()),
            this,	SLOT(handleTransientSliderValue())
            );

        // Handle the final value from a slider drag or click.
        connect(
            slider,	SIGNAL(sliderChangeCompleted()),
            this,	SLOT(handleFinalSliderValue())
            );
    }
}

const QColor KxColorSliderGrp::color() const
{
    QColor qcolor;
    qcolor.setRgbF((qreal)qBound(0.0, fRValue, 1.0), (qreal)qBound(0.0, fGValue, 1.0), (qreal)qBound(0.0, fBValue, 1.0));
    return qcolor;
}

void KxColorSliderGrp::setColor(const QColor &newColor)
{
    fRValue = newColor.redF();
    fGValue = newColor.greenF();
    fBValue = newColor.blueF();
    updateCanvas();
    useColorValueToSetSliderValue();
}

// Description:	 This procedure is called after the user editted
//				 the field.	 We want to use the field value to 
//				 set the slider value.
//
void KxColorSliderGrp::useColorValueToSetSliderValue()
{
    /*
    KxSlider * theSlider = mayaSlider();

    if(theSlider ==NULL) {
        return;
    }

    if(theSlider->sliderValueF() == fValue) {
        return;
    }

    // If the slider has a more limited range than the float 
    // field, and the new value lies outside that range, 
    // then we can increase the range of the slider to 
    // include the new value, so long as the increase keeps 
    // it within the range of the float field.
    //
    double sliderMin = theSlider->sliderMinimumF();
    double sliderMax = theSlider->sliderMaximumF();

    if ( fValue < sliderMin ){

        if ( fValue < 0.0 ){
            sliderMin = 1.2 * fValue;
        } else {
            // The new value is greater than 0.	 Increase the range
            // downwards by a distance of 1/2 the current range.
            //
            sliderMin -= (sliderMax-sliderMin)*0.5;
        }

        theSlider->setSliderRangeF(sliderMin,sliderMax);

    } else if ( fValue > sliderMax ){

        if ( fValue > 0 ){
            sliderMax = 1.2 * fValue;
        } else {
            // The new value is less than 0.  Increase the range
            // upwards by a distance of 1/2 the current range.
            //
            sliderMax += (sliderMax-sliderMin)*0.5;
        }

        theSlider->setSliderRangeF(sliderMin,sliderMax);
    }

    theSlider->setSliderValueF(fValue);
    */
}

void KxColorSliderGrp::canvasSingleClicked()
{
    QColor qc = canvas()->palette().color(QPalette::Background);
    qreal	r, g, b, a;
    qc.getRgbF(&r, &g, &b, &a);
    //QColor initialColor((float)r, (float)g, (float)b, (float)a);

    //KxNoAttrColorPicker * largeColorPickerDialog = 
    //    KxNoAttrColorPicker::getNoAttrColorEditor(false);
    //if (largeColorPickerDialog->isVisible())
    //{
    //    largeColorPickerDialog->addingConnections(this);

    //    largeColorPickerDialog->setModal(true);
    //    largeColorPickerDialog->setInitialColor(QColor(initialColor));
    //    connect(largeColorPickerDialog, SIGNAL(colorAccepted(const QColor &)), this, SLOT(newColor(const QColor &)));
    //    connect(largeColorPickerDialog, SIGNAL(colorRejected(const QColor &)), this, SLOT(colorRejected(const QColor &)));
    //    connect(largeColorPickerDialog, SIGNAL(interactiveEdit(const QColor &)), this, SLOT(handleTransientCanvasValue(const QColor &)));
    //    connect(largeColorPickerDialog, SIGNAL(colorChanged(const QColor &)), this, SLOT(handleFinalCanvasValue(const QColor &)));
    //    return;
    //}

    //KxNoAttrMiniColorPicker * miniColorPickerDialog = 
    //    KxNoAttrMiniColorPicker::getNoAttrMiniColorEditor(false);

    //miniColorPickerDialog->setInitialColor(initialColor);
    //miniColorPickerDialog->centerDialogOn(QCursor::pos());

    //connect(miniColorPickerDialog, SIGNAL(colorAccepted(const QColor &)), this, SLOT(newColor(const QColor &)));
    //connect(miniColorPickerDialog, SIGNAL(colorRejected(const QColor &)), this, SLOT(colorRejected(const QColor &)));
    //connect(miniColorPickerDialog, SIGNAL(interactiveEdit(const QColor &)), this, SLOT(handleTransientCanvasValue(const QColor &)));
    //connect(miniColorPickerDialog, SIGNAL(colorChanged(const QColor &)), this, SLOT(handleFinalCanvasValue(const QColor &)));

    //miniColorPickerDialog->show();
}

void KxColorSliderGrp::canvasDoubleClicked()
{
    QColor qc = canvas()->palette().color(QPalette::Background);
    qreal	r, g, b, a;
    qc.getRgbF(&r, &g, &b, &a);
   /* QColor initialColor((float)r, (float)g, (float)b, (float)a);

    KxNoAttrColorPicker * largeColorPickerDialog = 
        KxNoAttrColorPicker::getNoAttrColorEditor(false);

    largeColorPickerDialog->addingConnections(this);

    largeColorPickerDialog->setModal(false);
    largeColorPickerDialog->setInitialColor(initialColor);

    connect(largeColorPickerDialog, SIGNAL(colorAccepted(const QColor &)), this, SLOT(newColor(const QColor &)));
    connect(largeColorPickerDialog, SIGNAL(colorRejected(const QColor &)), this, SLOT(colorRejected(const QColor &)));
    connect(largeColorPickerDialog, SIGNAL(interactiveEdit(const QColor &)), this, SLOT(handleTransientCanvasValue(const QColor &)));
    connect(largeColorPickerDialog, SIGNAL(colorChanged(const QColor &)), this, SLOT(handleFinalCanvasValue(const QColor &)));

    largeColorPickerDialog->show();*/
}

void KxColorSliderGrp::newColor(const QColor & newColor)
{
    setColor(newColor);

    emit newValueForConnections(newColor, false);
    emit groupChangeCompleted();

    disconnectFromColorPickers();
}

void KxColorSliderGrp::colorRejected(const QColor & initialColor)
{
    newColor(initialColor);
}

void KxColorSliderGrp::disconnectFromColorPickers()
{
    /*KxNoAttrColorPicker * largeColorPickerDialog = 
        KxNoAttrColorPicker::getNoAttrColorEditor(false);
    largeColorPickerDialog->resetConnections();

    KxNoAttrMiniColorPicker * miniColorPickerDialog = 
        KxNoAttrMiniColorPicker::getNoAttrMiniColorEditor(false);
    disconnect(miniColorPickerDialog, 0, this, 0);*/
}

// Description:	 This procedure is called when the slider has finished
// 				 changing value, either at the end of a drag or in response
// 				 to an instantaneous action.
//
//				 We update the canvas and inform watchers of the new color.
//
void KxColorSliderGrp::handleFinalSliderValue()
{
    /*
    if (mayaSlider())
    {
        double newValue = mayaSlider()->sliderValueF();

        if(fValue != newValue)
        {
            fValue = newValue;

            if (canvas()) updateCanvas();

            fChangeCompleteRequired = true;
        }

        //	If a meaningful change took place, let everyone know about it.
        if (fChangeCompleteRequired)
        {
            //	Let connections know that the user is done changing the value.
            emit newValueForConnections(qcolor(), false);

            //	Let the group know about the change as well.
            emit groupChangeCompleted();

            fChangeCompleteRequired = false;
        }
    }
    */
}

// Description:	 This procedure is called after the slider is moved.
//				 We want to use the slider's value to update the canvas
//				 and inform watchers of the new color.
//
void KxColorSliderGrp::handleTransientSliderValue()
{
    /*
    if (mayaSlider())
    {
        double newValue = mayaSlider()->sliderValueF();

        if(fValue != newValue)
        {
            fValue = newValue;

            if (canvas()) updateCanvas();

            //	Let connections know about the new value.
            QColor newColor = qcolor();
            emit newValueForConnections(newColor, true);

            //	Execute the user's drag command, if any.
            //KxQtHelper::executeCommandScript("dragCommand", this);

            //	Even if there are no further changes in value, make sure
            //	that we send out a final signal when the slider is
            //	released.
            fChangeCompleteRequired = true;
        }
    }
    */
}

// Description:	 This procedure is called when the colorpicker has finished
// 				 changing, either at the end of a drag.
//
//				 We update the swatch/slider and inform watchers of the new color.
//
void KxColorSliderGrp::handleFinalCanvasValue(const QColor & newColor)
{
    if (newColor != QColor() || fChangeCompleteRequired)
    {
        setColor(newColor);

        emit newValueForConnections(newColor, false);
        emit groupChangeCompleted();

        fChangeCompleteRequired = false;
    }
}

// Description:	 This procedure is called while the color picker is editing the color.
//				 We update the swatch/slider and inform watchers of the new color.
//
void KxColorSliderGrp::handleTransientCanvasValue(const QColor & newColor)
{
    double r, g, b;
    newColor.getRgbF(&r, &g, &b);
    setColor(newColor);

    //	Let connections know about the new value.
    emit newValueForConnections(newColor, true);

    //	Execute the user's drag command, if any.
    //KxQtHelper::executeCommandScript("dragCommand", this);

    fChangeCompleteRequired = true;
}

void KxColorSliderGrp::updateCanvas()
{
    QColor canvasColor = QColor();
    QPalette canvasPalette(canvasColor);
    fCanvas->setPalette(canvasPalette);
    fCanvas->setAutoFillBackground(true);
}


void KxColorSliderGrp::newValueFromConnection(const QVariant& value)
{
    if (value.canConvert<QColor>())
    {
        QColor	color = value.value<QColor>();
        setColor(color);
    }
}


void KxColorSliderGrp::setReadOnly(bool readOnly)
{
    if (fReadOnly != readOnly) {
        if (fSlider) fSlider->setDisabled(readOnly);
        if (fCanvas) fCanvas->setDisabled(readOnly);

        fReadOnly = readOnly;
    }
}
