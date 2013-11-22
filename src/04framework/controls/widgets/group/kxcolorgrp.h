#ifndef KxColorSliderGrp_H_
#define KxColorSliderGrp_H_

#include "../../base/HQtGui.h"

#include "KxConnectableWidgetGrp.h"
#include "KxIntSliderGrp.h"

class KX_DLLEXPORT KxColorGrp : public KxConnectableWidgetGrp
{
    Q_OBJECT

public:
    KxColorGrp(QWidget* parent);

    virtual ~KxColorGrp() {}

    const QColor color() const;
    void setColor(const QColor& newColor);

    QLabel*             canvas()     { return fCanvas; }
    KxIntSliderGrp*     redSliderGrp() { return fRedSliderGrp; }
    KxIntSliderGrp*     greenSliderGrp() { return fGreenSliderGrp; }
    KxIntSliderGrp*     blueSliderGrp() { return fBlueSliderGrp; }
    KxIntSliderGrp*     alphaSliderGrp() { return fAlphaSliderGrp; }

public slots:
    //	Invoked by the entity at the other end of the connection whenever
    //	it has a new value for the control.
    virtual void newValueFromConnection(const QVariant& value);

    void newColorFromSliderGrp(const QVariant& value, bool isInterim);
    void newColorFromColorField();

signals:
    //	Emitted by the control whenever its value changes, so that connected
    //	entities can immediately be updated.
    void newValueForConnections(const QVariant& newValue, bool isInterim);

private:
    void updateCanvas();
    void updatePicker();

    KxIntSliderGrp* createColorSlider(int currentValue);
        
private:
    QLabel* fCanvas;
    KxField* fColorField;
    KxIntSliderGrp* fRedSliderGrp;
    KxIntSliderGrp* fGreenSliderGrp;
    KxIntSliderGrp* fBlueSliderGrp;
    KxIntSliderGrp* fAlphaSliderGrp;

    QColor fColor;
};


#endif
