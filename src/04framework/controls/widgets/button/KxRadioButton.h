#ifndef  KxRadioButton_H_
#define  KxRadioButton_H_
#include "../../base/HQtGui.h"


class KX_DLLEXPORT KxRadioButton : public QRadioButton
{
    Q_OBJECT

public:
    KxRadioButton(const QString & text = QString(), QWidget *parent=0);

    bool isReadOnly() const;
    void setReadOnly(bool readOnly);

signals: 
    void checkStateChangedToOn();
    void checkStateChangedToOff();

    protected slots:
        void onOffChanged(bool checked);
};

#endif