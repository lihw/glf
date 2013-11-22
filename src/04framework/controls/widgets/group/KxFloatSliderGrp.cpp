#include "KxFloatSliderGrp.h"

// Description:  Constructor
//
KxFloatSliderGrp::KxFloatSliderGrp(
    QWidget*            parent,
    bool                hasLabel,
    KxFloatField*    newField,
    KxFloatSlider*   newSlider,
    bool                hasExtraLabel
    )
    : KxSliderGrp(parent, hasLabel, newField, newSlider, hasExtraLabel)
    , fFloatField(newField)
    , fFloatSlider(newSlider)
{
    init();
}


// Description:  Constructor
//
KxFloatSliderGrp::KxFloatSliderGrp(
    QWidget*            parent,
    KxLabel*         newLabel,
    KxFloatField*    newField,
    KxFloatSlider*   newSlider,
    bool                hasExtraLabel
    )
    : KxSliderGrp(parent, newLabel, newField, newSlider, hasExtraLabel)
    , fFloatField(newField)
    , fFloatSlider(newSlider)
{
    init();
};


void KxFloatSliderGrp::addSlider(KxFloatSlider* newSlider)
{
    //	Get rid of any existing slider.
    removeSlider();

    fFloatSlider = newSlider;

    if (newSlider)
    {
        initSlider();

        //	Insert the new slider into the appropriate position in the
        //	layout.
        insertSlider(newSlider);
    }
}


// Description:  Common initialization for all constructors.
//
void KxFloatSliderGrp::init()
{
    initField();
    initSlider();
}


void KxFloatSliderGrp::initField()
{
    if (fFloatField)
    {
        //  Whenever the field emits a 'newValueForConnections' signal,
        //  have us emit one as well so that anyone connected directly to
        //  the group will get it.
        connect(
            fFloatField,
            SIGNAL(newValueForConnections(const QVariant&, bool)),
            this,
            SIGNAL(newValueForConnections(const QVariant&, bool))
            );

        //  If this field supports expressions, then do the same for the
        //  'newExpressionForConnections' signal.
        static QByteArray kNewExpressionForConnectionsSig =
            QMetaObject::normalizedSignature(
            "newExpressionForConnections(const QString&)"
            );
        const QMetaObject*  ctrlInfo = fFloatField->metaObject();

        if (ctrlInfo->indexOfSignal(kNewExpressionForConnectionsSig) != -1)
        {
            connect(
                fFloatField,
                SIGNAL(newExpressionForConnections(const QString&)),
                this,
                SIGNAL(newExpressionForConnections(const QString&))
                );
        }
    }
}


void KxFloatSliderGrp::initSlider()
{
    //  Whenever the slider emits a 'newValueForConnections' signal, have us
    //  emit one as well so that anyone connected directly to the group
    //  will get it.
    if (fFloatSlider)
    {
        connect(
            fFloatSlider,
            SIGNAL(newValueForConnections(const QVariant&, bool)),
            this,
            SIGNAL(newValueForConnections(const QVariant&, bool))
            );
    }
}


void KxFloatSliderGrp::newValueFromConnection(const QVariant& value)
{
    if (fFloatField) fFloatField->setValueF(value.value<double>());
    if (fFloatSlider) fFloatSlider->setSliderValue(value);
}


void KxFloatSliderGrp::removeSlider()
{
    if (fFloatSlider) 
    {
        KxSliderGrp::removeSlider(fFloatSlider);
        fFloatSlider = NULL;
    }
}

