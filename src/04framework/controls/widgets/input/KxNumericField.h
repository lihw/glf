#ifndef KxNumericField_H_
#define KxNumericField_H_
#include "KxField.h"

class KX_DLLEXPORT KxNumericField : public KxField 
{
    Q_OBJECT
public:
    KxNumericField(QWidget *parent = 0);

    virtual double minValueF() const = 0;
    virtual double maxValueF() const = 0;
    virtual void   setValueF(double ) = 0;
    virtual double valueF() const = 0;
    virtual double deltaF() const = 0;

signals:
    void numericValueChanged();

protected:
    virtual void mousePressEvent(QMouseEvent *ev);
    virtual void mouseReleaseEvent(QMouseEvent *);
    virtual void mouseMoveEvent(QMouseEvent *);
    virtual void makeValueInvalid();

private:
    // invisio -sldier related values 
    bool fInvisioSliderIsActivated;
    QPoint fInvisioSliderInitial;
    double fInvisioSliderInitialValue;
    double fInvisioSliderCurrentValue;
    double fInvisioSliderMinValueF;
    double fInvisioSliderMaxValueF;
    double fInvisioSliderDeltaF;
    Qt::MouseButton  fInvisioSliderWhichButton;
};

#endif