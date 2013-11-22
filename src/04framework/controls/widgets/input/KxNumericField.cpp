#include "KxNumericField.h"
#include "../../base/HQtGui.h"

KxNumericField::KxNumericField(QWidget *parent /* = 0 */)
:KxField(parent)
,fInvisioSliderIsActivated(false)
{
    // We set the contextMenuPolicy, so that the default
    // context menu does not show up on right mouse button release.
    // For the numeric field, we use the Ctrl-RightMouseButton drag 
    // for invisio-slider.  In which case, when we release the right
    // mouse button, we do not want to see the default context menu.
    //
    setContextMenuPolicy(Qt::CustomContextMenu);
}


void KxNumericField::mousePressEvent(QMouseEvent *ev)
{
    // check to see the invisio-slider should be activated 
    fInvisioSliderIsActivated = false;
    if(ev != NULL && ev->modifiers() == Qt::ControlModifier){
        fInvisioSliderIsActivated = true;
        fInvisioSliderInitial =   ev->globalPos();
        fInvisioSliderInitialValue = valueF();
        fInvisioSliderCurrentValue = valueF();
        fInvisioSliderWhichButton = ev->button();
        fInvisioSliderMinValueF = minValueF();
        fInvisioSliderMaxValueF = maxValueF();
        fInvisioSliderDeltaF = deltaF();

        // we don't want focus to be given to the field in invisio-slider mode 
        setFocusPolicy(Qt::TabFocus);
        if(hasFocus()){
            clearFocus();
        }
        return;
    }else{
       // if not in invisio-lsider mode, the field shoud get focus as normal 
        setFocusPolicy(Qt::StrongFocus);
        if(!hasFocus()){
            setFocus();
        }
    }
}

void KxNumericField::mouseReleaseEvent(QMouseEvent *ev)
{
    // whence the mouse is released , the invisio-slider is de-activated 
    if(fInvisioSliderIsActivated){
        fInvisioSliderIsActivated= false;
        // check if the value in the fiel has changed due to this 
        // inviso-slider operation , if so , trigger the changed command 
        if(fInvisioSliderInitialValue != valueF()){
             // update the old text variable , so when we lose focus 
            // handleEditFinish() will know if the value has changed or not 
            fOldText = text();
            // let connections know about the new vlaue 
            updateConnections(false);
            // trigger the change command 
            emit changeCompleted();
        }
        return;
    }
    KxField::mouseReleaseEvent(ev);
}


void KxNumericField::mouseMoveEvent(QMouseEvent *ev)
{
    if(fInvisioSliderIsActivated && ev  != NULL && !isReadOnly()){
        // handle mouse drag , which behaves like an invisible slider
        // get the new x position and compare it with initialX 
        int deltaX = ev->globalPos().x() - fInvisioSliderInitial.x();
        if(deltaX != 0 ){
            // vary the amount of change with the mouse button 
            double step;
            if(fInvisioSliderWhichButton == Qt::LeftButton){
                step = fInvisioSliderDeltaF / 10.0;
            }else if(fInvisioSliderWhichButton == Qt::MidButton){
                step = fInvisioSliderDeltaF;
            }else if(fInvisioSliderWhichButton == Qt::RightButton){
                step = fInvisioSliderDeltaF * 10.0;
            }else {
                step = fInvisioSliderDeltaF;
            }
            // the value of the slider depends only on the position of the mouse. This
            // makes the "virtual slider" consistent in that the same value is always used
            // at the same mouse position.
            fInvisioSliderCurrentValue = fInvisioSliderInitialValue + step * deltaX;
            if(fInvisioSliderCurrentValue >= fInvisioSliderMaxValueF){
                fInvisioSliderCurrentValue = fInvisioSliderMaxValueF;
            }else if(fInvisioSliderCurrentValue <= fInvisioSliderMinValueF){
                fInvisioSliderCurrentValue = fInvisioSliderMinValueF;
            }
            setValueF(fInvisioSliderCurrentValue);
            // let connections know about the new vlaue 
            updateConnections(true);
            // trigger the command 
            //KxQtHelper::executeCommandScript("dragCommand", this);

            //// If the field is part of a slider group, the dragCommand of the slider should be triggered,
            //// since the dragCommands of slider groups are set to the sliders, not the fields.
            ////
            //KxSliderGrp *sliderGrp = qobject_cast<KxSliderGrp *>(parentWidget());
            //if(sliderGrp) {
            //   KxQtHelper::executeCommandScript("dragCommand", sliderGrp->KxSlider());
            //}
        }
        return;
    }
    KxField::mouseMoveEvent(ev);
}


void KxNumericField::makeValueInvalid()
{
    int pos = 0;
    QString texQString = text();
    if(validator() != NULL && validator()->validate(texQString,pos) == QValidator::Acceptable){
        // input is already valid , but set the float value again just in case the input was an integer value which is 
        // considered acceptable 
        setValueF(valueF());
        return;
    }
   // make sure the value in the input field is within range 
    double currentValue = valueF();
    if(currentValue < minValueF()){
        setValueF(minValueF());
        return;
    }

    if(currentValue > maxValueF()){
        setValueF(maxValueF());
        return;
    }
    if(validator() != NULL && validator()->validate(texQString,pos) != QValidator::Acceptable){
        setValueF(currentValue);
    }
}


