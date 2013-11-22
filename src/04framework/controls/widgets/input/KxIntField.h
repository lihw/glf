#ifndef KxIntField_H_
#define KxIntField_H_

#include "../../base/HQtGui.h"
#include "KxNumericField.h"

class KX_DLLEXPORT KxIntField : public KxNumericField
{
    Q_OBJECT

public:
    KxIntField(QWidget *parent = 0);

    virtual void setValueF(double);
    virtual double valueF() const;
    void    setValue(int);
    int  value() const;

    virtual double minValueF() const;
    void            setMinValue(int);
    int          minValue() const;

    virtual double maxValueF() const;
    void            setMaxValue(int);
    int          maxValue() const;

    virtual double deltaF() const;
    void            setDelta(int);
    int          delta() const;

    //---------------------------------
    //    QIconnectableControl Interface
    //
    public slots:
        //  Invoked by the entity at the other end of the connection whenever
        //  it has a new value for the control to display.
        virtual void newValueFromConnection(const QVariant& value);

        //  This is not officially a part of the QIconnectableControl
        //  interface, but KxField provides it to make it easier for derived
        //  classes to provide connections with data in compatible formats.
        virtual void getValueForConnections(QVariant& value) const;

        //---------------------------------

private:
    int          fDelta;
};

inline double KxIntField::minValueF() const
{
    return minValue();
}

inline double KxIntField::maxValueF() const
{
    return maxValue();
}

inline void KxIntField::setValueF(double newValue)
{
    setValue((int)newValue);
}

inline double KxIntField::valueF() const
{
    return value();
}

inline double KxIntField::deltaF() const
{
    return fDelta; 
}

inline int KxIntField::delta() const
{
    return fDelta; 
}

inline void KxIntField::getValueForConnections(QVariant& outValue) const
{
    outValue.setValue(value());
}

#endif