#ifndef KxColorSliderGrp_H_
#define KxColorSliderGrp_H_
#include "../../base/HQtGui.h"
#include "KxConnectableWidgetGrp.h"


class KxSlider;
class KxLabel;

class KX_DLLEXPORT KxColorSliderLabel : public QLabel
{
    Q_OBJECT

public:
    KxColorSliderLabel(QWidget * parent = NULL);
    ~KxColorSliderLabel();


protected:
    void mousePressEvent( QMouseEvent * );
    void mouseReleaseEvent( QMouseEvent * );
    void mouseDoubleClickEvent( QMouseEvent * );

signals:
    void singleClicked();
    void doubleClicked();

    public slots:
        void singleClickResponder();

private:
    bool fDoubleClicked;
    QTime  fTime;
    QTimer fDoubleClickTimer;
};

class KX_DLLEXPORT KxColorSliderGrp : public KxConnectableWidgetGrp
{
    Q_OBJECT

public:
    KxColorSliderGrp(QWidget *parent = 0, 
        bool hasLabel = false, 
        KxColorSliderLabel * newCanvas = NULL, 
        KxSlider * newSlider = NULL);

    KxColorSliderGrp(QWidget *parent, 
        KxLabel  * newLabel,
        KxColorSliderLabel * newCanvas, 
        KxSlider * newSlider);

    virtual	~KxColorSliderGrp()	{}

    const QColor		color() const;
    void                setColor(const QColor &newColor);

    QLabel *			canvas()		{return fCanvas;}
    KxSlider *		mayaSlider()	{return fSlider;}

    //---------------------------------
    //	QIconnectableControl Interface
    //
    public slots:
        //	Invoked by the entity at the other end of the connection whenever
        //	it has a new value for the control.
        virtual void newValueFromConnection(const QVariant& value);

signals:
        //	Emitted by the control whenever its value changes, so that connected
        //	entities can immediately be updated.
        void newValueForConnections(
            const QVariant& newValue, bool isInterim
            );

        //---------------------------------

        public slots:
            void			canvasSingleClicked();
            void			canvasDoubleClicked();
            void			newColor(const QColor &);
            void			colorRejected(const QColor &);

            void			handleFinalCanvasValue(const QColor &);
            void			handleTransientCanvasValue(const QColor &);
            void			handleFinalSliderValue();
            void			handleTransientSliderValue();
            void			setReadOnly(bool);
            void			useColorValueToSetSliderValue();

protected:
    void			createRemainingControls(
        KxColorSliderLabel * canvas, KxSlider* slider
        );

    void			updateCanvas();

    void			disconnectFromColorPickers();

    bool			fReadOnly;

private:
    KxSlider *	fSlider;
    QLabel *		fCanvas;
    bool		fChangeCompleteRequired;
    double			fRValue;
    double			fGValue;
    double			fBValue;
};
#endif
