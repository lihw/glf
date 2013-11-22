#ifndef KxSliderGrp_H_
#define KxSliderGrp_H_

#include "../../base/HQtGui.h"

#include "KxConnectableWidgetGrp.h"
class KxLabel;
class KxNumericField;
class KxSlider;


class KX_DLLEXPORT KxSliderGrp : public KxConnectableWidgetGrp
{
    Q_OBJECT

public:
    KxSliderGrp(QWidget *parent, 
        bool hasLabel, 
        KxNumericField * newField = NULL, 
        KxSlider * newSlider = NULL,
        bool hasExtraLabel = false);
    KxSliderGrp(QWidget *parent,
        KxLabel *newLabel = NULL, 
        KxNumericField * newField = NULL, 
        KxSlider * newSlider = NULL,
        bool hasExtraLabel = false);
    void init(  KxLabel *newLabel = NULL, 
        KxNumericField * newField = NULL, 
        KxSlider * newSlider = NULL,
        bool hasExtraLabel = false);

    virtual void				setFieldValue(double newValue);
    virtual void				setSliderValue(double newValue);
    virtual void				setValue(double newValue);
    virtual KxSlider *       mayaSlider() = 0;
    virtual KxNumericField * numericField() = 0;

protected:
    void                        insertSlider(KxSlider* newSlider);
    void						insertSliderSpace(int index);
    void						removeSlider(KxSlider* slider);

    void						makeConnections(
        KxNumericField* field, KxSlider* slider
        );

    public slots:
        void            useSliderValueToSetFieldValue();
        void            useFieldValueToSetSliderValue();
};

#endif