#ifndef KxFloatSliderGrp_H_
#define KxFloatSliderGrp_H_

#include "../../base/HQtGui.h"
#include "KxSliderGrp.h"
#include "../slider/KxFloatSlider.h"
#include "../input/KxFloatField.h"


class KX_DLLEXPORT KxFloatSliderGrp : public KxSliderGrp
{
    Q_OBJECT

public:
    KxFloatSliderGrp(QWidget *parent = 0,
        bool hasLabel = false,
        KxFloatField * newField = NULL,
        KxFloatSlider * newSlider = NULL,
        bool hasExtraLabel = false);

    KxFloatSliderGrp(QWidget *parent ,
        KxLabel * newLabel ,
        KxFloatField * newField = NULL,
        KxFloatSlider * newSlider = NULL,
        bool hasExtraLabel = false);

    virtual void addSlider(KxFloatSlider* slider);
    virtual void removeSlider();

    KxFloatSlider * slider() const;
    KxFloatField  * inputField() const;

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
        //	Emitted by controls which can provide expression strings whenever a
        //	new expression is made available or an existing expression is
        //	changed.
        void newExpressionForConnections(const QString& newExpr);

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
    void init();
    void initField();
    void initSlider();

    KxFloatSlider * fFloatSlider;
    KxFloatField  * fFloatField;
};

inline KxSlider * KxFloatSliderGrp::mayaSlider()
{
    return fFloatSlider;
}

inline KxFloatSlider * KxFloatSliderGrp::slider() const
{
    return fFloatSlider;
}

inline KxNumericField * KxFloatSliderGrp::numericField()
{
    return fFloatField;
}

inline KxFloatField * KxFloatSliderGrp::inputField() const
{
    return fFloatField;
}

inline KxField * KxFloatSliderGrp::kxField()
{
    return fFloatField;
}
#endif