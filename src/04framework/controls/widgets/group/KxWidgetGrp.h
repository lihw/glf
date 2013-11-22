#ifndef KxWidgetGrp_H_
#define KxWidgetGrp_H_
#include "../../base/HQtGui.h"

class KxLabel;
class KxSlider;
class KxField;
class KxOptionMenu;
class KxRowColumnLayout;
class KxRowLayout;

class KX_DLLEXPORT KxWidgetGrp : public QWidget
{
    Q_OBJECT

public:
    KxWidgetGrp(QWidget *parent=0);

    enum OffsetSide { kLeftSide, kRightSide, kTopSide, kBottomSide, kBothHSides, kBothVSides };

    QString     	labelText() const;
    void        	setLabelText( const QString &);
    QString     	extraLabelText() const;
    void        	setExtraLabelText( const QString &);
    void        	addLabel();
    void        	addLabel(KxLabel*);
    void        	addExtraLabel();
    void        	addExtraLabel(KxLabel*);
    KxLabel* 	label();
    KxLabel*     extraLabel();
    void        	addWidgetToGrp(QWidget *newWidget);
    QWidget*    	widget(unsigned int index);

    int				numRows();
    int				numColumns();

    virtual	void	makeVertical();
    bool			isVertical();

    void			setColumnWidth(int index, int width, bool adjustable = false);
    void			setColumnAlignment(int index, Qt::Alignment alignment);
    void			setColumnAttachment(int index, OffsetSide side, int offset);
    void			setRowAttachment(int index, OffsetSide side, int offset);
    void			setColumnAdjustable(int index);

    virtual KxSlider* 	mayaSlider();
    virtual KxField* 	kxField();
    virtual QComboBox* 		comboBox();

signals:
    void        	groupChangeCompleted();

protected:
    KxLabel				*fLabel;
    QLayout					*fLayout;
    KxRowLayout			*fRowLayout;
    KxRowColumnLayout	*fRowColumnLayout;
    KxLabel				*fExtraLabel;

private:
    bool		fIsVertical;
    int			fExtraLabelWidth;
};

inline KxLabel * KxWidgetGrp::label()
{
    return fLabel;
}

inline KxLabel * KxWidgetGrp::extraLabel()
{
    return fExtraLabel;
}

inline KxSlider * KxWidgetGrp::mayaSlider()
{
    return NULL;
}

inline KxField * KxWidgetGrp::kxField()
{
    return NULL;
}

inline QComboBox* KxWidgetGrp::comboBox()
{
    return NULL;
}
#endif