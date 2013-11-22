#ifndef KxMenuItem_H_
#define KxMenuItem_H_
#include "../base/HQtGui.h"

class KxMenuItem;

/// \class KxMenuItemWidget
/// \brief A menu item that can draw option boxes
class KX_DLLEXPORT KxMenuItemWidget : public QWidget
{
public:
    KxMenuItemWidget(KxMenuItem *item, QWidget *parent = NULL);

    virtual QSize sizeHint() const;

protected:
    virtual void paintEvent(QPaintEvent *event);
    virtual void mouseMoveEvent(QMouseEvent * event);
    virtual void mouseReleaseEvent(QMouseEvent * event);
    virtual void tabletEvent(QTabletEvent* event);

    QStyleOptionMenuItem getStyleOption() const;

private:
    KxMenuItem *fMenuItem;

    // for dealing with submenu clicks from tablets
    int fTabletMoveSkipCount;
    bool fSkippingTabletMoves;
    static const int sNumTabletMovesToSkip;
};

/// \class KxMenuItem
/// \brief A QAction that can handle option boxes, being repeatable and being on a Shelf
class KX_DLLEXPORT KxMenuItem : public QWidgetAction
{
    Q_OBJECT

public:
    KxMenuItem		(QObject *parent);
    KxMenuItem		(const QString &text, QObject *parent);

    // Marking Menu positions
    enum {
        kMMPositionNone			= -1,	///< no specific position
        kMMPositionNorth		= 0,	///< top
        kMMPositionNorthEast	= 1,	///< top-right
        kMMPositionEast			= 2,	///< left
        kMMPositionSouthEast	= 3,	///< bottom-right
        kMMPositionSouth		= 4,	///< bottom
        kMMPositionSouthWest	= 5,	///< bottom-left
        kMMPositionWest			= 6,	///< left
        kMMPositionNorthWest	= 7		///< top-left
    };

    void	        setRadialPosition	(int p);
    int	        	getRadialPosition	();

    bool	        hasOptionBox		() const;
    void	        setOptionBoxAction	(QAction *);

    void            setValue			(const QVariant& value);
    const QVariant& getValue			() const;

    void			setRepeatable		(bool repeat);
    bool			isRepeatable		() const;

    QAction *	    optionBoxAction		() const;

    enum {
        kMainAction,
        kOptionBoxAction
    };

    bool			saveToShelf			(int whichAction);

    public slots:
        void			onToggled(bool);

protected:
    virtual QWidget * createWidget		(QWidget *parent);
    virtual void		deleteWidget(QWidget *widget);

    QVariant    fValue;

private:
    int			fRadialPosition;
    QAction *	fOptionBoxAction;
    bool		fIsRepeatable;
};

inline void KxMenuItem::setRadialPosition(int p)
{
    fRadialPosition = p;
}

inline int KxMenuItem::getRadialPosition()
{
    return fRadialPosition;
}

inline bool KxMenuItem::hasOptionBox() const
{
    return (NULL != fOptionBoxAction);
}


inline void KxMenuItem::setOptionBoxAction(QAction *action)
{
    fOptionBoxAction = action;
}

inline void KxMenuItem::setValue(const QVariant& value)
{
    fValue = value;
}

inline const QVariant& KxMenuItem::getValue() const
{
    return fValue;
}

inline QAction * KxMenuItem::optionBoxAction() const
{
    return fOptionBoxAction;
}

inline void KxMenuItem::setRepeatable(bool repeat)
{
    fIsRepeatable = repeat;
}

inline bool KxMenuItem::isRepeatable() const
{
    return fIsRepeatable;
}
#endif