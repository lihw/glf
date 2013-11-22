#ifndef KxMessageBox_H_
#define KxMessageBox_H_
#include "../../base/HQtGui.h"

class KX_DLLEXPORT KxMessageBox : public QMessageBox
{
public:
    KxMessageBox(Icon icon, const QString & title, 
        const QString & text, 
        StandardButtons buttons = NoButton, 
        QWidget * parent = 0, 
        Qt::WindowFlags f = Qt::Dialog | Qt::MSWindowsFixedSizeDialogHint ); 

    // override addButton methods of QMessageBox to add Maya specific behaviour
    QPushButton* addButton(const QString & text, ButtonRole role);
    QPushButton* addButton(StandardButton button);

    public slots:
        int exec();

protected: 
    virtual void closeEvent(QCloseEvent *);
};

#endif