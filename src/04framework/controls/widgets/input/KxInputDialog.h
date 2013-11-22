#ifndef KxInputDialog_H_
#define KxInputDialog_H_
#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxInputDialog : public QDialog
{
    Q_OBJECT

public:
    enum Type { LineEdit, SpinBox, DoubleSpinBox};

    KxInputDialog(const QString &title, const QString &label, QWidget* parent, Type type, Qt::WindowFlags f = 0);
    ~KxInputDialog();

public:
    QString             getText( const QString &text = QString());
    int                 getInteger( int value, 
        int minValue = -2147483647, 
        int maxValue = 2147483647,
        int step = 1);
    double              getDouble(double value = 0, 
        double minValue = -2147483647, 
        double maxValue = 2147483647);

    void                setDefaultButton(QPushButton *button);
    QPushButton *       addButton(const QString &text);
    QAbstractButton *   clickedButton();
    void                setEscapeButton(QPushButton *button);

    private slots:
        void buttonClicked(QAbstractButton * button);

private:
    void                init(const QString &labelText, Type type);
    virtual void        keyPressEvent(QKeyEvent *e);

    QWidget             *fInput;
    QDialogButtonBox    *fButtonBox;
    QAbstractButton     *fClickedButton;
    QAbstractButton     *fEscapeButton;
};

// Description:  This procedure returns the button which got clicked.
//
inline QAbstractButton * KxInputDialog::clickedButton()
{
    return fClickedButton;
}

// Description:  This procedure remembers which button is clicked.
//
inline void KxInputDialog::buttonClicked(QAbstractButton *button)
{
    fClickedButton = button;
}

// Description:  This procedure sets the button which will be activated
//               when the escape key is pressed.
//
inline void KxInputDialog::setEscapeButton(QPushButton *button)
{
    fEscapeButton = button;
}

#endif