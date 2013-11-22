#ifndef KxIconTextRadioButton_H_
#define KxIconTextRadioButton_H_

#include "../../base/HQtGui.h"
#include "KxIconTextControl.h"


class KX_DLLEXPORT KxIconTextRadioButton : public QPushButton 
{
    Q_OBJECT
public:
    KxIconTextRadioButton(QWidget *parent=0);

    virtual QSize		sizeHint() const;

    //virtual void		setChangedCommand(Taction * action);
    //Taction *			changedCommand() const;
    //void				setOnCommand(Taction * action);
    //Taction *			onCommand() const;
    //void				setOffCommand(Taction * action);
    //Taction *			offCommand() const;

    KxIconTextControl *	iconTextControl();

signals:
    void checkStateChangedToOn();
    void checkStateChangedToOff();

    private slots:
        void onOffChanged();

protected:
    void mouseReleaseEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);

private:
    KxIconTextControl *	fIconTextControl;
};

#endif