#ifndef QmayIconTextButton_H_
#define QmayIconTextButton_H_
#include "../../base/HQtGui.h"
#include "KxIconTextControl.h"

class KX_DLLEXPORT KxIconTextButton : public QPushButton 
{
    Q_OBJECT
public:
    KxIconTextButton(QWidget *parent=0);

    virtual QSize		sizeHint() const;

    //virtual void		setCommand(Taction * action);
    //Taction *			command() const;
    //void				setDoubleClickCmd(Taction * action);
    //Taction *			doubleClickCmd() const;

 /*   void				setLabelEditingCallback(TscriptAction*);*/

    KxIconTextControl *	iconTextControl() const;

    void	setRepeatable(bool repeat);
    bool	isRepeatable() const;

protected:
    void mouseReleaseEvent(QMouseEvent * event);
    void mouseDoubleClickEvent(QMouseEvent * event);
    void resizeEvent(QResizeEvent * event);

    protected slots:
        virtual bool executeSingleClickCommand();

        private slots:
            void executeLabelEditingCallback(QString newLabel);

private:
    KxIconTextControl *	fIconTextControl;
    QTimer				fDoubleClickTimer;
    bool				fGotDoubleClickEvent;
   /* TscriptAction*      fLabelEditingCallback;*/
    bool				fIsRepeatable;
};

inline void KxIconTextButton::setRepeatable(bool repeat)
{
    fIsRepeatable = repeat;
}

inline bool KxIconTextButton::isRepeatable() const
{
    return fIsRepeatable;
}
#endif