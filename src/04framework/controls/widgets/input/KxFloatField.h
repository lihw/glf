#ifndef KxFloatField_H_
#define KxFloatField_H_
#include "../../base/HQtGui.h"
#include "KxNumericField.h"

class KX_DLLEXPORT KxFloatField  : public KxNumericField
{
    Q_OBJECT
public:
    KxFloatField(QWidget *parent = 0);

    void setMinValueF(double );
    virtual double minValueF() const;

    void setMaxValueF(double );
    virtual double maxValueF() const;

    virtual void setValueF(double );
    virtual double valueF() const;
    double  inputValueF() const;

    void setDeltaF(double);
    virtual double deltaF() const;

    void setDecimals(int );
    virtual int decimals() const;

    virtual QSize sizeHint() const;

    // KxIconnectableControl interface 
public slots:
    // invoked by the entity at the other end of of the connection 
    // whenever it has a new vlaue for the control to display 
    virtual void newValueFromConnection(const QVariant &value);

    //this is not offcially a part of the KxIconnectableControl 
    // interface , but KxField provide it to make it easier for the derived
    // classes to provide connections with data in the comptible formats
    virtual void getValueForConnections(QVariant& value) const;

protected:
    virtual void mouseDoubleClickEvent(QMouseEvent *ev);

private:
    double fDeltaF;
    double fInputValueF;
    unsigned int fNumDecimals;
    QDoubleValidator * fRangeValidator;
};


inline double KxFloatField::deltaF() const
{
    return fDeltaF;
}

inline void KxFloatField::getValueForConnections(QVariant& value) const
{
    value.setValue(fInputValueF);
}

#endif