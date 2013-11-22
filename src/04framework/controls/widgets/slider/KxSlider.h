#ifndef KxSlider_H_
#define KxSlider_H_

#include "../../base/HQtGui.h"

#include "../../base/KxIconnectableControl.h"

class KX_DLLEXPORT KxSlider : public QSlider, public KxIconnectableControl
{
    Q_OBJECT

public:
    KxSlider(Qt::Orientation orientation, QWidget *parent = 0); 
    virtual void    getSliderValue(QVariant& value) = 0;
    virtual double sliderValueF() = 0;
    virtual void    setSliderValue(const QVariant& value) = 0;
    virtual void    setSliderValueF(double) = 0;
    virtual double sliderMinimumF() const = 0;
    virtual double sliderMaximumF() const = 0;
    virtual void    setSliderRangeF(double, double) = 0;

signals:
    //  Emitted whenever the value changes while dragging the slider.
    void transientSliderChange();

    //  Emitted when the user has finished dragging the slider, or in
    //  response to an instantaneous change such as clicking on the slide.
    void sliderChangeCompleted();

protected:
    virtual void mousePressEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void wheelEvent(QWheelEvent * event);

    private slots:
        void flagUserAction(int action);
        void dragComplete();
        void handleSliderValueChanged(int newValue);

        //---------------------------------
        //    KxIconnectableControl Interface
        //
        public slots:
            //  Invoked by the entity at the other end of the connection whenever
            //  it has a new value for the control to display.
            virtual void newValueFromConnection(const QVariant& value);

signals:
            //  Emitted by the control whenever the user changes its value, so
            //  that connected entities can immediately be updated.
            void newValueForConnections(
                const QVariant& newValue, bool isInterim
                );

            //---------------------------------

private:
    bool fDragInProgress;
    bool fIsUserAction;
    int  fOldRawValue;
};


#endif