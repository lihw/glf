#ifndef KxProgressBar_H_
#define KxProgressBar_H_
#include "../../base/HQtGui.h"
#include "../../widgets/display/KxProgressIndicator.h"
#include "../../utils/KxWaitCursor.h"

class KX_DLLEXPORT KxProgressBar : public QProgressBar , public KxProgressIndicator 
{
    Q_OBJECT
public:
    KxProgressBar(QWidget *parent = 0);

    virtual 			~KxProgressBar();

    void				setRange( int min,  int max);

    int				minValue() const;
    int				maxValue() const;

    void				setProgress( int);
    int				progress() const;

    void				setStatus(const QString&);
    void				setIsInterruptable( bool);
    void				setShowPercentText( bool);

    void				setIsMainProgressBar( bool);
    bool 			isMainProgressBar() const;

    bool			isCancelled() const;

    void				startWithCursor();
    void				start();
    void				finishWithCursor();
    void				finish();

    static void			setCancelMessage(const QString & message);


    private slots:
        void                onValueChanged(int);
private:
    QString 			fProgressBarString;

    KxWaitCursor			fWaitCursor;

    static QString		fsCancelMessage;
    // Moved this variable from Tcontrol.h (temporarily?)
};
#endif