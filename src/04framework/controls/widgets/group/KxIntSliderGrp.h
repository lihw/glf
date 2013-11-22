#ifndef KxIntSliderGrp_H_
#define KxIntSliderGrp_H_

#include "../../base/HQtGui.h"
#include "KxSliderGrp.h"
#include "../slider/KxIntSlider.h"
#include "../input/KxIntField.h"


class KX_DLLEXPORT KxIntSliderGrp : public KxSliderGrp
{
    Q_OBJECT

public:
    KxIntSliderGrp(QWidget *parent = 0,
        bool hasLabel = false,
        KxIntField * newField = NULL,
        KxIntSlider * newSlider = NULL,
        bool hasExtraLabel = false);

    KxIntSlider * slider();
    KxIntField  * inputField();

    virtual KxSlider * mayaSlider();
    virtual KxField * kxField();

    //----------------------------------------------
    //    QIconnectableControl Interface
    //
    public slots:
        //  Invoked by the entity at the other end of the connection whenever
        //  it has a new value for the control to display.
        virtual void newValueFromConnection(const QVariant& value);

signals:
        //  Emitted by the control whenever the user changes its value, so that
        //  connected entities can immediately be updated.
        void newValueForConnections(
            const QVariant& newValue, bool isInterim
            );
        //
        //----------------------------------------------

protected:
    virtual KxNumericField * numericField();

private:
    KxIntSlider * fIntSlider;
    KxIntField  * fIntField;
};

inline KxSlider * KxIntSliderGrp::mayaSlider()
{
    return fIntSlider;
}

inline KxIntSlider * KxIntSliderGrp::slider()
{
    return fIntSlider;
}

inline KxNumericField * KxIntSliderGrp::numericField()
{
    return fIntField;
}

inline KxIntField * KxIntSliderGrp::inputField()
{
    return fIntField;
}

inline KxField * KxIntSliderGrp::kxField()
{
    return fIntField;
}

#endif