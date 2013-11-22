#ifndef KxIconTextControl_H_
#define KxIconTextControl_H_
#include "../../base/HQtGui.h"
class KX_DLLEXPORT KxIconTextControl : public QWidget
{
    Q_OBJECT
public:
    enum KxIconTextStyle {
        NoStyle = -1,
        iconOnly = 0,
        textOnly,
        iconAndTextHorizontal,
        iconAndTextVertical,
        iconAndTextCentered
    };

    KxIconTextControl(QWidget *parent=0);
    ~KxIconTextControl();

    QSize 				sizeHint() const;
    QSize				minimumSize() const;

    bool				setIcon(const QString &, int index = 0,
        bool force=false, bool useDefaultOnFailure=true);
    QString				icon(int index = 0) const;

    virtual bool		setSelectionIcon(const QString &);
    QString				selectionIcon() const;

    virtual bool		setHighlightIcon(const QString &);
    QString				highlightIcon() const;

    virtual bool		setSelectionHighlightIcon(const QString &);
    QString				selectionHighlightIcon() const;

    virtual bool		setDisabledIcon(const QString &);
    QString				disabledIcon() const;

    void				setText(const QString &text);
    QString				text() const;

    void				setOverlayText(const QString &text);
    QString				overlayText() const;

    void				setOverlayColor(float color[3]);
    const float*		overlayColor() const;

    void				setOverlayBackColor(float color[4]);
    const float*		overlayBackColor() const;

    void				setStyle(KxIconTextStyle style);
    KxIconTextStyle		style() const;

    virtual void		setAlignment(const Qt::Alignment, bool explictlySet = false);
    Qt::Alignment		alignment() const;

    virtual void		setLabelOffset(const int);
    int					labelOffset() const;

    void        setCellSize(const int , const int);
    void                cellSize(int &width , int &height)const;

    void				setSelected(bool selected);
    void				editLabel();

signals:
    void				labelChanged(QString newLabel);

    private slots:
        void				acceptNewLabel();

protected:
    void				enterEvent(QEvent * event);
    void				leaveEvent(QEvent * event);
    void				changeEvent(QEvent * event);
    void				resizeEvent(QResizeEvent *event);

private:
    QPixmap *			createIconPixmap(const QString&);
    void				updateIcon(bool needToChooseIcon = false);
    QPixmap *			chooseIcon();
    void				centerIconAndText();

    QLabel *			fPixmap;
    QLabel *			fLabel;
    int					fLabelOffset;
    Qt::Alignment		fLabelAlignment;
    int                 fCellWidth;
    int                 fCellHeight;
    QLayout *			fLayout;
    QPointer<QLineEdit> fLabelEditor;
    QObject *			fLabelChangeListener;

    KxIconTextStyle		fStyle;

    QString				fOverlayText;		//!< Text drawn on top of icon
    float				fOverlayColor[3];	//!< TColor of fOverlayText text
    float				fOverlayBackColor[4];	//!< TColor of fOverlayText background

    QHash<int, QString>		fIconNameArray;
    QString					fSelectionIconName;
    QString					fHighlightIconName;
    QString					fSelectionHighlightIconName;
    QString					fDisabledIconName;

    QPixmap *				fPixmapWithOverlay;	//!< Copy of current icon with overlayed text
    QHash<int, QPixmap *>	fIconPixmapArray;
    QPixmap *				fSelectionIconPixmap;
    QPixmap *				fHighlightIconPixmap;
    QPixmap *				fSelectionHighlightIconPixmap;
    QPixmap *				fDisabledIconPixmap;

    bool				fIsSelected;
    bool				fIsHighlighted;

    int					fCurrentIconPixmapIndex;
};

#endif